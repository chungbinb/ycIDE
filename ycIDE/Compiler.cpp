#include "Compiler.h"
#include "ProjectManager.h"
#include "SyntaxChecker.h"
#include "Keyword.h"
#include "YiEditor.h"
#include <shlobj.h>
#include <sstream>
#include <fstream>
#include <chrono>
#include <algorithm>

// 宽字符串转UTF-8字符串
static std::string WideToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string result(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &result[0], size, NULL, NULL);
    return result;
}

// 获取编译器单例
Compiler& Compiler::GetInstance() {
    static Compiler instance;
    return instance;
}

Compiler::Compiler() 
    : m_isCompiling(false)
    , m_hRunningProcess(NULL)
    , m_callback(nullptr)
{
}

Compiler::~Compiler() {
    StopExecutable();
}

// 发送编译消息
void Compiler::SendMessage(CompileMessageType type, const std::wstring& message) {
    if (m_callback) {
        CompileMessage msg(type, message);
        m_callback(msg);
    }
}

void Compiler::SendMessage(CompileMessageType type, const std::wstring& message,
                          const std::wstring& file, int line) {
    if (m_callback) {
        CompileMessage msg(type, message, file, line);
        m_callback(msg);
    }
}

// 获取临时目录
std::wstring Compiler::GetTempDirectory() const {
    auto& pm = ProjectManager::GetInstance();
    if (pm.HasOpenProject()) {
        const ProjectInfo* project = pm.GetCurrentProject();
        if (project) {
            return project->projectDirectory + L"\\temp";
        }
    }
    
    // 如果没有项目，使用系统临时目录
    wchar_t tempPath[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath);
    return std::wstring(tempPath) + L"ycide_temp";
}

// 获取输出目录
std::wstring Compiler::GetOutputDirectory() const {
    auto& pm = ProjectManager::GetInstance();
    if (pm.HasOpenProject()) {
        const ProjectInfo* project = pm.GetCurrentProject();
        if (project) {
            return project->projectDirectory + L"\\output";
        }
    }
    
    // 如果没有项目，使用系统临时目录
    wchar_t tempPath[MAX_PATH];
    GetTempPathW(MAX_PATH, tempPath);
    return std::wstring(tempPath) + L"ycide_output";
}

// 获取程序所在目录
std::wstring Compiler::GetAppDirectory() const {
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring exeDir(exePath);
    size_t lastSlash = exeDir.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        exeDir = exeDir.substr(0, lastSlash);
    }
    return exeDir;
}

// 查找C++编译器路径
std::wstring Compiler::FindCppCompiler() {
    // 1. 优先查找程序目录下附带的编译器
    std::wstring appDir = GetAppDirectory();
    
    // 查找程序目录下的 compiler/mingw64/bin/g++.exe
    std::wstring bundledGpp = appDir + L"\\compiler\\mingw64\\bin\\g++.exe";
    if (GetFileAttributesW(bundledGpp.c_str()) != INVALID_FILE_ATTRIBUTES) {
        return bundledGpp;
    }
    
    // 查找程序目录下的 compiler/bin/g++.exe (简化结构)
    bundledGpp = appDir + L"\\compiler\\bin\\g++.exe";
    if (GetFileAttributesW(bundledGpp.c_str()) != INVALID_FILE_ATTRIBUTES) {
        return bundledGpp;
    }
    
    // 查找程序目录下的 mingw64/bin/g++.exe
    bundledGpp = appDir + L"\\mingw64\\bin\\g++.exe";
    if (GetFileAttributesW(bundledGpp.c_str()) != INVALID_FILE_ATTRIBUTES) {
        return bundledGpp;
    }
    
    // 2. 查找 Visual Studio 编译器
    std::vector<std::wstring> vsBasePaths = {
        L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC",
        L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Professional\\VC\\Tools\\MSVC",
        L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Enterprise\\VC\\Tools\\MSVC",
        L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC",
        L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Professional\\VC\\Tools\\MSVC",
        L"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Enterprise\\VC\\Tools\\MSVC"
    };
    
    for (const auto& basePath : vsBasePaths) {
        WIN32_FIND_DATAW findData;
        HANDLE hFind = FindFirstFileW((basePath + L"\\*").c_str(), &findData);
        if (hFind != INVALID_HANDLE_VALUE) {
            std::wstring latestVersion;
            do {
                if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    if (findData.cFileName[0] != '.') {
                        std::wstring version = findData.cFileName;
                        if (version > latestVersion) {
                            latestVersion = version;
                        }
                    }
                }
            } while (FindNextFileW(hFind, &findData));
            FindClose(hFind);
            
            if (!latestVersion.empty()) {
                std::wstring clPath = basePath + L"\\" + latestVersion + L"\\bin\\Hostx64\\x64\\cl.exe";
                if (GetFileAttributesW(clPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
                    return clPath;
                }
                // 尝试32位版本
                clPath = basePath + L"\\" + latestVersion + L"\\bin\\Hostx86\\x86\\cl.exe";
                if (GetFileAttributesW(clPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
                    return clPath;
                }
            }
        }
    }
    
    // 尝试查找 MinGW g++
    std::vector<std::wstring> gppPaths = {
        L"C:\\MinGW\\bin\\g++.exe",
        L"C:\\mingw64\\bin\\g++.exe",
        L"C:\\msys64\\mingw64\\bin\\g++.exe",
        L"C:\\msys64\\ucrt64\\bin\\g++.exe"
    };
    
    for (const auto& path : gppPaths) {
        if (GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
            return path;
        }
    }
    
    // 在 PATH 中查找
    wchar_t pathEnv[32767];
    GetEnvironmentVariableW(L"PATH", pathEnv, 32767);
    std::wstring pathStr = pathEnv;
    
    size_t start = 0;
    size_t end = pathStr.find(L';');
    while (end != std::wstring::npos || start < pathStr.length()) {
        std::wstring dir = pathStr.substr(start, end - start);
        if (!dir.empty()) {
            std::wstring clPath = dir + L"\\cl.exe";
            if (GetFileAttributesW(clPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
                return clPath;
            }
            std::wstring gppPath = dir + L"\\g++.exe";
            if (GetFileAttributesW(gppPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
                return gppPath;
            }
        }
        start = end + 1;
        end = pathStr.find(L';', start);
        if (end == std::wstring::npos && start < pathStr.length()) {
            end = pathStr.length();
        }
    }
    
    return L"";
}

// 查找TCC编译器路径
// target32bit: true 表示查找32位交叉编译器，false 表示查找64位编译器
std::wstring Compiler::FindTccCompiler(bool target32bit) {
    std::wstring appDir = GetAppDirectory();
    
    // 查找程序目录下的 compiler/tcc/
    std::wstring tccDir = appDir + L"\\compiler\\tcc";
    
    if (target32bit) {
        // 32位目标：使用 i386-win32-tcc.exe 交叉编译器
        std::wstring tcc32 = tccDir + L"\\i386-win32-tcc.exe";
        if (GetFileAttributesW(tcc32.c_str()) != INVALID_FILE_ATTRIBUTES) {
            return tcc32;
        }
    } else {
        // 64位目标：使用 tcc.exe
        std::wstring tcc64 = tccDir + L"\\tcc.exe";
        if (GetFileAttributesW(tcc64.c_str()) != INVALID_FILE_ATTRIBUTES) {
            return tcc64;
        }
    }
    
    return L"";
}

// 生成纯C代码 (用于TCC编译器)
bool Compiler::GenerateCCode(const std::wstring& outputDir) {
    auto& pm = ProjectManager::GetInstance();
    if (!pm.HasOpenProject()) {
        SendMessage(CompileMessageType::Error, L"错误: 没有打开的项目");
        return false;
    }
    
    const ProjectInfo* project = pm.GetCurrentProject();
    if (!project) {
        SendMessage(CompileMessageType::Error, L"错误: 无法获取项目信息");
        return false;
    }
    
    // 确保输出目录存在
    CreateDirectoryW(outputDir.c_str(), NULL);
    
    std::wstring mainCPath = outputDir + L"\\main.c";
    
    // 直接以二进制模式创建文件，写入 UTF-8 内容（不带BOM）
    std::ofstream mainFile(mainCPath.c_str(), std::ios::out | std::ios::binary);
    if (!mainFile.is_open()) {
        SendMessage(CompileMessageType::Error, L"错误: 无法创建main.c文件");
        return false;
    }
    
    // 生成纯C代码框架 (TCC兼容)
    mainFile << "/* 由 ycIDE 自动生成 */\n";
    mainFile << "/* 项目名称: " << WideToUtf8(project->projectName) << " */\n";
    mainFile << "\n";
    mainFile << "#include <windows.h>\n";
    mainFile << "#include <stdio.h>\n";
    mainFile << "\n";
    mainFile << "/* 程序入口点 */\n";
    mainFile << "int main(int argc, char* argv[]) {\n";
    mainFile << "    /* 设置控制台编码为UTF-8 */\n";
    mainFile << "    SetConsoleOutputCP(65001);\n";
    mainFile << "    SetConsoleCP(65001);\n";
    mainFile << "    \n";
    mainFile << "    printf(\"程序开始运行...\\n\");\n";
    mainFile << "    printf(\"项目: " << WideToUtf8(project->projectName) << "\\n\");\n";
    mainFile << "    printf(\"\\n\");\n";
    mainFile << "    \n";
    mainFile << "    /* TODO: 在这里添加转换后的程序逻辑 */\n";
    mainFile << "    \n";
    mainFile << "    printf(\"\\n\");\n";
    mainFile << "    printf(\"程序运行结束.\\n\");\n";
    mainFile << "    printf(\"按回车键退出...\");\n";
    mainFile << "    getchar();\n";
    mainFile << "    \n";
    mainFile << "    return 0;\n";
    mainFile << "}\n";
    
    mainFile.close();
    
    return true;
}

// 调用TCC编译器
bool Compiler::InvokeTccCompiler(const std::wstring& cFile, const std::wstring& outputExe,
                                const CompileOptions& options) {
    // 默认编译64位，后续可以添加选项
    bool target32bit = false;
    std::wstring tccPath = FindTccCompiler(target32bit);
    
    if (tccPath.empty()) {
        SendMessage(CompileMessageType::Error, 
            L"错误: 找不到TCC编译器\n"
            L"请确保 compiler\\tcc 目录下有 tcc.exe");
        return false;
    }
    
    // 获取TCC所在目录
    std::wstring tccDir = tccPath;
    size_t lastSlash = tccDir.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        tccDir = tccDir.substr(0, lastSlash);
    }
    
    // 构建命令行
    // TCC 命令格式: tcc -o output.exe input.c -lkernel32 -luser32
    std::wstring cmdLine = L"\"" + tccPath + L"\" -o \"" + outputExe + L"\" \"" + cFile + L"\"";
    
    // 添加库
    cmdLine += L" -lkernel32 -luser32";
    
    // 优化选项（TCC支持有限）
    if (options.optimizeSpeed) {
        // TCC 没有太多优化选项，但可以添加
    }
    
    std::wstring output;
    bool success = CreateCompilerProcess(cmdLine, output, tccDir);
    
    if (!output.empty()) {
        // 解析编译器输出
        std::wistringstream iss(output);
        std::wstring line;
        while (std::getline(iss, line)) {
            if (!line.empty()) {
                std::wstring lowerLine = line;
                std::transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);
                
                if (lowerLine.find(L"error") != std::wstring::npos) {
                    SendMessage(CompileMessageType::Error, line);
                    m_lastResult.errorCount++;
                } else if (lowerLine.find(L"warning") != std::wstring::npos) {
                    SendMessage(CompileMessageType::Warning, line);
                    m_lastResult.warningCount++;
                } else {
                    SendMessage(CompileMessageType::Info, line);
                }
            }
        }
    }
    
    return success;
}

// 生成C++代码 (用于MinGW/MSVC)
bool Compiler::GenerateCppCode(const std::wstring& outputDir) {
    auto& pm = ProjectManager::GetInstance();
    if (!pm.HasOpenProject()) {
        SendMessage(CompileMessageType::Error, L"错误: 没有打开的项目");
        return false;
    }
    
    const ProjectInfo* project = pm.GetCurrentProject();
    if (!project) {
        SendMessage(CompileMessageType::Error, L"错误: 无法获取项目信息");
        return false;
    }
    
    // 确保输出目录存在
    CreateDirectoryW(outputDir.c_str(), NULL);
    
    std::wstring mainCppPath = outputDir + L"\\main.cpp";
    
    // 直接以二进制模式创建文件，写入 UTF-8 内容（不带BOM，g++更兼容）
    std::ofstream mainFileUtf8(mainCppPath.c_str(), std::ios::out | std::ios::binary);
    if (!mainFileUtf8.is_open()) {
        SendMessage(CompileMessageType::Error, L"错误: 无法创建main.cpp文件");
        return false;
    }
    
    // 生成C++代码框架
    mainFileUtf8 << "// 由 ycIDE 自动生成\n";
    mainFileUtf8 << "// 项目名称: " << WideToUtf8(project->projectName) << "\n";
    mainFileUtf8 << "// 生成时间: " << __DATE__ << " " << __TIME__ << "\n";
    mainFileUtf8 << "\n";
    mainFileUtf8 << "#include <windows.h>\n";
    mainFileUtf8 << "#include <iostream>\n";
    mainFileUtf8 << "#include <string>\n";
    mainFileUtf8 << "\n";
    mainFileUtf8 << "// 程序入口点\n";
    mainFileUtf8 << "int main(int argc, char* argv[]) {\n";
    mainFileUtf8 << "    // 设置控制台编码为UTF-8\n";
    mainFileUtf8 << "    SetConsoleOutputCP(65001);\n";
    mainFileUtf8 << "    SetConsoleCP(65001);\n";
    mainFileUtf8 << "    \n";
    mainFileUtf8 << "    std::cout << \"程序开始运行...\" << std::endl;\n";
    mainFileUtf8 << "    std::cout << \"项目: " << WideToUtf8(project->projectName) << "\" << std::endl;\n";
    mainFileUtf8 << "    std::cout << std::endl;\n";
    mainFileUtf8 << "    \n";
    mainFileUtf8 << "    // TODO: 在这里添加转换后的程序逻辑\n";
    mainFileUtf8 << "    \n";
    mainFileUtf8 << "    std::cout << std::endl;\n";
    mainFileUtf8 << "    std::cout << \"程序运行结束.\" << std::endl;\n";
    mainFileUtf8 << "    std::cout << \"按回车键退出...\" << std::endl;\n";
    mainFileUtf8 << "    std::cin.get();\n";
    mainFileUtf8 << "    \n";
    mainFileUtf8 << "    return 0;\n";
    mainFileUtf8 << "}\n";
    
    mainFileUtf8.close();
    
    return true;
}

// 创建进程并捕获输出
bool Compiler::CreateCompilerProcess(const std::wstring& cmdLine, std::wstring& output, 
                                     const std::wstring& workDir) {
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    
    HANDLE hReadPipe, hWritePipe;
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        return false;
    }
    
    // 确保读取端不被子进程继承
    SetHandleInformation(hReadPipe, HANDLE_FLAG_INHERIT, 0);
    
    STARTUPINFOW si = { 0 };
    si.cb = sizeof(si);
    si.hStdError = hWritePipe;
    si.hStdOutput = hWritePipe;
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    
    PROCESS_INFORMATION pi = { 0 };
    
    std::wstring cmdLineCopy = cmdLine;  // CreateProcess需要可修改的字符串
    
    // 设置工作目录
    const wchar_t* workDirPtr = workDir.empty() ? NULL : workDir.c_str();
    
    if (!CreateProcessW(NULL, &cmdLineCopy[0], NULL, NULL, TRUE, 
                       CREATE_NO_WINDOW, NULL, workDirPtr, &si, &pi)) {
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return false;
    }
    
    CloseHandle(hWritePipe);  // 关闭写入端，否则读取会阻塞
    
    // 读取输出
    std::string outputStr;
    char buffer[4096];
    DWORD bytesRead;
    
    while (ReadFile(hReadPipe, buffer, sizeof(buffer) - 1, &bytesRead, NULL) && bytesRead > 0) {
        buffer[bytesRead] = '\0';
        outputStr += buffer;
    }
    
    // 等待进程结束
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hReadPipe);
    
    // 转换为宽字符串
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, outputStr.c_str(), -1, NULL, 0);
    if (wideLen > 0) {
        output.resize(wideLen - 1);
        MultiByteToWideChar(CP_UTF8, 0, outputStr.c_str(), -1, &output[0], wideLen);
    }
    
    return exitCode == 0;
}

// 调用C++编译器
bool Compiler::InvokeCppCompiler(const std::wstring& cppFile, const std::wstring& outputExe,
                                const CompileOptions& options) {
    std::wstring compilerPath = FindCppCompiler();
    
    if (compilerPath.empty()) {
        SendMessage(CompileMessageType::Error, 
            L"错误: 找不到C++编译器\n"
            L"请将MinGW-w64编译器放到程序目录的 compiler\\mingw64 文件夹中\n"
            L"或安装以下编译器之一:\n"
            L"  - Visual Studio 2019/2022\n"
            L"  - MinGW-w64\n"
            L"详细说明请参阅: 编译器配置说明.md");
        return false;
    }
    
    std::wstring cmdLine;
    std::wstring workDir;
    bool isMSVC = (compilerPath.find(L"cl.exe") != std::wstring::npos);
    
    // 获取编译器所在目录
    std::wstring compilerDir = compilerPath;
    size_t lastSlash = compilerDir.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        compilerDir = compilerDir.substr(0, lastSlash);
    }
    
    if (isMSVC) {
        // Visual Studio 编译器
        // 需要先设置环境变量
        std::wstring vcvarsPath;
        size_t pos = compilerPath.find(L"\\VC\\Tools\\MSVC");
        if (pos != std::wstring::npos) {
            vcvarsPath = compilerPath.substr(0, pos) + L"\\VC\\Auxiliary\\Build\\vcvars64.bat";
        }
        
        cmdLine = L"cmd /c \"";
        if (!vcvarsPath.empty() && GetFileAttributesW(vcvarsPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
            cmdLine += L"\"" + vcvarsPath + L"\" x64 && ";
        }
        cmdLine += L"cl.exe /nologo /EHsc /Fe:\"" + outputExe + L"\" \"" + cppFile + L"\"";
        if (options.debug) {
            cmdLine += L" /Zi";
        }
        cmdLine += L" /link user32.lib kernel32.lib";
        cmdLine += L"\"";
    } else {
        // MinGW g++ 编译器
        // 检查是否是附带的编译器（在程序目录下）
        std::wstring appDir = GetAppDirectory();
        bool isBundled = (compilerPath.find(appDir) == 0);
        
        if (isBundled) {
            // 附带的编译器，使用cmd设置PATH环境变量
            cmdLine = L"cmd /c \"set PATH=" + compilerDir + L";%PATH% && ";
            cmdLine += L"\"" + compilerPath + L"\" -o \"" + outputExe + L"\" \"" + cppFile + L"\" -static";
        } else {
            // 系统安装的编译器
            cmdLine = L"\"" + compilerPath + L"\" -o \"" + outputExe + L"\" \"" + cppFile + L"\" -static";
        }
        
        if (options.debug) {
            cmdLine += L" -g";
        }
        if (options.optimizeSpeed) {
            cmdLine += L" -O2";
        } else if (options.optimizeSize) {
            cmdLine += L" -Os";
        }
        cmdLine += L" -luser32 -lkernel32";
        
        if (isBundled) {
            cmdLine += L"\"";  // 关闭cmd /c的引号
        }
        
        workDir = compilerDir;  // 设置工作目录为编译器目录
    }
    
    std::wstring output;
    bool success = CreateCompilerProcess(cmdLine, output, workDir);
    
    if (!output.empty()) {
        // 解析编译器输出，提取错误和警告
        std::wistringstream iss(output);
        std::wstring line;
        while (std::getline(iss, line)) {
            if (!line.empty()) {
                // 简单判断是否是错误或警告
                std::wstring lowerLine = line;
                std::transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);
                
                if (lowerLine.find(L"error") != std::wstring::npos) {
                    SendMessage(CompileMessageType::Error, line);
                    m_lastResult.errorCount++;
                } else if (lowerLine.find(L"warning") != std::wstring::npos) {
                    SendMessage(CompileMessageType::Warning, line);
                    m_lastResult.warningCount++;
                } else {
                    SendMessage(CompileMessageType::Info, line);
                }
            }
        }
    }
    
    return success;
}

// 编译项目
CompileResult Compiler::CompileProject(const CompileOptions& options) {
    m_lastResult = CompileResult();
    m_isCompiling = true;
    
    try {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        auto& pm = ProjectManager::GetInstance();
        if (!pm.HasOpenProject()) {
            SendMessage(CompileMessageType::Error, L"错误: 没有打开的项目，请先创建或打开一个项目");
            m_lastResult.errorCount++;
            m_isCompiling = false;
            return m_lastResult;
        }
    
    const ProjectInfo* project = pm.GetCurrentProject();
    if (!project) {
        SendMessage(CompileMessageType::Error, L"错误: 无法获取项目信息");
        m_lastResult.errorCount++;
        m_isCompiling = false;
        return m_lastResult;
    }
    
    SendMessage(CompileMessageType::Info, L"正在编译项目: " + project->projectName);
    
    // 获取输出目录
    std::wstring tempDir = GetTempDirectory();
    std::wstring outputDir = GetOutputDirectory();
    
    // 确保目录存在
    CreateDirectoryW(tempDir.c_str(), NULL);
    CreateDirectoryW(outputDir.c_str(), NULL);
    
    // 确定输出文件名
    std::wstring outputName = options.outputName;
    if (outputName.empty()) {
        outputName = project->projectName;
    }
    std::wstring outputExe = outputDir + L"\\" + outputName + L".exe";
    
    // 步骤1: 语法检查
    SendMessage(CompileMessageType::Info, L"正在检查语法...");
    if (!CheckProjectSyntax(project)) {
        SendMessage(CompileMessageType::Error, L"编译失败!");
        m_isCompiling = false;
        return m_lastResult;
    }
    
    // 步骤2: 选择编译器并生成代码
    // 优先使用 TCC（体积小、编译快），找不到再用 MinGW/MSVC
    std::wstring tccPath = FindTccCompiler(false);  // 64位
    bool useTcc = !tccPath.empty();
    
    if (useTcc) {
        // 使用 TCC 编译器
        SendMessage(CompileMessageType::Info, L"正在生成代码 (TCC)...");
        if (!GenerateCCode(tempDir)) {
            m_lastResult.errorCount++;
            m_isCompiling = false;
            return m_lastResult;
        }
        
        // 调用 TCC 编译器
        SendMessage(CompileMessageType::Info, L"正在编译 (TCC)...");
        std::wstring mainC = tempDir + L"\\main.c";
        if (!InvokeTccCompiler(mainC, outputExe, options)) {
            SendMessage(CompileMessageType::Error, L"编译失败!");
            m_lastResult.errorCount++;
            m_isCompiling = false;
            return m_lastResult;
        }
    } else {
        // 使用 MinGW/MSVC 编译器
        SendMessage(CompileMessageType::Info, L"正在生成代码 (C++)...");
        if (!GenerateCppCode(tempDir)) {
            m_lastResult.errorCount++;
            m_isCompiling = false;
            return m_lastResult;
        }
        
        // 调用 C++ 编译器
        SendMessage(CompileMessageType::Info, L"正在编译 (C++)...");
        std::wstring mainCpp = tempDir + L"\\main.cpp";
        if (!InvokeCppCompiler(mainCpp, outputExe, options)) {
            SendMessage(CompileMessageType::Error, L"编译失败!");
            m_lastResult.errorCount++;
            m_isCompiling = false;
            return m_lastResult;
        }
    }
    
    // 检查输出文件是否存在
    if (GetFileAttributesW(outputExe.c_str()) == INVALID_FILE_ATTRIBUTES) {
        SendMessage(CompileMessageType::Error, L"编译失败: 输出文件不存在");
        m_lastResult.errorCount++;
        m_isCompiling = false;
        return m_lastResult;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    m_lastResult.elapsedMs = (DWORD)std::chrono::duration_cast<std::chrono::milliseconds>(
        endTime - startTime).count();
    
    m_lastResult.success = true;
    m_lastResult.outputFile = outputExe;
    
    SendMessage(CompileMessageType::Success, 
        L"编译成功 (" + std::to_wstring(m_lastResult.elapsedMs) + L" 毫秒)");
    SendMessage(CompileMessageType::Info, L"输出文件: " + outputExe);
    
    }  // end try
    catch (const std::exception& e) {
        std::string errMsg = e.what();
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, errMsg.c_str(), -1, NULL, 0);
        std::wstring wideErr(wideLen > 0 ? wideLen - 1 : 0, 0);
        if (wideLen > 0) {
            MultiByteToWideChar(CP_UTF8, 0, errMsg.c_str(), -1, &wideErr[0], wideLen);
        }
        SendMessage(CompileMessageType::Error, L"编译过程发生异常: " + wideErr);
        m_lastResult.errorCount++;
    }
    catch (...) {
        SendMessage(CompileMessageType::Error, L"编译过程发生未知异常");
        m_lastResult.errorCount++;
    }
    
    m_isCompiling = false;
    return m_lastResult;
}

// 运行已编译的程序
bool Compiler::RunExecutable(const std::wstring& exePath, HANDLE* hProcess) {
    if (exePath.empty()) {
        SendMessage(CompileMessageType::Error, L"错误: 可执行文件路径为空");
        return false;
    }
    
    if (GetFileAttributesW(exePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        SendMessage(CompileMessageType::Error, L"错误: 可执行文件不存在: " + exePath);
        return false;
    }
    
    // 如果已有程序在运行，先停止
    if (m_hRunningProcess) {
        StopExecutable();
    }
    
    SendMessage(CompileMessageType::Info, L"");
    SendMessage(CompileMessageType::Info, L"==========================================");
    SendMessage(CompileMessageType::Info, L"正在运行程序...");
    SendMessage(CompileMessageType::Info, L"==========================================");
    
    // 获取程序所在目录作为工作目录
    std::wstring workDir = exePath;
    size_t lastSlash = workDir.find_last_of(L"\\/");
    if (lastSlash != std::wstring::npos) {
        workDir = workDir.substr(0, lastSlash);
    }
    
    // 创建进程
    STARTUPINFOW si = { 0 };
    si.cb = sizeof(si);
    
    PROCESS_INFORMATION pi = { 0 };
    
    std::wstring cmdLine = L"\"" + exePath + L"\"";
    
    if (!CreateProcessW(NULL, &cmdLine[0], NULL, NULL, FALSE,
                       CREATE_NEW_CONSOLE, NULL, workDir.c_str(), &si, &pi)) {
        DWORD error = GetLastError();
        SendMessage(CompileMessageType::Error, 
            L"启动程序失败! 错误代码: " + std::to_wstring(error));
        return false;
    }
    
    m_hRunningProcess = pi.hProcess;
    CloseHandle(pi.hThread);
    
    if (hProcess) {
        *hProcess = pi.hProcess;
    }
    
    SendMessage(CompileMessageType::Success, L"程序已启动 (PID: " + 
        std::to_wstring(pi.dwProcessId) + L")");
    
    return true;
}

// 停止正在运行的程序
bool Compiler::StopExecutable() {
    if (!m_hRunningProcess) {
        return true;
    }
    
    // 首先尝试优雅地关闭
    DWORD exitCode;
    if (GetExitCodeProcess(m_hRunningProcess, &exitCode) && exitCode == STILL_ACTIVE) {
        // 强制终止进程
        TerminateProcess(m_hRunningProcess, 1);
        WaitForSingleObject(m_hRunningProcess, 3000);  // 等待最多3秒
        SendMessage(CompileMessageType::Info, L"程序已停止");
    }
    
    CloseHandle(m_hRunningProcess);
    m_hRunningProcess = NULL;
    
    return true;
}

// 读取源文件内容
// 优先从编辑器内存中读取（可以编译未保存的修改），否则从磁盘读取
std::wstring Compiler::ReadSourceFile(const std::wstring& filePath) {
    // 首先尝试从编辑器中获取内容（如果文件已在编辑器中打开）
    if (g_EditorData) {
        for (int i = 0; i < (int)g_EditorData->documents.size(); i++) {
            EditorDocument* doc = g_EditorData->documents[i];
            if (doc && !doc->filePath.empty()) {
                // 比较文件路径（不区分大小写）
                std::wstring docPath = doc->filePath;
                std::wstring checkPath = filePath;
                std::transform(docPath.begin(), docPath.end(), docPath.begin(), ::tolower);
                std::transform(checkPath.begin(), checkPath.end(), checkPath.begin(), ::tolower);
                
                if (docPath == checkPath) {
                    // 找到了，从编辑器中获取内容
                    std::wstring content;
                    for (size_t j = 0; j < doc->lines.size(); j++) {
                        if (j > 0) content += L"\n";
                        content += doc->lines[j];
                    }
                    return content;
                }
            }
        }
    }
    
    // 文件未在编辑器中打开，从磁盘读取
    // 以二进制模式读取文件
    std::ifstream file(filePath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        return L"";
    }
    
    // 读取全部内容
    std::string utf8Content((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
    file.close();
    
    if (utf8Content.empty()) {
        return L"";
    }
    
    // 跳过 UTF-8 BOM
    size_t start = 0;
    if (utf8Content.length() >= 3 && 
        (unsigned char)utf8Content[0] == 0xEF && 
        (unsigned char)utf8Content[1] == 0xBB && 
        (unsigned char)utf8Content[2] == 0xBF) {
        start = 3;
    }
    
    // UTF-8 转宽字符
    const char* data = utf8Content.c_str() + start;
    int dataLen = (int)(utf8Content.length() - start);
    
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, data, dataLen, NULL, 0);
    if (wideLen <= 0) {
        // 如果 UTF-8 转换失败，尝试用本地编码
        wideLen = MultiByteToWideChar(CP_ACP, 0, data, dataLen, NULL, 0);
        if (wideLen <= 0) {
            return L"";
        }
        std::wstring content(wideLen, 0);
        MultiByteToWideChar(CP_ACP, 0, data, dataLen, &content[0], wideLen);
        return content;
    }
    
    std::wstring content(wideLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, data, dataLen, &content[0], wideLen);
    return content;
}

// 检查单个文件语法
bool Compiler::CheckFileSyntax(const std::wstring& filePath) {
    try {
        // 读取文件内容
        std::wstring content = ReadSourceFile(filePath);
        if (content.empty()) {
            SendMessage(CompileMessageType::Warning, L"警告: 文件为空或无法读取: " + filePath);
            m_lastResult.warningCount++;
            return true;  // 空文件不算错误
        }
        
        // 获取文件名
        std::wstring fileName = filePath;
        size_t lastSlash = fileName.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos) {
            fileName = fileName.substr(lastSlash + 1);
        }
        
        // 创建语法检查器
        SyntaxChecker checker;
        checker.SetKeywordManager(&KeywordManager::GetInstance());
        
        // 执行语法检查
        SyntaxCheckResult result = checker.Check(content);
        
        // 输出错误信息 - 易语言风格: 错误(文件:行,列): 消息
        bool hasErrors = false;
        for (const auto& error : result.errors) {
            // 格式: 错误(文件名:行,列): 消息
            std::wstring errorMsg = L"错误(" + fileName + L":" + std::to_wstring(error.line);
            if (error.column > 0) {
                errorMsg += L"," + std::to_wstring(error.column);
            }
            errorMsg += L"): " + error.message;
            
            // 如果有上下文信息，添加到消息后面
            if (!error.context.empty()) {
                // 清理上下文中的换行符
                std::wstring ctx = error.context;
                for (auto& ch : ctx) {
                    if (ch == L'\n' || ch == L'\r') ch = L' ';
                }
                if (!ctx.empty() && ctx != L" ") {
                    errorMsg += L" \"" + ctx + L"\"";
                }
            }
            
            SendMessage(CompileMessageType::Error, errorMsg, filePath, error.line);
            m_lastResult.errorCount++;
            hasErrors = true;
        }
        
        return !hasErrors;
    }
    catch (const std::exception& e) {
        std::string errMsg = e.what();
        int wideLen = MultiByteToWideChar(CP_UTF8, 0, errMsg.c_str(), -1, NULL, 0);
        std::wstring wideErr(wideLen - 1, 0);
        MultiByteToWideChar(CP_UTF8, 0, errMsg.c_str(), -1, &wideErr[0], wideLen);
        SendMessage(CompileMessageType::Error, L"语法检查异常: " + wideErr);
        m_lastResult.errorCount++;
        return false;
    }
    catch (...) {
        SendMessage(CompileMessageType::Error, L"语法检查时发生未知异常: " + filePath);
        m_lastResult.errorCount++;
        return false;
    }
}

// 检查整个项目语法
bool Compiler::CheckProjectSyntax(const ProjectInfo* project) {
    if (!project) return false;
    
    bool allPassed = true;
    int fileCount = 0;
    
    // 目前只对 .eyc 源代码文件进行语法检查
    // 其他文件类型（.ell, .efw, .edt 等）有不同的格式，需要专门的检查器
    std::vector<std::wstring> checkableExtensions = {
        L".eyc"    // 源代码文件
        // 以下文件类型暂不进行语法检查，因为它们有特殊格式
        // L".efw",   // 窗口文件 - 包含控件定义
        // L".ell",   // DLL声明文件 - 包含外部函数声明
        // L".edt",   // 数据类型文件
        // L".egv",   // 全局变量文件
        // L".ecs",   // 常量文件
        // L".ecm",   // 类模块文件
        // L".ecl",   // 程序集文件
        // L".eal"    // 数据类型别名文件
    };
    
    for (const auto& file : project->files) {
        // 获取文件扩展名
        std::wstring ext = file.filePath;
        size_t dotPos = ext.find_last_of(L'.');
        if (dotPos != std::wstring::npos) {
            ext = ext.substr(dotPos);
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        } else {
            continue;  // 没有扩展名，跳过
        }
        
        // 检查是否是需要语法检查的文件类型
        bool shouldCheck = false;
        for (const auto& checkExt : checkableExtensions) {
            if (ext == checkExt) {
                shouldCheck = true;
                break;
            }
        }
        
        if (!shouldCheck) {
            continue;  // 跳过不需要检查的文件（如资源文件）
        }
        
        fileCount++;
        std::wstring fullPath = file.filePath;
        
        // 如果是相对路径，转为绝对路径
        if (fullPath.length() < 2 || fullPath[1] != L':') {
            fullPath = project->projectDirectory + L"\\" + fullPath;
        }
        
        if (!CheckFileSyntax(fullPath)) {
            allPassed = false;
        }
    }
    
    if (fileCount == 0) {
        SendMessage(CompileMessageType::Warning, L"警告: 项目中没有可检查的源文件");
        m_lastResult.warningCount++;
    }
    
    return allPassed;
}
