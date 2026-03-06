#include "Compiler.h"
#include "ProjectManager.h"
#include "SyntaxChecker.h"
#include "Keyword.h"
#include "Parser.h"
#include "YiEditor.h"
#include "json.hpp"
#include <shlobj.h>
#include <sstream>
#include <fstream>
#include <chrono>
#include <algorithm>

using json = nlohmann::json;

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

// 查找Clang编译器路径
std::wstring Compiler::FindClangCompiler() {
    std::wstring appDir = GetAppDirectory();
    
    // 查找顺序：
    // 1. 程序目录\compiler\llvm\bin\clang.exe
    // 2. 程序目录\compiler\bin\clang.exe
    // 3. 程序目录\llvm\bin\clang.exe
    
    std::vector<std::wstring> searchPaths = {
        appDir + L"\\compiler\\llvm\\bin\\clang.exe",
        appDir + L"\\compiler\\bin\\clang.exe",
        appDir + L"\\llvm\\bin\\clang.exe",
    };
    
    for (const auto& path : searchPaths) {
        if (GetFileAttributesW(path.c_str()) != INVALID_FILE_ATTRIBUTES) {
            return path;
        }
    }
    
    // 尝试从系统 PATH 查找
    wchar_t pathBuf[MAX_PATH];
    DWORD result = SearchPathW(NULL, L"clang.exe", NULL, MAX_PATH, pathBuf, NULL);
    if (result > 0 && result < MAX_PATH) {
        return std::wstring(pathBuf);
    }
    
    return L"";
}

// 查找附带的 MinGW 根目录（包含头文件和库）
std::wstring Compiler::FindMinGWRoot() {
    std::wstring appDir = GetAppDirectory();
    
    // 查找顺序：
    // 1. 程序目录\compiler\mingw64
    // 2. 程序目录\mingw64
    std::vector<std::wstring> searchPaths = {
        appDir + L"\\compiler\\mingw64",
        appDir + L"\\mingw64",
    };
    
    for (const auto& path : searchPaths) {
        // 验证关键文件存在: x86_64-w64-mingw32\include\windows.h
        std::wstring windowsH = path + L"\\x86_64-w64-mingw32\\include\\windows.h";
        if (GetFileAttributesW(windowsH.c_str()) != INVALID_FILE_ATTRIBUTES) {
            return path;
        }
        // 也检查 include\windows.h (某些精简包的结构)
        windowsH = path + L"\\include\\windows.h";
        if (GetFileAttributesW(windowsH.c_str()) != INVALID_FILE_ATTRIBUTES) {
            return path;
        }
    }
    
    return L"";
}

// 从项目中查找启动窗口文件
static std::wstring FindStartupWindowFile(const ProjectInfo* project) {
    if (!project) return L"";
    
    // 优先查找 _启动窗口.efw
    for (const auto& file : project->files) {
        if (file.fileName == L"_启动窗口.efw" || file.filePath.find(L"_启动窗口.efw") != std::wstring::npos) {
            std::wstring fullPath = file.filePath;
            if (fullPath.length() < 2 || fullPath[1] != L':') {
                fullPath = project->projectDirectory + L"\\" + fullPath;
            }
            return fullPath;
        }
    }
    
    // 查找任意 .efw 文件
    for (const auto& file : project->files) {
        std::wstring ext = file.filePath;
        size_t dotPos = ext.find_last_of(L'.');
        if (dotPos != std::wstring::npos) {
            ext = ext.substr(dotPos);
            std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);
            if (ext == L".efw") {
                std::wstring fullPath = file.filePath;
                if (fullPath.length() < 2 || fullPath[1] != L':') {
                    fullPath = project->projectDirectory + L"\\" + fullPath;
                }
                return fullPath;
            }
        }
    }
    
    return L"";
}

// 控件信息结构
struct WindowControlInfo {
    std::string type;       // 控件类型: Button, Label, Edit, etc.
    std::string name;       // 控件名称
    int x, y, width, height;
    std::string text;       // 标题/文本
};

// 窗口信息结构
struct WindowFileInfo {
    int width = 640;
    int height = 480;
    std::wstring title = L"窗口";
    std::vector<WindowControlInfo> controls;
};

// UTF-8 转宽字符串
static std::wstring Utf8ToWide(const std::string& utf8) {
    if (utf8.empty()) return L"";
    int wideLen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    if (wideLen <= 0) return L"";
    std::wstring result(wideLen - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &result[0], wideLen);
    return result;
}

// 解析窗口文件获取窗口信息和控件
static bool ParseWindowFile(const std::wstring& efwPath, WindowFileInfo& info) {
    // 默认值
    info.width = 640;
    info.height = 480;
    info.title = L"窗口";
    info.controls.clear();
    
    std::ifstream file(efwPath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    file.close();
    
    try {
        json j = json::parse(content);
        
        // 解析窗口尺寸 (兼容两种格式)
        if (j.contains("formWidth")) {
            info.width = j["formWidth"].get<int>();
        } else if (j.contains("width")) {
            info.width = j["width"].get<int>();
        }
        
        if (j.contains("formHeight")) {
            info.height = j["formHeight"].get<int>();
        } else if (j.contains("height")) {
            info.height = j["height"].get<int>();
        }
        
        // 解析窗口标题
        if (j.contains("formTitle")) {
            info.title = Utf8ToWide(j["formTitle"].get<std::string>());
        } else if (j.contains("title")) {
            info.title = Utf8ToWide(j["title"].get<std::string>());
        }
        
        // 解析控件列表
        if (j.contains("controls") && j["controls"].is_array()) {
            for (const auto& ctrlJson : j["controls"]) {
                WindowControlInfo ctrl;
                ctrl.type = ctrlJson.value("type", "");
                ctrl.name = ctrlJson.value("name", "");
                ctrl.x = ctrlJson.value("x", 0);
                ctrl.y = ctrlJson.value("y", 0);
                ctrl.width = ctrlJson.value("width", 80);
                ctrl.height = ctrlJson.value("height", 24);
                
                // 从 properties 中获取标题/文本
                if (ctrlJson.contains("properties") && ctrlJson["properties"].is_object()) {
                    const auto& props = ctrlJson["properties"];
                    if (props.contains("标题")) {
                        ctrl.text = props["标题"].get<std::string>();
                    } else if (props.contains("文本")) {
                        ctrl.text = props["文本"].get<std::string>();
                    } else if (props.contains("title")) {
                        ctrl.text = props["title"].get<std::string>();
                    } else if (props.contains("text")) {
                        ctrl.text = props["text"].get<std::string>();
                    }
                }
                
                info.controls.push_back(ctrl);
            }
        }
        
        return true;
    }
    catch (...) {
        return false;
    }
}

// 获取控件对应的 Win32 类名
static std::string GetWin32ClassName(const std::string& ctrlType) {
    if (ctrlType == "Button" || ctrlType == "按钮") return "BUTTON";
    if (ctrlType == "Label" || ctrlType == "标签") return "STATIC";
    if (ctrlType == "Edit" || ctrlType == "编辑框") return "EDIT";
    if (ctrlType == "TextBox" || ctrlType == "文本框") return "EDIT";
    if (ctrlType == "CheckBox" || ctrlType == "复选框") return "BUTTON";
    if (ctrlType == "RadioButton" || ctrlType == "单选框") return "BUTTON";
    if (ctrlType == "ListBox" || ctrlType == "列表框") return "LISTBOX";
    if (ctrlType == "ComboBox" || ctrlType == "组合框") return "COMBOBOX";
    if (ctrlType == "GroupBox" || ctrlType == "分组框") return "BUTTON";
    return "STATIC";  // 默认为静态文本
}

// 获取控件的窗口样式
static std::string GetWin32Style(const std::string& ctrlType) {
    if (ctrlType == "Button" || ctrlType == "按钮") 
        return "WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON";
    if (ctrlType == "Label" || ctrlType == "标签") 
        return "WS_CHILD | WS_VISIBLE | SS_LEFT";
    if (ctrlType == "Edit" || ctrlType == "编辑框" || ctrlType == "TextBox" || ctrlType == "文本框") 
        return "WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL";
    if (ctrlType == "CheckBox" || ctrlType == "复选框") 
        return "WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX";
    if (ctrlType == "RadioButton" || ctrlType == "单选框") 
        return "WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON";
    if (ctrlType == "ListBox" || ctrlType == "列表框") 
        return "WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY";
    if (ctrlType == "ComboBox" || ctrlType == "组合框") 
        return "WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL";
    if (ctrlType == "GroupBox" || ctrlType == "分组框") 
        return "WS_CHILD | WS_VISIBLE | BS_GROUPBOX";
    return "WS_CHILD | WS_VISIBLE | SS_LEFT";
}

// 生成纯C代码 (用于Clang编译器)
bool Compiler::GenerateCCode(const std::wstring& outputDir, ProjectOutputType outputType) {
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
    
    // 生成纯C代码框架 (Clang兼容)
    mainFile << "/* 由 ycIDE 自动生成 */\n";
    mainFile << "/* 项目名称: " << WideToUtf8(project->projectName) << " */\n";
    mainFile << "\n";
    mainFile << "#include <windows.h>\n";
    mainFile << "#include <stdio.h>\n";
    mainFile << "\n";
    
    // 生成易语言标准库命令的C实现
    mainFile << "/* 易语言标准库命令实现 */\n";
    mainFile << "int 信息框(const wchar_t* 内容, int 按钮类型, const wchar_t* 标题, int 图标类型) {\n";
    mainFile << "    UINT type = MB_OK;\n";
    mainFile << "    if (按钮类型 == 1) type = MB_OKCANCEL;\n";
    mainFile << "    else if (按钮类型 == 2) type = MB_ABORTRETRYIGNORE;\n";
    mainFile << "    else if (按钮类型 == 3) type = MB_YESNOCANCEL;\n";
    mainFile << "    else if (按钮类型 == 4) type = MB_YESNO;\n";
    mainFile << "    else if (按钮类型 == 5) type = MB_RETRYCANCEL;\n";
    mainFile << "    if (图标类型 == 1) type |= MB_ICONERROR;\n";
    mainFile << "    else if (图标类型 == 2) type |= MB_ICONQUESTION;\n";
    mainFile << "    else if (图标类型 == 3) type |= MB_ICONWARNING;\n";
    mainFile << "    else if (图标类型 == 4) type |= MB_ICONINFORMATION;\n";
    mainFile << "    const wchar_t* title = 标题 ? 标题 : L\"提示\";\n";
    mainFile << "    return MessageBoxW(NULL, 内容, title, type);\n";
    mainFile << "}\n";
    mainFile << "\n";
    
    if (outputType == ProjectOutputType::WindowsApp) {
        // 查找并解析窗口文件
        std::wstring efwPath = FindStartupWindowFile(project);
        WindowFileInfo winInfo;
        winInfo.title = project->projectName;
        
        if (!efwPath.empty()) {
            if (ParseWindowFile(efwPath, winInfo)) {
                SendMessage(CompileMessageType::Info, L"使用窗口文件: " + efwPath);
                SendMessage(CompileMessageType::Info, L"窗口尺寸: " + 
                    std::to_wstring(winInfo.width) + L"x" + std::to_wstring(winInfo.height));
                SendMessage(CompileMessageType::Info, L"控件数量: " + 
                    std::to_wstring(winInfo.controls.size()));
            }
        }
        
        // 生成真正的窗口程序代码
        mainFile << "/* 全局变量 */\n";
        mainFile << "static const wchar_t* g_szClassName = L\"ycIDEWindowClass\";\n";
        mainFile << "static const wchar_t* g_szTitle = L\"" << WideToUtf8(winInfo.title) << "\";\n";
        mainFile << "static int g_nWidth = " << winInfo.width << ";\n";
        mainFile << "static int g_nHeight = " << winInfo.height << ";\n";
        mainFile << "static HINSTANCE g_hInstance;\n";
        mainFile << "\n";
        
        // 生成控件ID定义
        if (!winInfo.controls.empty()) {
            mainFile << "/* 控件ID定义 */\n";
            int ctrlId = 1001;
            for (const auto& ctrl : winInfo.controls) {
                std::string idName = "IDC_" + ctrl.name;
                // 转为大写
                for (auto& c : idName) {
                    if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
                }
                mainFile << "#define " << idName << " " << ctrlId++ << "\n";
            }
            mainFile << "\n";
        }
        
        // 生成创建控件的函数
        mainFile << "/* 创建所有控件 */\n";
        mainFile << "void CreateControls(HWND hWndParent) {\n";
        mainFile << "    /* 获取默认GUI字体 */\n";
        mainFile << "    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);\n";
        mainFile << "    HWND hCtrl;\n";
        mainFile << "    \n";
        
        int ctrlId = 1001;
        for (const auto& ctrl : winInfo.controls) {
            std::string className = GetWin32ClassName(ctrl.type);
            std::string style = GetWin32Style(ctrl.type);
            std::string text = ctrl.text.empty() ? ctrl.name : ctrl.text;
            
            mainFile << "    hCtrl = CreateWindowExW(0, L\"" << className << "\", L\"" << text << "\",\n";
            mainFile << "        " << style << ",\n";
            mainFile << "        " << ctrl.x << ", " << ctrl.y << ", " << ctrl.width << ", " << ctrl.height << ",\n";
            mainFile << "        hWndParent, (HMENU)" << ctrlId++ << ", g_hInstance, NULL);\n";
            mainFile << "    SendMessage(hCtrl, WM_SETFONT, (WPARAM)hFont, TRUE);\n";
            mainFile << "    \n";
        }
        
        mainFile << "}\n";
        mainFile << "\n";
        
        // 生成事件处理函数的默认实现 (弱链接，用户代码可覆盖)
        mainFile << "/* 事件处理函数默认实现 (用户源代码中的同名函数会覆盖这些) */\n";
        mainFile << "#define WEAK_FUNC __attribute__((weak))\n";
        for (const auto& ctrl : winInfo.controls) {
            // 按钮类型的控件生成被单击事件
            if (ctrl.type == "Button" || ctrl.type == "按钮") {
                mainFile << "WEAK_FUNC void _" << ctrl.name << "_被单击(void) { }\n";
            }
        }
        // 窗口创建完毕事件
        mainFile << "WEAK_FUNC void __启动窗口_创建完毕(void) { }\n";
        mainFile << "\n";
        
        mainFile << "/* 窗口过程函数 */\n";
        mainFile << "LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {\n";
        mainFile << "    switch (message) {\n";
        mainFile << "    case WM_CREATE:\n";
        mainFile << "        CreateControls(hWnd);\n";
        mainFile << "        __启动窗口_创建完毕();\n";
        mainFile << "        break;\n";
        mainFile << "    case WM_COMMAND:\n";
        mainFile << "        {\n";
        mainFile << "            int wmId = LOWORD(wParam);\n";
        mainFile << "            int wmEvent = HIWORD(wParam);\n";
        mainFile << "            switch (wmId) {\n";
        
        // 为每个按钮生成点击事件处理
        ctrlId = 1001;
        for (const auto& ctrl : winInfo.controls) {
            std::string idName = "IDC_" + ctrl.name;
            for (auto& c : idName) {
                if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
            }
            if (ctrl.type == "Button" || ctrl.type == "按钮") {
                mainFile << "            case " << idName << ":\n";
                mainFile << "                if (wmEvent == BN_CLICKED) {\n";
                mainFile << "                    _" << ctrl.name << "_被单击();\n";
                mainFile << "                }\n";
                mainFile << "                break;\n";
            }
            ctrlId++;
        }
        
        mainFile << "            default:\n";
        mainFile << "                return DefWindowProcW(hWnd, message, wParam, lParam);\n";
        mainFile << "            }\n";
        mainFile << "        }\n";
        mainFile << "        break;\n";
        mainFile << "    case WM_PAINT:\n";
        mainFile << "        {\n";
        mainFile << "            PAINTSTRUCT ps;\n";
        mainFile << "            HDC hdc = BeginPaint(hWnd, &ps);\n";
        mainFile << "            EndPaint(hWnd, &ps);\n";
        mainFile << "        }\n";
        mainFile << "        break;\n";
        mainFile << "    case WM_DESTROY:\n";
        mainFile << "        PostQuitMessage(0);\n";
        mainFile << "        break;\n";
        mainFile << "    default:\n";
        mainFile << "        return DefWindowProcW(hWnd, message, wParam, lParam);\n";
        mainFile << "    }\n";
        mainFile << "    return 0;\n";
        mainFile << "}\n";
        mainFile << "\n";
        mainFile << "/* 窗口程序入口点 */\n";
        mainFile << "int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,\n";
        mainFile << "                   LPSTR lpCmdLine, int nCmdShow) {\n";
        mainFile << "    g_hInstance = hInstance;\n";
        mainFile << "    \n";
        mainFile << "    WNDCLASSEXW wcex;\n";
        mainFile << "    wcex.cbSize = sizeof(WNDCLASSEXW);\n";
        mainFile << "    wcex.style = CS_HREDRAW | CS_VREDRAW;\n";
        mainFile << "    wcex.lpfnWndProc = WndProc;\n";
        mainFile << "    wcex.cbClsExtra = 0;\n";
        mainFile << "    wcex.cbWndExtra = 0;\n";
        mainFile << "    wcex.hInstance = hInstance;\n";
        mainFile << "    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);\n";
        mainFile << "    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);\n";
        mainFile << "    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);\n";
        mainFile << "    wcex.lpszMenuName = NULL;\n";
        mainFile << "    wcex.lpszClassName = g_szClassName;\n";
        mainFile << "    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);\n";
        mainFile << "    \n";
        mainFile << "    if (!RegisterClassExW(&wcex)) {\n";
        mainFile << "        MessageBoxW(NULL, L\"窗口类注册失败!\", L\"错误\", MB_ICONERROR);\n";
        mainFile << "        return 1;\n";
        mainFile << "    }\n";
        mainFile << "    \n";
        mainFile << "    /* 计算窗口位置（居中显示）*/\n";
        mainFile << "    int screenW = GetSystemMetrics(SM_CXSCREEN);\n";
        mainFile << "    int screenH = GetSystemMetrics(SM_CYSCREEN);\n";
        mainFile << "    int posX = (screenW - g_nWidth) / 2;\n";
        mainFile << "    int posY = (screenH - g_nHeight) / 2;\n";
        mainFile << "    \n";
        mainFile << "    HWND hWnd = CreateWindowExW(0, g_szClassName, g_szTitle,\n";
        mainFile << "        WS_OVERLAPPEDWINDOW,\n";
        mainFile << "        posX, posY, g_nWidth, g_nHeight,\n";
        mainFile << "        NULL, NULL, hInstance, NULL);\n";
        mainFile << "    \n";
        mainFile << "    if (!hWnd) {\n";
        mainFile << "        MessageBoxW(NULL, L\"窗口创建失败!\", L\"错误\", MB_ICONERROR);\n";
        mainFile << "        return 1;\n";
        mainFile << "    }\n";
        mainFile << "    \n";
        mainFile << "    ShowWindow(hWnd, nCmdShow);\n";
        mainFile << "    UpdateWindow(hWnd);\n";
        mainFile << "    \n";
        mainFile << "    /* 消息循环 */\n";
        mainFile << "    MSG msg;\n";
        mainFile << "    while (GetMessage(&msg, NULL, 0, 0)) {\n";
        mainFile << "        TranslateMessage(&msg);\n";
        mainFile << "        DispatchMessage(&msg);\n";
        mainFile << "    }\n";
        mainFile << "    \n";
        mainFile << "    return (int)msg.wParam;\n";
        mainFile << "}\n";
    } else {
        // 控制台程序使用 main 入口
        mainFile << "/* 控制台程序入口点 */\n";
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
    }
    
    mainFile.close();
    
    return true;
}

// =========================================================
// C 代码生成器 (AST → C)
// =========================================================

// 数据类型映射：易承语言 → C
static std::string MapTypeToCType(const std::wstring& type) {
    if (type == L"整数型")          return "int";
    if (type == L"长整数型")         return "long long";
    if (type == L"小数型")          return "float";
    if (type == L"双精度小数型")     return "double";
    if (type == L"文本型")          return "wchar_t*";
    if (type == L"逻辑型")          return "int";
    if (type == L"字节型")          return "unsigned char";
    if (type == L"短整数型")         return "short";
    if (type.empty())               return "int";
    return "int";
}

static std::string CIndent(int depth) {
    return std::string(depth * 4, ' ');
}

// 前向声明
static std::string GenExprNode(std::shared_ptr<ASTNode> node);
static std::string GenStmtNode(std::shared_ptr<ASTNode> node, int indent);

static std::string GenExprNode(std::shared_ptr<ASTNode> node) {
    if (!node) return "0";
    switch (node->type) {
        case ASTNodeType::LITERAL_EXPR: {
            auto* lit = static_cast<LiteralExprNode*>(node.get());
            if (lit->literalType == EYTokenType::KEYWORD_TRUE)  return "1";
            if (lit->literalType == EYTokenType::KEYWORD_FALSE) return "0";
            if (lit->literalType == EYTokenType::STRING)
                return "L\"" + WideToUtf8(lit->value) + "\"";
            return WideToUtf8(lit->value);
        }
        case ASTNodeType::IDENTIFIER_EXPR: {
            auto* ident = static_cast<IdentifierExprNode*>(node.get());
            return WideToUtf8(ident->name);
        }
        case ASTNodeType::BINARY_EXPR: {
            auto* bin = static_cast<BinaryExprNode*>(node.get());
            std::string left  = GenExprNode(bin->left);
            std::string right = GenExprNode(bin->right);
            std::string op;
            switch (bin->op) {
                case EYTokenType::OP_PLUS:     op = "+";  break;
                case EYTokenType::OP_MINUS:    op = "-";  break;
                case EYTokenType::OP_MULTIPLY: op = "*";  break;
                case EYTokenType::OP_DIVIDE:   op = "/";  break;
                case EYTokenType::OP_MOD:      op = "%";  break;
                case EYTokenType::OP_EQ:       op = "=="; break;
                case EYTokenType::OP_NEQ:      op = "!="; break;
                case EYTokenType::OP_GT:       op = ">";  break;
                case EYTokenType::OP_LT:       op = "<";  break;
                case EYTokenType::OP_GTE:      op = ">="; break;
                case EYTokenType::OP_LTE:      op = "<="; break;
                case EYTokenType::OP_AND:      op = "&&"; break;
                case EYTokenType::OP_OR:       op = "||"; break;
                default: op = "?"; break;
            }
            return "(" + left + " " + op + " " + right + ")";
        }
        case ASTNodeType::UNARY_EXPR: {
            auto* un = static_cast<UnaryExprNode*>(node.get());
            std::string operand = GenExprNode(un->operand);
            if (un->op == EYTokenType::OP_NOT)   return "!" + operand;
            if (un->op == EYTokenType::OP_MINUS) return "-" + operand;
            return operand;
        }
        case ASTNodeType::CALL_EXPR: {
            auto* call = static_cast<CallExprNode*>(node.get());
            std::string result = WideToUtf8(call->functionName) + "(";
            for (size_t i = 0; i < call->arguments.size(); i++) {
                if (i > 0) result += ", ";
                result += GenExprNode(call->arguments[i]);
            }
            result += ")";
            return result;
        }
        case ASTNodeType::ARRAY_ACCESS: {
            auto* arr = static_cast<ArrayAccessNode*>(node.get());
            std::string result = WideToUtf8(arr->arrayName);
            for (auto& idx : arr->indices)
                result += "[" + GenExprNode(idx) + "]";
            return result;
        }
        default:
            return "0";
    }
}

static std::string GenVarDeclLine(VarDeclNode* var, int indent) {
    std::string ind = CIndent(indent);
    std::string ctype = MapTypeToCType(var->dataType);
    std::string result = ind + ctype + " " + WideToUtf8(var->name);
    if (var->isArray && !var->arrayDimensions.empty()) {
        for (auto& dim : var->arrayDimensions)
            result += "[" + GenExprNode(dim) + "]";
    }
    if (var->initialValue)
        result += " = " + GenExprNode(var->initialValue);
    result += ";\n";
    return result;
}

static std::string GenStmtNode(std::shared_ptr<ASTNode> node, int indent) {
    if (!node) return "";
    std::string ind = CIndent(indent);
    switch (node->type) {
        case ASTNodeType::VAR_DECL:
            return GenVarDeclLine(static_cast<VarDeclNode*>(node.get()), indent);
        case ASTNodeType::ASSIGN_STMT: {
            auto* assign = static_cast<AssignStmtNode*>(node.get());
            return ind + GenExprNode(assign->target) + " = " + GenExprNode(assign->value) + ";\n";
        }
        case ASTNodeType::IF_STMT: {
            auto* ifStmt = static_cast<IfStmtNode*>(node.get());
            std::string result = ind + "if (" + GenExprNode(ifStmt->condition) + ") {\n";
            for (auto& s : ifStmt->thenBlock) result += GenStmtNode(s, indent + 1);
            result += ind + "}";
            if (!ifStmt->elseBlock.empty()) {
                result += " else {\n";
                for (auto& s : ifStmt->elseBlock) result += GenStmtNode(s, indent + 1);
                result += ind + "}";
            }
            result += "\n";
            return result;
        }
        case ASTNodeType::WHILE_STMT: {
            auto* w = static_cast<WhileStmtNode*>(node.get());
            std::string result = ind + "while (" + GenExprNode(w->condition) + ") {\n";
            for (auto& s : w->body) result += GenStmtNode(s, indent + 1);
            result += ind + "}\n";
            return result;
        }
        case ASTNodeType::FOR_STMT: {
            auto* f = static_cast<ForStmtNode*>(node.get());
            std::string varName = WideToUtf8(f->loopVar);
            std::string start   = GenExprNode(f->startValue);
            std::string end     = GenExprNode(f->endValue);
            std::string step    = f->stepValue ? GenExprNode(f->stepValue) : "1";
            std::string result  = ind + "for (int " + varName + " = " + start +
                "; " + varName + " <= " + end + "; " + varName + " += " + step + ") {\n";
            for (auto& s : f->body) result += GenStmtNode(s, indent + 1);
            result += ind + "}\n";
            return result;
        }
        case ASTNodeType::RETURN_STMT: {
            auto* ret = static_cast<ReturnStmtNode*>(node.get());
            if (ret->returnValue)
                return ind + "return " + GenExprNode(ret->returnValue) + ";\n";
            return ind + "return;\n";
        }
        case ASTNodeType::BREAK_STMT:    return ind + "break;\n";
        case ASTNodeType::CONTINUE_STMT: return ind + "continue;\n";
        case ASTNodeType::EXPR_STMT:     return ""; // 占位符
        case ASTNodeType::CALL_EXPR:
            // 函数调用作为语句
            return ind + GenExprNode(node) + ";\n";
        default:                         return "";
    }
}

// 将 .eyc 文本内容转换为 C 代码字符串
std::string Compiler::TranspileEycContent(const std::wstring& eycContent,
                                           const std::wstring& fileName) {
    Parser parser;
    auto ast = parser.Parse(eycContent);

    std::string result;
    result += "/* 由 ycIDE 自动从 " + WideToUtf8(fileName) + " 生成 */\n\n";
    result += "#include <windows.h>\n\n";
    
    // 声明外部库函数
    result += "/* 外部库函数声明 */\n";
    result += "extern int 信息框(const wchar_t* 内容, int 按钮类型, const wchar_t* 标题, int 图标类型);\n";
    result += "\n";

    // 全局变量 / 常量声明
    for (auto& decl : ast->declarations) {
        if (decl->type == ASTNodeType::VAR_DECL) {
            result += GenVarDeclLine(static_cast<VarDeclNode*>(decl.get()), 0);
        } else if (decl->type == ASTNodeType::CONST_DECL) {
            auto* con = static_cast<ConstDeclNode*>(decl.get());
            result += "#define " + WideToUtf8(con->name) + " " +
                (con->value ? GenExprNode(con->value) : "0") + "\n";
        }
    }
    result += "\n";

    // 子程序前向声明
    for (auto& sub : ast->subroutines) {
        if (sub->type != ASTNodeType::SUBROUTINE) continue;
        auto* s = static_cast<SubroutineNode*>(sub.get());
        std::string retType = s->returnType.empty() ? "void" : MapTypeToCType(s->returnType);
        result += retType + " " + WideToUtf8(s->name) + "(";
        for (size_t i = 0; i < s->params.size(); i++) {
            if (i > 0) result += ", ";
            result += MapTypeToCType(s->params[i]->dataType) + " " + WideToUtf8(s->params[i]->name);
        }
        result += ");\n";
    }
    result += "\n";

    // 子程序实现
    for (auto& sub : ast->subroutines) {
        if (sub->type != ASTNodeType::SUBROUTINE) continue;
        auto* s = static_cast<SubroutineNode*>(sub.get());
        std::string retType = s->returnType.empty() ? "void" : MapTypeToCType(s->returnType);
        result += retType + " " + WideToUtf8(s->name) + "(";
        for (size_t i = 0; i < s->params.size(); i++) {
            if (i > 0) result += ", ";
            result += MapTypeToCType(s->params[i]->dataType) + " " + WideToUtf8(s->params[i]->name);
        }
        result += ") {\n";
        // 局部变量
        for (auto& lv : s->localVars) {
            if (lv->type == ASTNodeType::VAR_DECL)
                result += GenVarDeclLine(static_cast<VarDeclNode*>(lv.get()), 1);
        }
        // 语句
        for (auto& stmt : s->statements)
            result += GenStmtNode(stmt, 1);
        result += "}\n\n";
    }

    return result;
}

// 将项目中所有 .eyc 文件转换为 C 文件，返回生成的 C 文件路径列表
bool Compiler::TranspileEycFiles(const std::wstring& tempDir, std::vector<std::wstring>& cFiles) {
    auto& pm = ProjectManager::GetInstance();
    const ProjectInfo* project = pm.GetCurrentProject();
    if (!project) return true;  // 没有项目不算错误

    for (const auto& file : project->files) {
        if (file.fileType != PROJECT_FILE_EYC) continue;

        // 转为绝对路径
        std::wstring fullPath = file.filePath;
        if (fullPath.length() < 2 || fullPath[1] != L':') {
            fullPath = project->projectDirectory + L"\\" + fullPath;
        }

        // 读取 .eyc 内容
        std::wstring content = ReadSourceFile(fullPath);
        if (content.empty()) {
            SendMessage(CompileMessageType::Warning, L"跳过空文件: " + file.fileName);
            continue;
        }

        // 文件名（不含扩展名）
        std::wstring baseName = file.fileName;
        size_t dotPos = baseName.find_last_of(L'.');
        if (dotPos != std::wstring::npos) baseName = baseName.substr(0, dotPos);

        SendMessage(CompileMessageType::Info, L"正在转换源文件: " + file.fileName);
        std::string cCode = TranspileEycContent(content, file.fileName);

        // 写入临时 C 文件
        std::wstring cFilePath = tempDir + L"\\" + baseName + L".c";
        std::ofstream cFileOut(cFilePath.c_str(), std::ios::out | std::ios::binary);
        if (!cFileOut.is_open()) {
            SendMessage(CompileMessageType::Error, L"错误: 无法创建临时C文件: " + cFilePath);
            return false;
        }
        cFileOut.write(cCode.c_str(), cCode.length());
        cFileOut.close();

        cFiles.push_back(cFilePath);
        SendMessage(CompileMessageType::Info, L"已生成: " + cFilePath);
    }
    return true;
}

// 调用Clang编译器
bool Compiler::InvokeClangCompiler(const std::wstring& cFile, const std::wstring& outputExe,
                                const CompileOptions& options, ProjectOutputType outputType,
                                const std::vector<std::wstring>& additionalCFiles) {
    std::wstring clangPath = FindClangCompiler();
    
    if (clangPath.empty()) {
        SendMessage(CompileMessageType::Error, 
            L"错误: 找不到Clang编译器\n"
            L"请确保 compiler\\llvm\\bin 目录下有 clang.exe");
        return false;
    }
    
    // 获取Clang所在目录
    std::wstring clangDir = clangPath;
    size_t lastSlash = clangDir.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        clangDir = clangDir.substr(0, lastSlash);
    }
    
    // 构建命令行
    // Clang 命令格式: clang -o output.exe main.c file1.c file2.c -lkernel32 -luser32
    std::wstring cmdLine = L"\"" + clangPath + L"\" -o \"" + outputExe + L"\" \"" + cFile + L"\"";
    
    // 追加额外的 .eyc 转换后的 C 文件
    for (const auto& extraFile : additionalCFiles) {
        cmdLine += L" \"" + extraFile + L"\"";
    }
    
    // 根据项目类型添加子系统选项
    if (outputType == ProjectOutputType::WindowsApp) {
        // 窗口程序：使用 Windows 子系统
        cmdLine += L" -Wl,-subsystem,windows";
        SendMessage(CompileMessageType::Info, L"项目类型: Windows窗口程序");
    } else if (outputType == ProjectOutputType::DynamicLibrary) {
        // DLL：添加共享库选项
        cmdLine += L" -shared";
        SendMessage(CompileMessageType::Info, L"项目类型: 动态链接库(DLL)");
    } else {
        // 控制台程序：默认子系统
        SendMessage(CompileMessageType::Info, L"项目类型: 控制台程序");
    }
    
    // 添加库
    cmdLine += L" -lkernel32 -luser32 -lgdi32";
    
    // 使用 MinGW 目标（不依赖 Visual Studio）
    cmdLine += L" --target=x86_64-w64-mingw32";
    
    // 查找附带的 MinGW 头文件/库
    std::wstring mingwRoot = FindMinGWRoot();
    if (!mingwRoot.empty()) {
        // 设置 sysroot 让 Clang 自动找到 MinGW 的头文件和库
        cmdLine += L" --sysroot=\"" + mingwRoot + L"\"";
        SendMessage(CompileMessageType::Info, L"MinGW: " + mingwRoot);
    } else {
        SendMessage(CompileMessageType::Error, 
            L"错误: 找不到 MinGW 运行时\n"
            L"请确保 compiler\\mingw64 目录下有头文件和库文件");
        return false;
    }
    
    // 优化选项
    if (options.optimizeSpeed) {
        cmdLine += L" -O2";
    } else if (options.optimizeSize) {
        cmdLine += L" -Os";
    }
    
    // 调试选项
    if (options.debug) {
        cmdLine += L" -g";
    }
    
    // 减少杀毒软件误报：
    // -fno-ident 不写入编译器标识
    // -ffunction-sections -fdata-sections + -Wl,--gc-sections 移除未使用的节，减少PE节数量
    // -Wl,--strip-all 链接时剥离所有符号
    if (!options.debug) {
        cmdLine += L" -fno-ident -ffunction-sections -fdata-sections";
        cmdLine += L" -Wl,--gc-sections,--strip-all";
    }
    
    std::wstring output;
    bool success = CreateCompilerProcess(cmdLine, output, clangDir);
    
    // 编译成功后，使用 strip 移除 .comment 节（包含 Clang 嵌入的 llvm-project URL）
    if (success && !options.debug) {
        std::wstring mingwRoot = FindMinGWRoot();
        std::wstring stripPath = mingwRoot + L"\\bin\\strip.exe";
        if (GetFileAttributesW(stripPath.c_str()) != INVALID_FILE_ATTRIBUTES) {
            std::wstring stripCmd = L"\"" + stripPath + L"\" --strip-all"
                L" --remove-section=.comment"
                L" --remove-section=.note"
                L" --remove-section=.note.GNU-stack"
                L" \"" + outputExe + L"\"";
            std::wstring stripOutput;
            CreateCompilerProcess(stripCmd, stripOutput, clangDir);
        }
    }
    
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
    
    // 步骤2: 将 .eyc 源文件转换为 C 代码
    std::vector<std::wstring> eycCFiles;
    SendMessage(CompileMessageType::Info, L"正在转换 .eyc 源文件...");
    if (!TranspileEycFiles(tempDir, eycCFiles)) {
        m_lastResult.errorCount++;
        m_isCompiling = false;
        return m_lastResult;
    }
    if (!eycCFiles.empty()) {
        SendMessage(CompileMessageType::Info,
            L"已转换 " + std::to_wstring(eycCFiles.size()) + L" 个源文件");
    }

    // 步骤3: 生成代码并调用Clang编译器
    std::wstring clangPath = FindClangCompiler();
    
    if (clangPath.empty()) {
        SendMessage(CompileMessageType::Error, 
            L"错误: 找不到Clang编译器\n"
            L"请确保 compiler\\llvm\\bin 目录下有 clang.exe");
        m_lastResult.errorCount++;
        m_isCompiling = false;
        return m_lastResult;
    }
    
    // 生成C代码框架 (入口点 WinMain/main)
    SendMessage(CompileMessageType::Info, L"正在生成入口框架代码...");
    if (!GenerateCCode(tempDir, project->outputType)) {
        m_lastResult.errorCount++;
        m_isCompiling = false;
        return m_lastResult;
    }
    
    // 调用 Clang 编译器（含所有转换后的 .eyc C 文件）
    SendMessage(CompileMessageType::Info, L"正在编译...");
    std::wstring mainC = tempDir + L"\\main.c";
    if (!InvokeClangCompiler(mainC, outputExe, options, project->outputType, eycCFiles)) {
        SendMessage(CompileMessageType::Error, L"编译失败!");
        m_lastResult.errorCount++;
        m_isCompiling = false;
        return m_lastResult;
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

// 规范化路径（统一分隔符并转小写）
static std::wstring NormalizePath(const std::wstring& path) {
    std::wstring normalized = path;
    // 统一路径分隔符为反斜杠
    for (auto& c : normalized) {
        if (c == L'/') c = L'\\';
    }
    // 转小写
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);
    return normalized;
}

// 读取源文件内容
// 优先从编辑器内存中读取（可以编译未保存的修改），否则从磁盘读取
std::wstring Compiler::ReadSourceFile(const std::wstring& filePath) {
    // 首先尝试从编辑器中获取内容（如果文件已在编辑器中打开）
    if (g_EditorData) {
        std::wstring checkPath = NormalizePath(filePath);
        
        for (int i = 0; i < (int)g_EditorData->documents.size(); i++) {
            EditorDocument* doc = g_EditorData->documents[i];
            if (doc && !doc->filePath.empty()) {
                std::wstring docPath = NormalizePath(doc->filePath);
                
                if (docPath == checkPath) {
                    // 找到了，从编辑器中获取内容
                    // 注意：编辑器内部存储的是内部格式，需要转换回EPL格式
                    extern std::wstring ConvertInternalToEPL(const std::vector<std::wstring>& lines);
                    return ConvertInternalToEPL(doc->lines);
                }
            }
        }
    }
    
    // 文件未在编辑器中打开，从磁盘读取（使用共享读取模式）
    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, 
                               FILE_SHARE_READ | FILE_SHARE_WRITE,  // 允许其他进程读写
                               NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return L"";
    }
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == 0 || fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        return L"";
    }
    
    std::string utf8Content;
    utf8Content.resize(fileSize);
    DWORD bytesRead = 0;
    if (!ReadFile(hFile, &utf8Content[0], fileSize, &bytesRead, NULL)) {
        CloseHandle(hFile);
        return L"";
    }
    CloseHandle(hFile);
    utf8Content.resize(bytesRead);
    
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
