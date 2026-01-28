#include "ProjectManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

ProjectManager& ProjectManager::GetInstance() {
    static ProjectManager instance;
    return instance;
}

ProjectManager::~ProjectManager() {
    if (currentProject_) {
        delete currentProject_;
    }
}

bool ProjectManager::CreateProject(const std::wstring& projectPath, const std::wstring& projectName,
                                  ProjectOutputType outputType) {
    if (currentProject_) {
        delete currentProject_;
    }
    
    currentProject_ = new ProjectInfo();
    currentProject_->projectName = projectName;
    currentProject_->projectPath = projectPath;
    currentProject_->outputType = outputType;
    
    // 提取项目目录
    size_t lastSlash = projectPath.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        currentProject_->projectDirectory = projectPath.substr(0, lastSlash);
    } else {
        currentProject_->projectDirectory = L".";
    }
    
    // 窗口程序不创建默认的.eyc文件，源代码通过窗口关联创建
    if (outputType != ProjectOutputType::WindowsApp) {
        // 创建默认主文件
        std::wstring mainFilePath = currentProject_->projectDirectory + L"\\" + projectName + L".eyc";
        ProjectFileItem mainFile;
        mainFile.filePath = projectName + L".eyc";
        mainFile.fileName = projectName + L".eyc";
        mainFile.fileType = PROJECT_FILE_EYC;
        mainFile.isMainFile = true;
        currentProject_->files.push_back(mainFile);
        
        // 根据项目类型创建不同的默认内容
        std::wstring defaultContent;
        switch (outputType) {
        case ProjectOutputType::Console:
            defaultContent = 
                L".版本 2\n"
                L".程序集 控制台程序集1\n"
                L"\n"
                L".子程序 _启动子程序, 整数型, , 本子程序在程序启动后最先执行\n"
                L"\n"
                L"    调试输出 (\"程序开始运行...\")\n"
                L"    \n"
                L"    ' TODO: 在这里编写程序逻辑\n"
                L"    \n"
                L"    调试输出 (\"程序运行结束.\")\n"
                L"    返回 (0)\n";
            break;
            
        case ProjectOutputType::DynamicLibrary:
            defaultContent = 
                L".版本 2\n"
                L".程序集 DLL程序集\n"
                L"\n"
                L".子程序 DllMain, 整数型, 公开, DLL入口函数\n"
                L".参数 hModule, 整数型\n"
                L".参数 reason, 整数型\n"
                L".参数 reserved, 整数型\n"
                L"\n"
                L"    返回 (1)  ' 返回真表示初始化成功\n"
                L"\n"
                L".子程序 示例导出函数, 整数型, 公开, 可被外部程序调用\n"
                L".参数 参数1, 整数型\n"
                L"\n"
                L"    ' TODO: 在这里编写函数逻辑\n"
                L"    返回 (参数1 × 2)\n";
            break;
        default:
            break;
        }
        
        // 将内容写入文件
        if (!defaultContent.empty()) {
            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, defaultContent.c_str(), -1, NULL, 0, NULL, NULL);
            if (utf8Len > 0) {
                std::string utf8Content(utf8Len - 1, 0);
                WideCharToMultiByte(CP_UTF8, 0, defaultContent.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                
                HANDLE hFile = CreateFileW(mainFilePath.c_str(), GENERIC_WRITE, 0, NULL,
                                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                if (hFile != INVALID_HANDLE_VALUE) {
                    DWORD bytesWritten;
                    WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
                    CloseHandle(hFile);
                }
            }
        }
    }
    
    return SaveProject();
}

bool ProjectManager::OpenProject(const std::wstring& projectPath) {
    // 读取项目文件
    HANDLE hFile = CreateFileW(projectPath.c_str(), GENERIC_READ, FILE_SHARE_READ,
                               NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    std::string content;
    if (fileSize > 0) {
        content.resize(fileSize);
        DWORD bytesRead;
        ReadFile(hFile, &content[0], fileSize, &bytesRead, NULL);
    }
    CloseHandle(hFile);
    
    // 转换为Unicode
    std::wstring wContent;
    if (!content.empty()) {
        int len = MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, NULL, 0);
        if (len > 0) {
            wContent.resize(len - 1);
            MultiByteToWideChar(CP_UTF8, 0, content.c_str(), -1, &wContent[0], len);
        }
    }
    
    // 解析项目文件
    if (currentProject_) {
        delete currentProject_;
    }
    
    currentProject_ = new ProjectInfo();
    currentProject_->projectPath = projectPath;
    
    // 提取项目目录
    size_t lastSlash = projectPath.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        currentProject_->projectDirectory = projectPath.substr(0, lastSlash);
    } else {
        currentProject_->projectDirectory = L".";
    }
    
    if (!ParseProjectFile(wContent, currentProject_)) {
        delete currentProject_;
        currentProject_ = nullptr;
        return false;
    }
    
    return true;
}

bool ProjectManager::SaveProject() {
    if (!currentProject_) {
        return false;
    }
    
    std::wstring content = GenerateProjectFile(currentProject_);
    
    // 转换为UTF-8
    int len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
    if (len <= 0) return false;
    
    std::string utf8Content(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], len, NULL, NULL);
    
    // 写入文件
    HANDLE hFile = CreateFileW(currentProject_->projectPath.c_str(), GENERIC_WRITE, 0, NULL,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return false;
    
    DWORD bytesWritten;
    bool result = WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
    CloseHandle(hFile);
    
    return result;
}

void ProjectManager::CloseProject() {
    if (currentProject_) {
        delete currentProject_;
        currentProject_ = nullptr;
    }
}

bool ProjectManager::AddFileToProject(const std::wstring& filePath) {
    if (!currentProject_) return false;
    
    // 检查文件是否已存在
    for (const auto& item : currentProject_->files) {
        if (item.filePath == filePath) {
            return false; // 已存在
        }
    }
    
    ProjectFileItem newItem;
    newItem.filePath = GetRelativePath(filePath);
    
    // 提取文件名
    size_t lastSlash = filePath.find_last_of(L"\\/");
    newItem.fileName = (lastSlash != std::wstring::npos) ? filePath.substr(lastSlash + 1) : filePath;
    
    // 判断文件类型
    size_t dotPos = newItem.fileName.find_last_of(L'.');
    if (dotPos != std::wstring::npos) {
        std::wstring ext = newItem.fileName.substr(dotPos);
        std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);
        
        if (ext == L".eyc") {
            newItem.fileType = PROJECT_FILE_EYC;
        } else if (ext == L".ell") {
            newItem.fileType = PROJECT_FILE_ELL;
        } else if (ext == L".ec") {
            newItem.fileType = PROJECT_FILE_EC;
        } else if (ext == L".efw") {
            newItem.fileType = PROJECT_FILE_EFW;
        } else if (ext == L".ecs") {
            newItem.fileType = PROJECT_FILE_ECS;
        } else if (ext == L".edt") {
            newItem.fileType = PROJECT_FILE_EDT;
        } else if (ext == L".egv") {
            newItem.fileType = PROJECT_FILE_EGV;
        } else if (ext == L".png" || ext == L".jpg" || ext == L".jpeg" || 
                   ext == L".bmp" || ext == L".ico" || ext == L".gif") {
            newItem.fileType = PROJECT_FILE_RES_IMAGE;
        } else if (ext == L".wav" || ext == L".mp3") {
            newItem.fileType = PROJECT_FILE_RES_AUDIO;
        } else if (ext == L".txt" || ext == L".dat" || ext == L".bin") {
            newItem.fileType = PROJECT_FILE_RES_DATA;
        } else {
            newItem.fileType = PROJECT_FILE_OTHER;
        }
    }
    
    currentProject_->files.push_back(newItem);
    return SaveProject();
}

bool ProjectManager::RemoveFileFromProject(const std::wstring& filePath) {
    if (!currentProject_) return false;
    
    std::wstring relativePath = GetRelativePath(filePath);
    
    for (auto it = currentProject_->files.begin(); it != currentProject_->files.end(); ++it) {
        if (it->filePath == relativePath) {
            currentProject_->files.erase(it);
            return SaveProject();
        }
    }
    
    return false;
}

const std::vector<ProjectFileItem>& ProjectManager::GetProjectFiles() const {
    static std::vector<ProjectFileItem> emptyVector;
    if (!currentProject_) {
        return emptyVector;
    }
    return currentProject_->files;
}

bool ProjectManager::SetMainFile(const std::wstring& filePath) {
    if (!currentProject_) return false;
    
    std::wstring relativePath = GetRelativePath(filePath);
    bool found = false;
    
    for (auto& item : currentProject_->files) {
        if (item.filePath == relativePath) {
            item.isMainFile = true;
            found = true;
        } else {
            item.isMainFile = false;
        }
    }
    
    if (found) {
        return SaveProject();
    }
    return false;
}

std::wstring ProjectManager::GetMainFile() const {
    if (!currentProject_) return L"";
    
    for (const auto& item : currentProject_->files) {
        if (item.isMainFile) {
            return GetAbsolutePath(item.filePath);
        }
    }
    
    return L"";
}

std::wstring ProjectManager::GetRelativePath(const std::wstring& fullPath) const {
    if (!currentProject_) return fullPath;
    
    std::wstring projDir = currentProject_->projectDirectory;
    
    // 确保目录以反斜杠结尾
    if (!projDir.empty() && projDir.back() != L'\\') {
        projDir += L"\\";
    }
    
    // 检查是否在项目目录下
    if (fullPath.find(projDir) == 0) {
        return fullPath.substr(projDir.length());
    }
    
    return fullPath;
}

std::wstring ProjectManager::GetAbsolutePath(const std::wstring& relativePath) const {
    if (!currentProject_) return relativePath;
    
    // 如果已经是绝对路径
    if (relativePath.length() > 1 && relativePath[1] == L':') {
        return relativePath;
    }
    
    return currentProject_->projectDirectory + L"\\" + relativePath;
}

bool ProjectManager::ParseProjectFile(const std::wstring& content, ProjectInfo* project) {
    std::wstringstream ss(content);
    std::wstring line;
    
    while (std::getline(ss, line)) {
        if (line.empty() || line[0] == L'#') continue; // 跳过空行和注释
        
        // 去除行尾的回车
        if (!line.empty() && line.back() == L'\r') {
            line.pop_back();
        }
        
        // 解析版本号
        if (line.find(L"Version=") == 0) {
            project->version = std::stoi(line.substr(8));
            continue;
        }
        
        // 解析项目名称
        if (line.find(L"ProjectName=") == 0) {
            project->projectName = line.substr(12);
            continue;
        }
        
        // 解析项目输出类型
        if (line.find(L"OutputType=") == 0) {
            std::wstring typeStr = line.substr(11);
            if (typeStr == L"Console") {
                project->outputType = ProjectOutputType::Console;
            } else if (typeStr == L"WindowsApp") {
                project->outputType = ProjectOutputType::WindowsApp;
            } else if (typeStr == L"DynamicLibrary") {
                project->outputType = ProjectOutputType::DynamicLibrary;
            }
            continue;
        }
        
        // 解析文件
        if (line.find(L"File=") == 0) {
            std::wstring fileInfo = line.substr(5);
            
            // 格式: File=类型|路径|是否主文件
            size_t pos1 = fileInfo.find(L'|');
            if (pos1 == std::wstring::npos) continue;
            
            size_t pos2 = fileInfo.find(L'|', pos1 + 1);
            if (pos2 == std::wstring::npos) continue;
            
            ProjectFileItem item;
            std::wstring typeStr = fileInfo.substr(0, pos1);
            item.filePath = fileInfo.substr(pos1 + 1, pos2 - pos1 - 1);
            std::wstring mainStr = fileInfo.substr(pos2 + 1);
            
            // 解析类型
            if (typeStr == L"EYC") item.fileType = PROJECT_FILE_EYC;
            else if (typeStr == L"ELL") item.fileType = PROJECT_FILE_ELL;
            else if (typeStr == L"EC") item.fileType = PROJECT_FILE_EC;
            else if (typeStr == L"EFW") item.fileType = PROJECT_FILE_EFW;
            else if (typeStr == L"ECS") item.fileType = PROJECT_FILE_ECS;
            else if (typeStr == L"EDT") item.fileType = PROJECT_FILE_EDT;
            else if (typeStr == L"EGV") item.fileType = PROJECT_FILE_EGV;
            else if (typeStr == L"RES_IMAGE") item.fileType = PROJECT_FILE_RES_IMAGE;
            else if (typeStr == L"RES_AUDIO") item.fileType = PROJECT_FILE_RES_AUDIO;
            else if (typeStr == L"RES_DATA") item.fileType = PROJECT_FILE_RES_DATA;
            else item.fileType = PROJECT_FILE_OTHER;
            
            // 解析是否主文件
            item.isMainFile = (mainStr == L"1" || mainStr == L"true");
            
            // 提取文件名
            size_t lastSlash = item.filePath.find_last_of(L"\\/");
            item.fileName = (lastSlash != std::wstring::npos) ? item.filePath.substr(lastSlash + 1) : item.filePath;
            
            // 如果类型是OTHER，根据扩展名重新判断（兼容旧项目文件）
            if (item.fileType == PROJECT_FILE_OTHER) {
                size_t dotPos = item.fileName.find_last_of(L'.');
                if (dotPos != std::wstring::npos) {
                    std::wstring ext = item.fileName.substr(dotPos);
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);
                    
                    if (ext == L".efw") item.fileType = PROJECT_FILE_EFW;
                    else if (ext == L".edt") item.fileType = PROJECT_FILE_EDT;
                    else if (ext == L".egv") item.fileType = PROJECT_FILE_EGV;
                    else if (ext == L".ecs") item.fileType = PROJECT_FILE_ECS;
                    else if (ext == L".ec") item.fileType = PROJECT_FILE_EC;
                    else if (ext == L".png" || ext == L".jpg" || ext == L".jpeg" || 
                             ext == L".bmp" || ext == L".ico" || ext == L".gif") {
                        item.fileType = PROJECT_FILE_RES_IMAGE;
                    } else if (ext == L".wav" || ext == L".mp3") {
                        item.fileType = PROJECT_FILE_RES_AUDIO;
                    }
                }
            }
            
            project->files.push_back(item);
        }
    }
    
    return true;
}

std::wstring ProjectManager::GenerateProjectFile(const ProjectInfo* project) {
    std::wstring content;
    
    content += L"# YiCode Project File\n";
    content += L"Version=" + std::to_wstring(project->version) + L"\n";
    content += L"ProjectName=" + project->projectName + L"\n";
    
    // 保存项目输出类型
    content += L"OutputType=";
    switch (project->outputType) {
        case ProjectOutputType::Console: content += L"Console"; break;
        case ProjectOutputType::WindowsApp: content += L"WindowsApp"; break;
        case ProjectOutputType::DynamicLibrary: content += L"DynamicLibrary"; break;
    }
    content += L"\n\n";
    
    for (const auto& item : project->files) {
        content += L"File=";
        
        // 类型
        switch (item.fileType) {
            case PROJECT_FILE_EYC: content += L"EYC"; break;
            case PROJECT_FILE_ELL: content += L"ELL"; break;
            case PROJECT_FILE_EC: content += L"EC"; break;
            case PROJECT_FILE_EFW: content += L"EFW"; break;
            case PROJECT_FILE_ECS: content += L"ECS"; break;
            case PROJECT_FILE_EDT: content += L"EDT"; break;
            case PROJECT_FILE_EGV: content += L"EGV"; break;
            case PROJECT_FILE_RES_IMAGE: content += L"RES_IMAGE"; break;
            case PROJECT_FILE_RES_AUDIO: content += L"RES_AUDIO"; break;
            case PROJECT_FILE_RES_DATA: content += L"RES_DATA"; break;
            default: content += L"OTHER"; break;
        }
        
        content += L"|" + item.filePath + L"|";
        content += item.isMainFile ? L"1" : L"0";
        content += L"\n";
    }
    
    return content;
}

// 显示创建项目对话框
bool ShowCreateProjectDialog(HWND hWnd, std::wstring& projectPath, std::wstring& projectName) {
    OPENFILENAMEW ofn = { 0 };
    WCHAR szFile[260] = { 0 };
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
    ofn.lpstrFilter = L"易语言项目文件 (*.epp)\0*.epp\0所有文件 (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = L"epp";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    ofn.lpstrTitle = L"创建新项目";
    
    if (GetSaveFileNameW(&ofn)) {
        projectPath = szFile;
        
        // 从路径提取项目名称
        size_t lastSlash = projectPath.find_last_of(L"\\/");
        size_t dotPos = projectPath.find_last_of(L'.');
        
        if (lastSlash != std::wstring::npos && dotPos != std::wstring::npos && dotPos > lastSlash) {
            projectName = projectPath.substr(lastSlash + 1, dotPos - lastSlash - 1);
        } else if (lastSlash != std::wstring::npos) {
            projectName = projectPath.substr(lastSlash + 1);
        } else {
            projectName = L"未命名项目";
        }
        
        return true;
    }
    return false;
}

// 显示打开项目对话框
bool ShowOpenProjectDialog(HWND hWnd, std::wstring& projectPath) {
    OPENFILENAMEW ofn = { 0 };
    WCHAR szFile[260] = { 0 };
    
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(WCHAR);
    ofn.lpstrFilter = L"易语言项目文件 (*.epp)\0*.epp\0所有文件 (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrDefExt = L"epp";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrTitle = L"打开项目";
    
    if (GetOpenFileNameW(&ofn)) {
        projectPath = szFile;
        return true;
    }
    return false;
}
