#include "LibraryParser.h"
#include "PinyinHelper.h"
#include <sstream>
#include <algorithm>
#include <Windows.h>
#include <ShlObj.h>

LibraryParser& LibraryParser::GetInstance() {
    static LibraryParser instance;
    return instance;
}

bool LibraryParser::LoadLibraryFile(const std::wstring& filePath) {
    // 使用UTF-8编码读取文件
    // 使用Windows API打开文件
    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 
                               NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    // 获取文件大小
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return false;
    }
    
    // 读取文件内容
    std::string content(fileSize, '\0');
    DWORD bytesRead = 0;
    if (!ReadFile(hFile, &content[0], fileSize, &bytesRead, NULL)) {
        CloseHandle(hFile);
        return false;
    }
    CloseHandle(hFile);
    
    // 使用Windows API转换UTF-8到宽字符
    int wlen = MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, NULL, 0);
    std::wstring wcontent(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, &wcontent[0], wlen);
    
    // 按行分割
    std::wstringstream ss(wcontent);
    std::wstring line;
    
    LibraryCommand currentCmd;
    bool inCommand = false;
    std::wstring currentLibrary;
    std::wstring currentCategory;
    
    while (std::getline(ss, line)) {
        // 去除行尾的\r
        if (!line.empty() && line.back() == L'\r') {
            line.pop_back();
        }
        
        // 去除前导空格
        size_t firstNonSpace = line.find_first_not_of(L" \t");
        if (firstNonSpace != std::wstring::npos) {
            line = line.substr(firstNonSpace);
        }
        
        // 跳过空行
        if (line.empty()) {
            continue;
        }
        
        // 检测支持库名称
        if (line.find(L"支持库名称及版本：") == 0) {
            size_t pos = line.find(L"：");
            if (pos != std::wstring::npos) {
                currentLibrary = line.substr(pos + 1);
                // 去除版本号和空格
                size_t spacePos = currentLibrary.find(L" ");
                if (spacePos != std::wstring::npos) {
                    currentLibrary = currentLibrary.substr(0, spacePos);
                }
            }
        }
        
        // 检测命令类别
        if (line.find(L"--- 命令类别：") == 0) {
            size_t pos = line.find(L"：");
            if (pos != std::wstring::npos) {
                currentCategory = line.substr(pos + 1);
                size_t endPos = currentCategory.find(L" ---");
                if (endPos != std::wstring::npos) {
                    currentCategory = currentCategory.substr(0, endPos);
                }
            }
        }
        
        // 检测调用格式（命令开始）
        if (line.find(L"调用格式：") == 0) {
            // 保存上一个命令
            if (inCommand && !currentCmd.chineseName.empty()) {
                // 生成拼音和首字母
                currentCmd.pinyin = PinyinHelper::GetStringPinyins(currentCmd.chineseName);
                currentCmd.initials = PinyinHelper::GetStringInitialsList(currentCmd.chineseName);

                commands.push_back(currentCmd);
                nameIndex[currentCmd.chineseName] = commands.size() - 1;
                if (!currentCmd.englishName.empty()) {
                    nameIndex[currentCmd.englishName] = commands.size() - 1;
                }
            }
            
            // 开始新命令
            currentCmd = LibraryCommand();
            currentCmd.library = currentLibrary;
            currentCmd.category = currentCategory;
            inCommand = true;
            
            // 解析调用格式：〈返回值类型〉 命令名 （参数...） - 库->类别
            size_t retStart = line.find(L"〈");
            size_t retEnd = line.find(L"〉");
            if (retStart != std::wstring::npos && retEnd != std::wstring::npos) {
                currentCmd.returnType = line.substr(retStart + 1, retEnd - retStart - 1);
            }
            
            // 提取命令名
            size_t nameStart = retEnd + 1;
            while (nameStart < line.length() && line[nameStart] == L' ') {
                nameStart++;
            }
            
            size_t nameEnd = line.find(L"（", nameStart);
            if (nameEnd == std::wstring::npos) {
                nameEnd = line.find(L" -", nameStart);
            }
            if (nameEnd == std::wstring::npos) {
                nameEnd = line.length();
            }
            
            if (nameStart < nameEnd) {
                currentCmd.chineseName = line.substr(nameStart, nameEnd - nameStart);
                // 去除末尾空格
                while (!currentCmd.chineseName.empty() && 
                       currentCmd.chineseName.back() == L' ') {
                    currentCmd.chineseName.pop_back();
                }
            }
        }
        // 检测英文名称
        else if (line.find(L"英文名称：") == 0 && inCommand) {
            size_t pos = line.find(L"：");
            if (pos != std::wstring::npos && pos + 1 < line.length()) {
                currentCmd.englishName = line.substr(pos + 1);
            }
        }
        // 检测命令说明
        else if (line.find(L"本命令") == 0 && inCommand && currentCmd.description.empty()) {
            currentCmd.description = line;
        }
        // 检测参数
        else if (line.find(L"参数<") == 0 && inCommand) {
            LibraryParameter param;
            
            // 提取参数名称 (Unicode 8220/8221 是中文引号 "")
            size_t nameStart = line.find(L'\u201C');  // 左双引号 "
            if (nameStart != std::wstring::npos) {
                nameStart += 1;
                size_t nameEnd = line.find(L'\u201D', nameStart);  // 右双引号 "
                if (nameEnd != std::wstring::npos) {
                    param.name = line.substr(nameStart, nameEnd - nameStart);
                }
            }
            
            // 提取参数类型
            if (nameStart != std::wstring::npos) {
                size_t typeStart = line.find(L'\u201C', nameStart + 1);  // 第二个左引号
                if (typeStart != std::wstring::npos) {
                    typeStart += 1;
                    size_t typeEnd = line.find(L'\u201D', typeStart);  // 第二个右引号
                    if (typeEnd != std::wstring::npos) {
                        param.type = line.substr(typeStart, typeEnd - typeStart);
                        // 去除类型后面的括号说明
                        size_t bracketPos = param.type.find(L"（");
                        if (bracketPos != std::wstring::npos) {
                            param.type = param.type.substr(0, bracketPos);
                        }
                    }
                }
            }
            
            param.optional = false;
            if (!param.name.empty()) {
                currentCmd.parameters.push_back(param);
            }
        }
    }
    
    // 保存最后一个命令
    if (inCommand && !currentCmd.chineseName.empty()) {
        // 生成拼音和首字母
        currentCmd.pinyin = PinyinHelper::GetStringPinyins(currentCmd.chineseName);
        currentCmd.initials = PinyinHelper::GetStringInitialsList(currentCmd.chineseName);

        commands.push_back(currentCmd);
        nameIndex[currentCmd.chineseName] = commands.size() - 1;
        if (!currentCmd.englishName.empty()) {
            nameIndex[currentCmd.englishName] = commands.size() - 1;
        }
    }
    
    return true;
}

const LibraryCommand* LibraryParser::FindCommand(const std::wstring& name) const {
    auto it = nameIndex.find(name);
    if (it != nameIndex.end() && it->second < commands.size()) {
        return &commands[it->second];
    }
    return nullptr;
}

std::vector<std::wstring> LibraryParser::GetCompletions(const std::wstring& input) const {
    std::vector<std::wstring> results;
    
    if (input.empty()) {
        return results;
    }
    
    std::wstring lowerInput = input;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::towlower);
    
    for (const auto& cmd : commands) {
        bool matched = false;
        
        // 匹配中文名
        if (cmd.chineseName.find(input) == 0) {
            matched = true;
        }
        
        // 匹配英文名
        if (!matched && !cmd.englishName.empty()) {
            std::wstring lowerEnglish = cmd.englishName;
            std::transform(lowerEnglish.begin(), lowerEnglish.end(), 
                         lowerEnglish.begin(), ::towlower);
            if (lowerEnglish.find(lowerInput) == 0) {
                matched = true;
            }
        }
        
        if (matched) {
            results.push_back(cmd.chineseName);
        }
    }
    
    return results;
}

// 从 FNE 解析器添加命令
void LibraryParser::AddCommandFromFne(const std::wstring& cmdName, const std::wstring& description,
                                      const std::wstring& returnType, const std::vector<std::wstring>& params,
                                      const std::wstring& libraryName) {
    LibraryCommand cmd;
    cmd.chineseName = cmdName;
    cmd.returnType = returnType;
    cmd.description = description;
    cmd.library = libraryName;
    cmd.category = libraryName;
    
    // 生成拼音和首字母
    cmd.pinyin = PinyinHelper::GetStringPinyins(cmd.chineseName);
    cmd.initials = PinyinHelper::GetStringInitialsList(cmd.chineseName);
    
    // 解析参数
    for (const auto& paramStr : params) {
        LibraryParameter param;
        
        // 参数格式: "参数名 : 参数类型 [可选]"
        size_t colonPos = paramStr.find(L" : ");
        if (colonPos != std::wstring::npos) {
            param.name = paramStr.substr(0, colonPos);
            std::wstring typeAndFlags = paramStr.substr(colonPos + 3);
            
            // 检查是否可选
            param.optional = (typeAndFlags.find(L"[可选]") != std::wstring::npos ||
                            typeAndFlags.find(L"[变量地址]") != std::wstring::npos);
            
            // 提取类型
            size_t bracketPos = typeAndFlags.find(L" [");
            if (bracketPos != std::wstring::npos) {
                param.type = typeAndFlags.substr(0, bracketPos);
            } else {
                param.type = typeAndFlags;
            }
        } else {
            param.name = paramStr;
            param.type = L"";
        }
        
        cmd.parameters.push_back(param);
    }
    
    commands.push_back(cmd);
    nameIndex[cmd.chineseName] = commands.size() - 1;
}

// 加载.ec模块文件
// 模块加载功能已移除
/*
bool LibraryParser::LoadModuleFile(const std::wstring& filePath, const std::wstring& password) {
    ... 已删除 ...
}
*/

