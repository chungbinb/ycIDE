#include "LibraryConfig.h"
#include "Keyword.h"
#include "LibraryParser.h"
#include <Windows.h>
#include <fstream>
#include <sstream>

LibraryConfigManager& LibraryConfigManager::GetInstance() {
    static LibraryConfigManager instance;
    return instance;
}

std::wstring LibraryConfigManager::GetConfigFilePath() {
    if (!configFilePath.empty()) {
        return configFilePath;
    }
    
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);
    std::wstring exeDir(exePath);
    size_t lastSlash = exeDir.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        exeDir = exeDir.substr(0, lastSlash);
    }
    
    configFilePath = exeDir + L"\\library_config.ini";
    return configFilePath;
}

void LibraryConfigManager::ScanLibraryFolders() {
    libraries.clear();
    
    // 获取程序目录
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);
    std::wstring exeDir(exePath);
    size_t lastSlash = exeDir.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        exeDir = exeDir.substr(0, lastSlash);
    }
    
    // 输出调试信息到文件
    CreateDirectoryW(L"logs", NULL);
    std::wstring debugPath = L"logs\\library_scan_debug.txt";
    HANDLE hDebug = CreateFileW(debugPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDebug != INVALID_HANDLE_VALUE) {
        std::wstring msg = L"程序目录: " + exeDir + L"\r\n";
        DWORD written;
        WriteFile(hDebug, msg.c_str(), static_cast<DWORD>(msg.length() * sizeof(wchar_t)), &written, NULL);
    }
    
    // 扫描lib文件夹（只支持.fne动态库）
    WIN32_FIND_DATAW findData;
    HANDLE hFind;
    std::wstring libFolder = exeDir + L"\\lib";
    std::wstring searchPath = libFolder + L"\\*.fne";
    
    if (hDebug != INVALID_HANDLE_VALUE) {
        std::wstring msg = L"扫描路径: " + searchPath + L"\r\n";
        DWORD written;
        WriteFile(hDebug, msg.c_str(), static_cast<DWORD>(msg.length() * sizeof(wchar_t)), &written, NULL);
    }
    
    hFind = FindFirstFileW(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::wstring fileName = findData.cFileName;
                // 去掉扩展名
                size_t dotPos = fileName.find_last_of(L".");
                std::wstring libName = (dotPos != std::wstring::npos) ? 
                    fileName.substr(0, dotPos) : fileName;
                
                LibraryConfigItem item;
                item.name = libName;
                item.filePath = libFolder + L"\\" + fileName;
                item.loaded = false;
                libraries.push_back(item);
                
                if (hDebug != INVALID_HANDLE_VALUE) {
                    std::wstring msg = L"找到 .fne: " + fileName + L"\r\n";
                    DWORD written;
                    WriteFile(hDebug, msg.c_str(), static_cast<DWORD>(msg.length() * sizeof(wchar_t)), &written, NULL);
                }
            }
        } while (FindNextFileW(hFind, &findData));
        FindClose(hFind);
    } else {
        if (hDebug != INVALID_HANDLE_VALUE) {
            std::wstring msg = L"未找到 lib 文件夹或没有 .fne 文件\r\n";
            DWORD written;
            WriteFile(hDebug, msg.c_str(), static_cast<DWORD>(msg.length() * sizeof(wchar_t)), &written, NULL);
        }
    }
    
    // 不再扫描static_lib文件夹，因为静态库(.lib)无法在运行时动态加载
    // 如果需要使用静态库，应该在编译时链接
    
    if (hDebug != INVALID_HANDLE_VALUE) {
        std::wstring msg = L"\r\n总共找到 " + std::to_wstring(libraries.size()) + L" 个支持库（仅统计可动态加载的.fne文件）\r\n";
        DWORD written;
        WriteFile(hDebug, msg.c_str(), static_cast<DWORD>(msg.length() * sizeof(wchar_t)), &written, NULL);
        CloseHandle(hDebug);
    }
}

void LibraryConfigManager::SetLibraryLoaded(size_t index, bool loaded) {
    if (index < libraries.size()) {
        libraries[index].loaded = loaded;
    }
}

int LibraryConfigManager::LoadSelectedLibraries() {
    int count = 0;
    auto& keywordMgr = KeywordManager::GetInstance();
    auto& libParser = LibraryParser::GetInstance();
    
    for (auto& lib : libraries) {
        if (lib.loaded) {
            bool loadSuccess = false;
            
            // 尝试加载 .fne 文件（动态库）
            if (lib.filePath.find(L".fne") != std::wstring::npos) {
                if (libParser.LoadFneLibrary(lib.filePath)) {
                    count++;
                    loadSuccess = true;
                }
            }
            // .lib 静态库文件无法动态加载，取消选中
            else if (lib.filePath.find(L".lib") != std::wstring::npos) {
                lib.loaded = false;
                continue;
            }
            
            // 如果加载失败，取消选中状态
            if (!loadSuccess) {
                lib.loaded = false;
            }
        }
    }
    
    // 保存配置（更新失败的库的状态）
    SaveConfig();
    
    // 输出所有已加载的命令到文件供验证
    if (count > 0) {
        // 获取程序当前目录
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);
        std::wstring exeDir = exePath;
        size_t lastSlash = exeDir.find_last_of(L"\\");
        if (lastSlash != std::wstring::npos) {
            exeDir = exeDir.substr(0, lastSlash);
        }
        CreateDirectoryW(L"logs", NULL);
        std::wstring debugPath = L"logs\\loaded_commands.txt";
        
        HANDLE hDebug = CreateFileW(debugPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 
                                    NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hDebug != INVALID_HANDLE_VALUE) {
            // 写入UTF-8 BOM
            BYTE bom[3] = {0xEF, 0xBB, 0xBF};
            DWORD written;
            WriteFile(hDebug, bom, 3, &written, NULL);
            
            // 构建内容
            std::wstring content = L"已加载 " + std::to_wstring(count) + L" 个支持库，共 " + 
                                   std::to_wstring(libParser.GetCommands().size()) + L" 个命令\r\n\r\n";
            
            // 按支持库分组输出
            std::map<std::wstring, std::vector<const LibraryCommand*>> cmdsByLib;
            for (const auto& cmd : libParser.GetCommands()) {
                cmdsByLib[cmd.library].push_back(&cmd);
            }
            
            for (const auto& pair : cmdsByLib) {
                content += L"【" + pair.first + L"】(" + 
                          std::to_wstring(pair.second.size()) + L" 个命令)\r\n";
                
                for (const auto* cmd : pair.second) {
                    content += L"  " + cmd->chineseName + L" : " + cmd->returnType;
                    if (!cmd->parameters.empty()) {
                        content += L"(";
                        for (size_t i = 0; i < cmd->parameters.size(); i++) {
                            if (i > 0) content += L", ";
                            content += cmd->parameters[i].name + L":" + cmd->parameters[i].type;
                        }
                        content += L")";
                    }
                    content += L"\r\n";
                    if (!cmd->description.empty()) {
                        content += L"    说明: " + cmd->description + L"\r\n";
                    }
                }
                
                content += L"\r\n";
            }
            
            // 转换为UTF-8并写入
            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
            if (utf8Len > 0) {
                std::string utf8Content(utf8Len, '\0');
                WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                WriteFile(hDebug, utf8Content.c_str(), static_cast<DWORD>(utf8Content.length() - 1), &written, NULL);
            }
            
            CloseHandle(hDebug);
        }
    }
    
    return count;
}

bool LibraryConfigManager::SaveConfig() {
    std::wstring configPath = GetConfigFilePath();
    
    // 使用Windows API写入文件
    HANDLE hFile = CreateFileW(configPath.c_str(), GENERIC_WRITE, 0, 
                               NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    // 写入UTF-8 BOM
    BYTE bom[3] = {0xEF, 0xBB, 0xBF};
    DWORD written;
    WriteFile(hFile, bom, 3, &written, NULL);
    
    // 写入配置
    std::wstring content = L"[Libraries]\r\n";
    for (const auto& lib : libraries) {
        content += lib.name + L"=" + (lib.loaded ? L"1" : L"0") + L"|" + lib.filePath + L"\r\n";
    }
    
    // 转换为UTF-8
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
    if (utf8Len > 0) {
        std::string utf8Content(utf8Len, '\0');
        WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
        WriteFile(hFile, utf8Content.c_str(), static_cast<DWORD>(utf8Content.length() - 1), &written, NULL);
    }
    
    CloseHandle(hFile);
    return true;
}

bool LibraryConfigManager::LoadConfig() {
    std::wstring configPath = GetConfigFilePath();
    
    // 使用Windows API读取文件
    HANDLE hFile = CreateFileW(configPath.c_str(), GENERIC_READ, FILE_SHARE_READ, 
                               NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return false;
    }
    
    std::string content(fileSize, '\0');
    DWORD bytesRead = 0;
    if (!ReadFile(hFile, &content[0], fileSize, &bytesRead, NULL)) {
        CloseHandle(hFile);
        return false;
    }
    CloseHandle(hFile);
    
    // 处理BOM
    size_t startPos = 0;
    if (content.size() >= 3 && (unsigned char)content[0] == 0xEF && 
        (unsigned char)content[1] == 0xBB && (unsigned char)content[2] == 0xBF) {
        startPos = 3;
    }
    
    // 转换为宽字符
    int wlen = MultiByteToWideChar(CP_UTF8, 0, content.c_str() + startPos, -1, NULL, 0);
    std::wstring wcontent(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, content.c_str() + startPos, -1, &wcontent[0], wlen);
    
    // 解析配置
    std::wstringstream ss(wcontent);
    std::wstring line;
    bool inLibrariesSection = false;
    
    while (std::getline(ss, line)) {
        // 去除\r
        if (!line.empty() && line.back() == L'\r') {
            line.pop_back();
        }
        
        if (line == L"[Libraries]") {
            inLibrariesSection = true;
            continue;
        }
        
        if (inLibrariesSection && !line.empty() && line[0] != L'[') {
            size_t eqPos = line.find(L"=");
            if (eqPos != std::wstring::npos) {
                std::wstring name = line.substr(0, eqPos);
                std::wstring value = line.substr(eqPos + 1);
                
                size_t pipePos = value.find(L"|");
                if (pipePos != std::wstring::npos) {
                    bool loaded = (value[0] == L'1');
                    std::wstring filePath = value.substr(pipePos + 1);
                    
                    // 更新已扫描的支持库状态
                    for (auto& lib : libraries) {
                        if (lib.name == name) {
                            lib.loaded = loaded;
                            break;
                        }
                    }
                }
            }
        }
    }
    
    return true;
}
