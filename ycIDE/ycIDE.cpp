#include "framework.h"
#include "ycIDE.h"
#include "AIModel.h"
#include "AIChat.h"
#include "Theme.h"
#include "Utils.h"
#include "YiEditor.h"
#include "EllEditor.h"
#include "TabBar.h"
#include "WelcomePage.h"
#include "ResourceExplorer.h"
#include "Keyword.h"
#include "LibraryConfig.h"
#include "ProjectManager.h"
#include "NewProjectDialog.h"
#include "VisualDesigner.h"
#include "PropertyGrid.h"
#include "ControlToolbox.h"
#include "ControlRenderer.h"
#include "OutputPanel.h"
#include "Compiler.h"
#include <vector>
#include <string>
#include <fstream>
#include <commdlg.h>
#include <gdiplus.h>
#include <shlobj.h>
#include <shellapi.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <commctrl.h>
#include <thread>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "comctl32.lib")

using namespace Gdiplus;

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
WCHAR szYiEditorClass[] = L"YiEditor";          // YiEditor 类名
const wchar_t szAIChatClass[] = L"AIChatClass";      // AI聊天窗口类

// 模型管理对话框控件ID
#define IDC_MODEL_LIST      3001
#define IDC_MODEL_NAME      3002
#define IDC_MODEL_PROVIDER  3003
#define IDC_MODEL_URL       3004
#define IDC_MODEL_KEY       3005
#define IDC_BTN_SAVE        3006
#define IDC_LABEL_NAME      3007
#define IDC_LABEL_PROVIDER  3008
#define IDC_LABEL_URL       3009
#define IDC_LABEL_KEY       3010
#define IDC_BTN_ADD         3011
#define IDC_BTN_DELETE      3012

// 自定义消息：面板边框拖动时的布局更新（不触发标题栏/工具栏/活动栏重绘）
#define WM_UPDATE_PANEL_LAYOUT (WM_USER + 300)

// 支持库配置对话框控件ID
#define IDC_LIBRARY_LIST    4001
#define IDC_LIBRARY_DESC    4002
#define IDC_BTN_SELECT_ALL  4003
#define IDC_BTN_DESELECT_ALL 4004
#define IDC_BTN_LOAD_LIBS   4005
#define IDC_BTN_REFRESH     4006

// 子窗口句柄
HWND hMainWnd = NULL;  // 主窗口句柄
HWND hAIChatWnd;
HWND hTabBarWnd;       // 标签栏窗口
HWND hEditorWnd;       // YiEditor窗口
HWND hEllEditorWnd;    // 表格编辑器窗口（.ell/.ecl/.eal/.edl/.edt）
HWND hVisualDesignerWnd = NULL;  // 可视化设计器窗口
HWND hPropertyGridWnd = NULL;    // 属性窗口
HWND hToolboxWnd = NULL;         // 组件箱窗口（浮动）
HWND hWelcomePageWnd;  // 欢迎页窗口
HWND hRightPanelWnd;
HWND hOutputWnd;
HWND hStatusBar = NULL;  // 状态栏

// 主窗口上次位置（用于计算组件箱跟随移动）
RECT g_LastMainWindowRect = {0};

// 可视化设计器相关
bool g_IsVisualDesignerActive = false;  // 当前是否处于可视化设计模式
bool g_IsToolboxDocked = false;         // 组件箱是否停靠到设计器内
static const int DOCKED_TOOLBOX_WIDTH = 180;  // 停靠时组件箱宽度
VisualDesigner* g_pVisualDesigner = nullptr;  // 可视化设计器实例
PropertyGrid* g_pPropertyGrid = nullptr;      // 属性窗口实例
ControlToolbox* g_pControlToolbox = nullptr;  // 组件箱实例
ControlRenderer* g_pControlRenderer = nullptr; // 控件渲染器实例
OutputPanel* g_pOutputPanel = nullptr;        // 输出面板实例

// 可视化设计器窗口类名
const wchar_t szVisualDesignerClass[] = L"VisualDesignerClass";
const wchar_t szPropertyGridClass[] = L"PropertyGridClass";
const wchar_t szToolboxClass[] = L"ToolboxClass";
const wchar_t szOutputPanelClass[] = L"OutputPanelClass";

// 标签栏高度
const int g_TabBarHeight = 35;

// 活动栏相关 (VS Code风格)
const int g_ActivityBarWidth = 48;  // 活动栏宽度

// 活动栏按钮ID定义
#define AB_EXPLORER     200  // 资源管理器
#define AB_SEARCH       201  // 搜索
#define AB_SCM          202  // 源代码管理
#define AB_DEBUG        203  // 运行和调试
#define AB_EXTENSIONS   204  // 扩展
#define AB_AI           205  // AI助手
#define AB_ACCOUNT      206  // 账号
#define AB_SETTINGS     207  // 设置

// 活动栏按钮结构
struct ActivityBarButton {
    int id;                 // 按钮ID
    std::wstring tooltip;   // 提示文本
    RECT rect;              // 按钮区域
    bool selected;          // 是否选中
};
std::vector<ActivityBarButton> g_ActivityBarButtons;
std::vector<ActivityBarButton> g_ActivityBarBottomButtons;  // 底部按钮（账号、设置）
int g_HoverActivityButton = -1;  // 悬停的活动栏按钮ID (-1表示无)
int g_ActiveActivityButton = AB_EXPLORER;  // 当前激活的活动栏按钮
bool g_ExplorerPanelVisible = true;  // 资源管理器面板是否可见
bool g_AIPanelVisible = true;  // AI面板是否可见

// 分隔条相关
int g_LeftPanelWidth = 250;  // 左侧面板宽度(资源管理器，默认250)
int g_SavedLeftPanelWidth = 250;  // 保存收起前的宽度
bool g_LeftPanelVisible = true;  // 左侧面板是否可见
int g_RightPanelWidth = 400; // 右侧面板宽度(AI聊天框，默认400)
int g_SavedRightPanelWidth = 400;  // 保存收起前的宽度
const int g_StatusBarHeight = 24;  // 状态栏高度
bool g_IsDraggingLeftSplitter = false;  // 是否正在拖拽左侧分隔条
bool g_IsDraggingRightSplitter = false;  // 是否正在拖拽右侧分隔条
int g_SplitterWidth = 1;  // 分隔条宽度（1像素边框）
int g_MinLeftPanelWidth = 150;  // 左侧面板最小宽度
int g_MinRightPanelWidth = 330;  // AI聊天框最小宽度
bool g_IsHoveringLeftSplitter = false;  // 鼠标是否悬停在左侧分隔条上
bool g_IsHoveringRightSplitter = false;  // 鼠标是否悬停在右侧分隔条上

// 自绘标题栏相关
const int g_TitleBarHeight = 32;  // 标题栏高度
const int g_ToolBarHeight = 32;   // 工具栏高度
const int g_TotalTopHeight = g_TitleBarHeight + g_ToolBarHeight;  // 顶部总高度（标题栏+工具栏）
bool g_IsMaximized = false;
bool g_IsDraggingWindow = false;
POINT g_DragStartPos;
RECT g_DragStartRect;

// 标题栏按钮区域
RECT g_CloseButtonRect = {0};
RECT g_MaxButtonRect = {0};
RECT g_MinButtonRect = {0};
// 标题栏右侧功能图标按钮
RECT g_ToggleAIButtonRect = {0};      // AI助手显示/隐藏
RECT g_SwapPanelsButtonRect = {0};    // 左右面板切换
RECT g_ToggleOutputButtonRect = {0};  // 输出面板显示/隐藏
RECT g_ToggleSidebarButtonRect = {0}; // 侧边栏显示/隐藏
int g_HoverButton = 0;  // 0=无, 1=最小化, 2=最大化, 3=关闭, 4=AI, 5=切换, 6=输出, 7=侧边栏
bool g_IsTrackingMouse = false;  // 是否正在追踪鼠标离开事件
bool g_OutputPanelVisible = false;  // 输出面板是否可见
bool g_PanelsSwapped = false;  // 资源管理器和AI面板是否已交换位置

// 工具栏相关
struct ToolBarButton {
    int id;                 // 按钮ID
    std::wstring tooltip;   // 提示文本
    RECT rect;              // 按钮区域
    bool enabled;           // 是否启用
    int iconIndex;          // 图标在图片中的索引（从左到右）
};
std::vector<ToolBarButton> g_ToolBarButtons;
Image* g_ToolBarImage = nullptr;  // 工具栏图标图片
const int g_ToolBarIconSize = 16;  // 每个图标的大小（图片是1376x16，每个图标16x16）
const int g_ToolBarButtonSize = 24; // 按钮大小（包含边距）
int g_HoverToolBarButton = -1;  // 悬停的工具栏按钮索引

// 工具栏按钮ID定义
#define TB_NEW_FILE     100
#define TB_OPEN_FILE    101
#define TB_SAVE_FILE    102
#define TB_SAVE_ALL     103
#define TB_UNDO         104
#define TB_REDO         105
#define TB_CUT          106
#define TB_COPY         107
#define TB_PASTE        108
#define TB_RUN          109
#define TB_DEBUG        110
#define TB_STOP         111
#define TB_BUILD        112

// 菜单项
struct MenuItem {
    std::wstring text;
    RECT rect;
    int id;
    bool hovered;
    bool enabled = true;  // 菜单项是否启用
    std::vector<MenuItem> subItems;
};
std::vector<MenuItem> g_MenuItems;
int g_ActiveMenu = -1;  // 当前激活的菜单
HWND g_MenuPopupWnd = NULL;  // 菜单弹出窗口

// 动态主题列表
std::vector<ThemeInfo> g_ThemeList;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                RegisterAIChatClass(HINSTANCE hInstance);
ATOM                RegisterMenuPopupClass(HINSTANCE hInstance);
ATOM                RegisterVisualDesignerClass(HINSTANCE hInstance);
ATOM                RegisterPropertyGridClass(HINSTANCE hInstance);
ATOM                RegisterToolboxClass(HINSTANCE hInstance);
ATOM                RegisterOutputPanelClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    AIChatWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    MenuPopupWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    VisualDesignerWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    PropertyGridWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ToolboxWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    OutputPanelWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    OutputWindowSubclassProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK    StatusBarSubclassProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK    LibraryConfigWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ModelSettingsDlg(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ThemeConfigDlg(HWND, UINT, WPARAM, LPARAM);
void                UpdateMenuItems();  // 更新菜单项（根据项目状态）

// 可视化设计器辅助函数
void SwitchToVisualDesignerMode(bool enable);
void UpdatePropertyGridForSelection();

// 窗口位置配置保存和加载
std::wstring GetWindowConfigPath() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);
    std::wstring exeDir(exePath);
    size_t lastSlash = exeDir.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        exeDir = exeDir.substr(0, lastSlash);
    }
    return exeDir + L"\\window_config.ini";
}

void SaveToolboxPosition() {
    if (!hToolboxWnd) return;
    
    // 组件箱是可视化设计器的子窗口，保存相对于父窗口的坐标
    RECT rc;
    GetWindowRect(hToolboxWnd, &rc);
    
    // 获取窗口大小
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;
    
    // 转换为父窗口坐标
    HWND hParent = GetParent(hToolboxWnd);
    int x = rc.left;
    int y = rc.top;
    if (hParent) {
        POINT pos = { rc.left, rc.top };
        ScreenToClient(hParent, &pos);
        x = pos.x;
        y = pos.y;
    }
    
    std::wstring configPath = GetWindowConfigPath();
    WritePrivateProfileStringW(L"Toolbox", L"X", std::to_wstring(x).c_str(), configPath.c_str());
    WritePrivateProfileStringW(L"Toolbox", L"Y", std::to_wstring(y).c_str(), configPath.c_str());
    WritePrivateProfileStringW(L"Toolbox", L"Width", std::to_wstring(width).c_str(), configPath.c_str());
    WritePrivateProfileStringW(L"Toolbox", L"Height", std::to_wstring(height).c_str(), configPath.c_str());
}

void LoadToolboxPosition() {
    if (!hToolboxWnd) return;
    
    std::wstring configPath = GetWindowConfigPath();
    
    int x = GetPrivateProfileIntW(L"Toolbox", L"X", -1, configPath.c_str());
    int y = GetPrivateProfileIntW(L"Toolbox", L"Y", -1, configPath.c_str());
    int width = GetPrivateProfileIntW(L"Toolbox", L"Width", 200, configPath.c_str());
    int height = GetPrivateProfileIntW(L"Toolbox", L"Height", 400, configPath.c_str());
    
    // 组件箱是可视化设计器的子窗口，坐标是相对于父窗口的
    // 获取父窗口（可视化设计器）的大小
    HWND hParent = GetParent(hToolboxWnd);
    RECT parentRect = {0};
    if (hParent) {
        GetClientRect(hParent, &parentRect);
    }
    
    // 如果没有保存的位置，或者父窗口尚未初始化，使用默认位置
    if (x == -1 || y == -1) {
        // 默认放在父窗口右上角
        if (parentRect.right > 0) {
            x = parentRect.right - width - 10;
            y = 10;
        } else {
            x = 50;
            y = 10;
        }
    }
    
    // 确保窗口在父窗口可见范围内（相对于父窗口的坐标）
    if (hParent && parentRect.right > 0 && parentRect.bottom > 0) {
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x + width > parentRect.right) x = parentRect.right - width;
        if (y + height > parentRect.bottom) y = parentRect.bottom - height;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
    }
    
    SetWindowPos(hToolboxWnd, NULL, x, y, width, height, SWP_NOZORDER);
}

// 保存面板布局状态
void SavePanelLayoutState() {
    std::wstring configPath = GetWindowConfigPath();
    WritePrivateProfileStringW(L"PanelLayout", L"PanelsSwapped", g_PanelsSwapped ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileStringW(L"PanelLayout", L"AIPanelVisible", g_AIPanelVisible ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileStringW(L"PanelLayout", L"LeftPanelVisible", g_LeftPanelVisible ? L"1" : L"0", configPath.c_str());
    WritePrivateProfileStringW(L"PanelLayout", L"OutputPanelVisible", g_OutputPanelVisible ? L"1" : L"0", configPath.c_str());
}

// 加载面板布局状态
void LoadPanelLayoutState() {
    std::wstring configPath = GetWindowConfigPath();
    g_PanelsSwapped = GetPrivateProfileIntW(L"PanelLayout", L"PanelsSwapped", 0, configPath.c_str()) != 0;
    g_AIPanelVisible = GetPrivateProfileIntW(L"PanelLayout", L"AIPanelVisible", 1, configPath.c_str()) != 0;
    g_LeftPanelVisible = GetPrivateProfileIntW(L"PanelLayout", L"LeftPanelVisible", 1, configPath.c_str()) != 0;
    g_ExplorerPanelVisible = g_LeftPanelVisible;
    g_OutputPanelVisible = GetPrivateProfileIntW(L"PanelLayout", L"OutputPanelVisible", 0, configPath.c_str()) != 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // 初始化关键词管理器
    KeywordManager::GetInstance().Initialize();

    // 加载支持库配置并加载选中的支持库
    auto& libConfig = LibraryConfigManager::GetInstance();
    libConfig.ScanLibraryFolders();
    libConfig.LoadConfig();
    libConfig.LoadSelectedLibraries();

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_YCIDE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    RegisterYiEditorClass(hInstance);
    RegisterEllEditorClass(hInstance);
    RegisterTabBarClass(hInstance);
    RegisterWelcomePageClass(hInstance);
    RegisterAIChatClass(hInstance);
    RegisterResourceExplorerClass(hInstance);
    RegisterMenuPopupClass(hInstance);
    RegisterVisualDesignerClass(hInstance);
    RegisterPropertyGridClass(hInstance);
    RegisterToolboxClass(hInstance);
    RegisterOutputPanelClass(hInstance);
    
    // 加载模型配置
    LoadModelsConfig();

    SetTheme(false); // 默认浅色

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        GdiplusShutdown(gdiplusToken);
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_YCIDE));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        // 加速键应该发送到主窗口
        if (!TranslateAccelerator(hMainWnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // 清理GDI+
    GdiplusShutdown(gdiplusToken);

    return (int) msg.wParam;
}

// 更新菜单项（根据项目状态）
void UpdateMenuItems() {
    g_MenuItems.clear();
    
    // 文件菜单
    MenuItem fileMenu = {L"文件", {0}, IDM_FILE, false, true, {}};
    fileMenu.subItems = {
        {L"新建项目", {0}, IDM_NEW_PROJECT, false, true, {}},
        {L"打开项目", {0}, IDM_OPEN_PROJECT, false, true, {}},
        {L"关闭项目", {0}, IDM_CLOSE_PROJECT, false, true, {}},
        {L"添加文件到项目", {0}, IDM_ADD_FILE_TO_PROJECT, false, true, {}},
        {L"新建文件", {0}, IDM_NEW, false, true, {}},
        {L"打开文件", {0}, IDM_OPEN, false, true, {}},
        {L"打开文件夹", {0}, IDM_OPEN_FOLDER, false, true, {}},
        {L"关闭文件夹", {0}, IDM_CLOSE_FOLDER, false, true, {}},
        {L"保存", {0}, IDM_SAVE, false, true, {}},
        {L"另存为", {0}, IDM_SAVE_AS, false, true, {}},
        {L"退出", {0}, IDM_EXIT, false, true, {}}
    };
    
    // 插入菜单（仅当项目打开时显示）
    MenuItem insertMenu = {L"插入", {0}, IDM_INSERT, false, true, {}};
    insertMenu.subItems = {
        {L"程序集", {0}, IDM_INSERT_MODULE, false, true, {}},
        {L"DLL命令", {0}, IDM_INSERT_DLL_COMMAND, false, true, {}},
        {L"自定义数据类型", {0}, IDM_INSERT_DATATYPE, false, true, {}},
        {L"全局变量", {0}, IDM_INSERT_GLOBAL_VAR, false, true, {}},
        {L"常量", {0}, IDM_INSERT_CONSTANT, false, true, {}},
        {L"窗口", {0}, IDM_INSERT_WINDOW, false, true, {}},
        {L"类模块", {0}, IDM_INSERT_CLASS_MODULE, false, true, {}}
    };
    
    // 主题菜单 - 动态加载主题文件
    MenuItem themeMenu = {L"主题", {0}, IDM_THEME, false, true, {}};
    g_ThemeList = GetAvailableThemes();
    for (size_t i = 0; i < g_ThemeList.size() && i < 100; ++i) {
        // 在当前选中的主题前面加上 ✓ 标记
        std::wstring displayName = g_ThemeList[i].name;
        if (g_ThemeList[i].filename == g_CurrentThemeFile) {
            displayName = L"✓ " + displayName;
        }
        themeMenu.subItems.push_back({displayName, {0}, IDM_THEME_FIRST + (int)i, false, true, {}});
    }
    // 添加分隔线和自定义主题选项
    if (!g_ThemeList.empty()) {
        themeMenu.subItems.push_back({L"─────────", {0}, 0, false, false, {}});  // 分隔线（禁用状态）
    }
    themeMenu.subItems.push_back({L"打开主题文件夹...", {0}, IDM_THEME_CUSTOM, false, true, {}});
    
    // 视图菜单
    MenuItem viewMenu = {L"视图", {0}, IDM_VIEW, false, true, {}};
    // 组件箱只有在可视化设计器激活时才启用
    MenuItem toolboxItem = {L"组件箱", {0}, IDM_VIEW_TOOLBOX, false, g_IsVisualDesignerActive, {}};
    viewMenu.subItems = { toolboxItem };
    
    // 工具菜单
    MenuItem toolsMenu = {L"工具", {0}, IDM_TOOLS, false, true, {}};
    toolsMenu.subItems = {
        {L"系统配置", {0}, IDM_SYSTEM_CONFIG, false, true, {}},
        {L"支持库配置", {0}, IDM_LIBRARY_CONFIG, false, true, {}}
    };
    
    // 帮助菜单
    MenuItem helpMenu = {L"帮助", {0}, IDM_HELP, false, true, {}};
    helpMenu.subItems = {
        {L"关于", {0}, IDM_ABOUT, false, true, {}}
    };
    
    // 添加菜单项
    g_MenuItems.push_back(fileMenu);
    
    // 检查是否有打开的项目
    auto& pm = ProjectManager::GetInstance();
    if (pm.HasOpenProject()) {
        g_MenuItems.push_back(insertMenu);
    }
    
    g_MenuItems.push_back(themeMenu);
    g_MenuItems.push_back(viewMenu);
    g_MenuItems.push_back(toolsMenu);
    g_MenuItems.push_back(helpMenu);
    
    // 重新计算菜单位置
    int menuX = 50;
    for (size_t i = 0; i < g_MenuItems.size(); i++) {
        g_MenuItems[i].rect = {menuX, 0, menuX + 60, g_TitleBarHeight};
        menuX += 60;
    }
    
    // 刷新标题栏
    extern HWND hMainWnd;
    if (hMainWnd) {
        RECT titleRect = {0, 0, 0, g_TitleBarHeight};
        GetClientRect(hMainWnd, &titleRect);
        titleRect.bottom = g_TitleBarHeight;
        InvalidateRect(hMainWnd, &titleRect, FALSE);
    }
}

// 初始化工具栏
void InitToolBar() {
    g_ToolBarButtons.clear();
    
    // 加载工具栏图标图片
    if (g_ToolBarImage) {
        delete g_ToolBarImage;
        g_ToolBarImage = nullptr;
    }
    
    // 获取exe所在目录
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);
    std::wstring basePath(exePath);
    size_t lastSlash = basePath.find_last_of(L"\\");
    if (lastSlash != std::wstring::npos) {
        basePath = basePath.substr(0, lastSlash);
    }
    
    // 尝试多个可能的路径加载工具栏图片
    std::vector<std::wstring> tryPaths = {
        basePath + L"\\img\\ToolBar.png",
        basePath + L"\\..\\img\\ToolBar.png",
        basePath + L"\\..\\..\\img\\ToolBar.png"
    };
    
    for (const auto& path : tryPaths) {
        g_ToolBarImage = Image::FromFile(path.c_str());
        if (g_ToolBarImage && g_ToolBarImage->GetLastStatus() == Ok) {
            // 调试信息：输出图片尺寸
            OutputDebugStringW((L"[ToolBar] 成功加载图片: " + path + L"\n").c_str());
            OutputDebugStringW((L"[ToolBar] 图片尺寸: " + std::to_wstring(g_ToolBarImage->GetWidth()) + L"x" + std::to_wstring(g_ToolBarImage->GetHeight()) + L"\n").c_str());
            break;
        }
        if (g_ToolBarImage) {
            delete g_ToolBarImage;
            g_ToolBarImage = nullptr;
        }
    }
    
    if (!g_ToolBarImage) {
        OutputDebugStringW(L"[ToolBar] 警告: 无法加载工具栏图片!\n");
    }
    
    // 定义工具栏按钮（按顺序，图标索引从0开始）
    struct ToolBarDef {
        int id;
        const wchar_t* tooltip;
        int iconIndex;
    };
    
    ToolBarDef buttons[] = {
        {TB_NEW_FILE,  L"新建文件", 0},   // 索引0: 新建文件
        {TB_OPEN_FILE, L"打开文件", 2},   // 索引2: 打开文件夹
        {TB_SAVE_FILE, L"保存", 4},       // 索引4: 保存
        {0, nullptr, -1},  // 分隔符
        {TB_CUT,       L"剪切", 6},       // 索引6: 剪切
        {TB_COPY,      L"复制", 8},       // 索引8: 复制
        {TB_PASTE,     L"粘贴", 10},      // 索引10: 粘贴
        {0, nullptr, -1},  // 分隔符
        {TB_REDO,      L"重做", 12},      // 索引12: 向前（重做）
        {TB_UNDO,      L"撤销", 14},      // 索引14: 撤销
        {0, nullptr, -1},  // 分隔符
        {TB_BUILD,     L"搜索", 16},      // 索引16: 搜索文件夹
        {TB_RUN,       L"运行", 18},      // 索引18: 运行
        {TB_STOP,      L"停止", 20},      // 索引20: 停止
        {TB_DEBUG,     L"调试", 22},      // 索引22: 下一个图标
        {100,          L"图标24", 24},    // 索引24
        {101,          L"图标26", 26},    // 索引26
        {102,          L"图标28", 28},    // 索引28
        {103,          L"图标30", 30},    // 索引30
        {104,          L"图标32", 32},    // 索引32
        {105,          L"图标34", 34},    // 索引34
        {106,          L"图标36", 36},    // 索引36
        {107,          L"图标38", 38},    // 索引38
        {108,          L"图标40", 40},    // 索引40
        {109,          L"图标42", 42}     // 索引42
    };
    
    int x = 10;  // 起始X位置
    int y = g_TitleBarHeight + (g_ToolBarHeight - g_ToolBarButtonSize) / 2;  // 垂直居中
    
    for (const auto& btn : buttons) {
        if (btn.id == 0) {
            // 分隔符：增加间距
            x += 8;
            continue;
        }
        
        ToolBarButton tbBtn;
        tbBtn.id = btn.id;
        tbBtn.tooltip = btn.tooltip;
        tbBtn.enabled = true;
        tbBtn.iconIndex = btn.iconIndex;
        tbBtn.rect = {x, y, x + g_ToolBarButtonSize, y + g_ToolBarButtonSize};
        g_ToolBarButtons.push_back(tbBtn);
        
        x += g_ToolBarButtonSize + 2;  // 按钮间隔
    }
}

// 初始化活动栏
void InitActivityBar() {
    g_ActivityBarButtons.clear();
    g_ActivityBarBottomButtons.clear();
    
    // 定义活动栏主按钮（从上到下）
    struct ActivityBarDef {
        int id;
        const wchar_t* tooltip;
    };
    
    ActivityBarDef topButtons[] = {
        {AB_EXPLORER,   L"资源管理器 (Ctrl+Shift+E)"},
        {AB_SEARCH,     L"搜索 (Ctrl+Shift+F)"},
        {AB_SCM,        L"源代码管理 (Ctrl+Shift+G)"},
        {AB_DEBUG,      L"运行和调试 (Ctrl+Shift+D)"},
        {AB_EXTENSIONS, L"扩展 (Ctrl+Shift+X)"}
        // AI助手按钮已移到标题栏
    };
    
    // 底部按钮（从下到上）
    ActivityBarDef bottomButtons[] = {
        {AB_SETTINGS,   L"设置 (Ctrl+,)"},
        {AB_ACCOUNT,    L"账号"}
    };
    
    int btnSize = 48;  // 按钮大小等于活动栏宽度
    int y = g_TotalTopHeight;  // 从标题栏+工具栏下面开始
    
    for (const auto& btn : topButtons) {
        ActivityBarButton abBtn;
        abBtn.id = btn.id;
        abBtn.tooltip = btn.tooltip;
        abBtn.selected = (btn.id == g_ActiveActivityButton);
        abBtn.rect = {0, y, btnSize, y + btnSize};
        g_ActivityBarButtons.push_back(abBtn);
        y += btnSize;
    }
    
    // 底部按钮位置会在 WM_SIZE 中动态计算
    for (const auto& btn : bottomButtons) {
        ActivityBarButton abBtn;
        abBtn.id = btn.id;
        abBtn.tooltip = btn.tooltip;
        abBtn.selected = false;
        abBtn.rect = {0, 0, btnSize, btnSize};  // 位置稍后更新
        g_ActivityBarBottomButtons.push_back(abBtn);
    }
}

// 更新活动栏底部按钮位置
void UpdateActivityBarBottomButtonsPosition(int windowHeight) {
    int btnSize = 48;
    int y = windowHeight - g_StatusBarHeight - btnSize;
    
    // 获取窗口宽度
    RECT rc;
    GetClientRect(hMainWnd, &rc);
    int windowWidth = rc.right;
    
    // 计算活动栏X位置
    int activityBarX = g_PanelsSwapped ? (windowWidth - g_ActivityBarWidth) : 0;
    
    // 从下往上排列底部按钮
    for (int i = (int)g_ActivityBarBottomButtons.size() - 1; i >= 0; i--) {
        g_ActivityBarBottomButtons[i].rect = {activityBarX, y, activityBarX + btnSize, y + btnSize};
        y -= btnSize;
    }
    
    // 同时更新顶部按钮位置
    y = g_TotalTopHeight;
    for (auto& btn : g_ActivityBarButtons) {
        btn.rect = {activityBarX, y, activityBarX + btnSize, y + btnSize};
        y += btnSize;
    }
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YCIDE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;  // 不使用系统菜单，改为自绘
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   // 使用WS_POPUP样式去除系统标题栏，但保留边框和调整大小功能
   int screenWidth = GetSystemMetrics(SM_CXSCREEN);
   int screenHeight = GetSystemMetrics(SM_CYSCREEN);
   int windowWidth = 1400;
   int windowHeight = 900;
   int x = (screenWidth - windowWidth) / 2;
   int y = (screenHeight - windowHeight) / 2;
   
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, 
      WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CLIPCHILDREN,
      x, y, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        {
            // 保存主窗口句柄
            hMainWnd = hWnd;
            
            // 初始化菜单项
            UpdateMenuItems();
            
            // 初始化工具栏
            InitToolBar();
            
            // 初始化活动栏
            InitActivityBar();
            
            // 加载主题配置
            LoadThemeConfig();
            
            // 1. 左侧: 资源管理器 (自定义类) - 现在在活动栏右边
            hRightPanelWnd = CreateWindowW(L"ResourceExplorer", nullptr,
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hWnd, (HMENU)1003, hInst, nullptr);

            // 2. 中间区域：标签栏 + 编辑器
            // 2.1 标签栏
            hTabBarWnd = CreateWindowW(L"TabBarClass", nullptr,
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hWnd, (HMENU)1005, hInst, nullptr);
            
            // 2.2 YiEditor (代码编辑器) - 初始隐藏，打开文件后显示
            hEditorWnd = CreateWindowW(szYiEditorClass, nullptr,
                WS_CHILD,
                0, 0, 0, 0, hWnd, (HMENU)1002, hInst, nullptr);
            
            // 2.3 DllEditor (DLL声明编辑器) - 初始隐藏
            hEllEditorWnd = CreateEllEditorWindow(hInst, hWnd);
            SetWindowLong(hEllEditorWnd, GWL_ID, 1006);
            
            {
                wchar_t debugMsg[256];
                swprintf_s(debugMsg, L"[主窗口] DllEditor窗口句柄: %p\n", hEllEditorWnd);
                OutputDebugStringW(debugMsg);
            }
            
            // 2.4 欢迎页 - 初始显示
            hWelcomePageWnd = CreateWindowW(szWelcomePageClass, nullptr,
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hWnd, (HMENU)1007, hInst, nullptr);
            
            {
                wchar_t debugMsg[256];
                swprintf_s(debugMsg, L"[主窗口] 欢迎页窗口句柄: %p, 是否可见: %d\n", 
                    hWelcomePageWnd, IsWindowVisible(hWelcomePageWnd));
                OutputDebugStringW(debugMsg);
            }
            
            // 2.5 可视化设计器 - 初始隐藏
            hVisualDesignerWnd = CreateWindowW(szVisualDesignerClass, nullptr,
                WS_CHILD | WS_CLIPCHILDREN,  // 添加 WS_CLIPCHILDREN 防止子窗口闪烁
                0, 0, 0, 0, hWnd, (HMENU)1008, hInst, nullptr);
            
            // 2.6 属性窗口 - 初始隐藏，在可视化设计模式下替代AI聊天窗口
            hPropertyGridWnd = CreateWindowW(szPropertyGridClass, nullptr,
                WS_CHILD,
                0, 0, 0, 0, hWnd, (HMENU)1009, hInst, nullptr);
            
            // 2.7 组件箱 - 作为可视化设计器的子窗口，初始隐藏
            // 注意：这里先创建为主窗口的子窗口，在进入设计器模式时会重新设置父窗口
            hToolboxWnd = CreateWindowExW(
                0,
                szToolboxClass, L"组件箱",
                WS_CHILD,  // 作为子窗口
                0, 0, 200, 400,
                hVisualDesignerWnd, nullptr, hInst, nullptr);  // 父窗口是可视化设计器
            
            // 加载组件箱保存的位置
            LoadToolboxPosition();
            
            // 加载面板布局状态
            LoadPanelLayoutState();

            // 3. 右侧: AI 聊天框（使用自定义类）- 现在在最右边
            hAIChatWnd = CreateWindowW(szAIChatClass, nullptr,
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hWnd, (HMENU)1001, hInst, nullptr);
            
            // 4. 底部: 输出面板（带标签切换）
            hOutputWnd = CreateWindowW(szOutputPanelClass, nullptr,
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hWnd, (HMENU)1004, hInst, nullptr);
            
            // 5. 状态栏
            hStatusBar = CreateWindowW(L"STATIC", L"就绪",
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                0, 0, 0, 0, hWnd, (HMENU)1005, hInst, nullptr);
            
            // 子类化状态栏以处理边缘调整大小
            SetWindowSubclass(hStatusBar, StatusBarSubclassProc, 0, 0);
            
            // 设置状态栏字体
            HFONT hStatusFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
            SendMessage(hStatusBar, WM_SETFONT, (WPARAM)hStatusFont, TRUE);
            
            // 为窗口添加阴影效果
            MARGINS margins = {0, 0, 0, 1};
            DwmExtendFrameIntoClientArea(hWnd, &margins);
            
            // 设置定时器检查文件修改状态
            SetTimer(hWnd, 1, 500, NULL);
        }
        break;
    case WM_TIMER:
        if (wParam == 1) {
            // 检查代码编辑器的修改状态（YiEditor 使用文档列表管理，需要定时检查）
            if (g_EditorData) {
                EditorDocument* doc = g_EditorData->GetActiveDoc();
                if (doc && doc->modified) {
                     ExplorerSetFileModified(doc->filePath, true);
                     
                     // 同时标记标签页为已修改
                     TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                     if (tabData && tabData->activeTabIndex >= 0 && tabData->activeTabIndex < (int)tabData->tabs.size()) {
                         if (!tabData->tabs[tabData->activeTabIndex].isModified) {
                             tabData->SetTabModified(tabData->activeTabIndex, true);
                         }
                     }
                }
            }
            // 注意：TableEditor（DllEditor、DataTypeEditor、GlobalVarEditor、ConstantEditor）
            // 现在通过 NotifyModified() 主动发送 WM_COMMAND 消息通知修改状态，不再需要定时轮询
        }
        break;
    case WM_SIZE:
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            
            // 检查窗口是否最大化
            WINDOWPLACEMENT wp = {sizeof(WINDOWPLACEMENT)};
            GetWindowPlacement(hWnd, &wp);
            g_IsMaximized = (wp.showCmd == SW_SHOWMAXIMIZED);

            // 新布局：活动栏 | 资源管理器 | 编辑器区域 | AI聊天 (或交换时：AI | 编辑器 | 资源管理器 | 活动栏)
            int activityBarW = g_ActivityBarWidth;  // 活动栏宽度
            int leftPanelW = g_LeftPanelVisible ? g_LeftPanelWidth : 0;  // 资源管理器宽度
            // 可视化设计器模式下隐藏AI面板
            int rightPanelW = (g_AIPanelVisible && !g_IsVisualDesignerActive) ? g_RightPanelWidth : 0;  // AI聊天框宽度
            int outputH = g_OutputPanelVisible ? 150 : 0;  // 输出窗口高度（根据可见性）
            int topH = g_TotalTopHeight;  // 标题栏+工具栏高度
            int statusH = g_StatusBarHeight;  // 状态栏高度
            
            // 活动栏位置（交换时在右侧）
            int activityBarX = g_PanelsSwapped ? (width - activityBarW) : 0;
            
            // 面板交换时，交换左右面板宽度
            int effectiveLeftW = g_PanelsSwapped ? rightPanelW : leftPanelW;
            int effectiveRightW = g_PanelsSwapped ? leftPanelW : rightPanelW;
            
            // 计算中间编辑器区域宽度
            int centerW = width - activityBarW - effectiveLeftW - effectiveRightW;
            if (centerW < 100) {
                centerW = 100;
            }
            
            // 中间区域可用高度（减去状态栏）
            int centerTotalH = height - topH - statusH;
            // 编辑器高度 = 中间总高度 - 输出窗口高度
            int editorH = centerTotalH - outputH;
            if (editorH < 0) editorH = 0;
            
            // 更新活动栏底部按钮位置
            UpdateActivityBarBottomButtonsPosition(height);

            // 调整各窗口位置（使用 DeferWindowPos 批量更新，减少闪烁）
            HDWP hdwp = BeginDeferWindowPos(12);
            if (hdwp) {
                // 面板交换逻辑
                if (g_PanelsSwapped) {
                    // 交换模式：AI在最左侧，活动栏在最右侧
                    // 布局：AI | 编辑器 | 资源管理器 | 活动栏
                    
                    // 左侧：AI聊天面板（从x=0开始）
                    if (hAIChatWnd) {
                        if (g_AIPanelVisible) {
                            hdwp = DeferWindowPos(hdwp, hAIChatWnd, NULL, 
                                0, topH, effectiveLeftW, centerTotalH, 
                                SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
                        } else {
                            hdwp = DeferWindowPos(hdwp, hAIChatWnd, NULL, 
                                0, topH, 0, centerTotalH, 
                                SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
                        }
                    }
                    // 右侧：资源管理器（在活动栏左边）
                    int rightPanelLeft = width - activityBarW - effectiveRightW;
                    if (hRightPanelWnd) {
                        if (g_LeftPanelVisible) {
                            hdwp = DeferWindowPos(hdwp, hRightPanelWnd, NULL, 
                                rightPanelLeft, topH, effectiveRightW, centerTotalH, 
                                SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
                            
                            // 交换模式下，如果是可视化设计模式且当前是属性标签，将PropertyGrid嵌入资源管理器
                            if (g_IsVisualDesignerActive && g_ExplorerData.activeTab == TAB_PROPERTY && hPropertyGridWnd) {
                                // PropertyGrid 放在资源管理器的标签栏下方
                                int propHeight = centerTotalH - g_ExplorerData.tabBarHeight;
                                int borderMargin = 8;  // 边框拖动区域宽度
                                
                                // 交换模式下边框在左边，PropertyGrid 需要右移留出左边框拖动区域
                                SetParent(hPropertyGridWnd, hRightPanelWnd);
                                SetWindowPos(hPropertyGridWnd, NULL, borderMargin, g_ExplorerData.tabBarHeight, 
                                    effectiveRightW - borderMargin, propHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
                                ExplorerSetPropertyGridWindow(hPropertyGridWnd);
                            } else if (hPropertyGridWnd && GetParent(hPropertyGridWnd) == hRightPanelWnd) {
                                // 不再需要嵌入时，将 PropertyGrid 移回主窗口并隐藏
                                SetParent(hPropertyGridWnd, hMainWnd);
                                ShowWindow(hPropertyGridWnd, SW_HIDE);
                                ExplorerSetPropertyGridWindow(NULL);
                            }
                        } else {
                            hdwp = DeferWindowPos(hdwp, hRightPanelWnd, NULL, 
                                rightPanelLeft, topH, 0, centerTotalH, 
                                SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
                        }
                    }
                } else {
                    // 正常模式：活动栏在最左侧，AI在最右侧
                    // 布局：活动栏 | 资源管理器 | 编辑器 | AI
                    
                    // 左侧面板（资源管理器）- 在活动栏右边
                    if (hRightPanelWnd) {
                        if (g_LeftPanelVisible) {
                            hdwp = DeferWindowPos(hdwp, hRightPanelWnd, NULL, 
                                activityBarW, topH, effectiveLeftW, centerTotalH, 
                                SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
                            
                            // 如果是可视化设计模式且当前是属性标签，将PropertyGrid嵌入资源管理器
                            if (g_IsVisualDesignerActive && g_ExplorerData.activeTab == TAB_PROPERTY && hPropertyGridWnd) {
                                // PropertyGrid 放在资源管理器的标签栏下方
                                int propTop = topH + g_ExplorerData.tabBarHeight;
                                int propHeight = centerTotalH - g_ExplorerData.tabBarHeight;
                                int borderMargin = 8;  // 边框拖动区域宽度
                                
                                // 设置 PropertyGrid 为资源管理器的子窗口，留出右边框拖动区域
                                SetParent(hPropertyGridWnd, hRightPanelWnd);
                                SetWindowPos(hPropertyGridWnd, NULL, 0, g_ExplorerData.tabBarHeight, 
                                    effectiveLeftW - borderMargin, propHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
                                ExplorerSetPropertyGridWindow(hPropertyGridWnd);
                            } else if (hPropertyGridWnd && GetParent(hPropertyGridWnd) == hRightPanelWnd) {
                                // 不再需要嵌入时，将 PropertyGrid 移回主窗口并隐藏
                                SetParent(hPropertyGridWnd, hMainWnd);
                                ShowWindow(hPropertyGridWnd, SW_HIDE);
                                ExplorerSetPropertyGridWindow(NULL);
                            }
                        } else {
                            hdwp = DeferWindowPos(hdwp, hRightPanelWnd, NULL, 
                                activityBarW, topH, 0, centerTotalH, 
                                SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
                        }
                    }
                }
                
                // 计算编辑器区域起始X坐标
                int editorLeft = g_PanelsSwapped ? effectiveLeftW : (activityBarW + effectiveLeftW);
                
                // 中间上部：标签栏（占据整个中间宽度）
                if (hTabBarWnd)
                    hdwp = DeferWindowPos(hdwp, hTabBarWnd, NULL, editorLeft, topH, centerW, g_TabBarHeight,
                        SWP_NOZORDER | SWP_NOACTIVATE);
                
                // 中间上部：编辑器（在标签栏下方）
                int editorTop = topH + g_TabBarHeight;
                int editorHeight = editorH - g_TabBarHeight;
                
                if (hEditorWnd) 
                    hdwp = DeferWindowPos(hdwp, hEditorWnd, NULL, editorLeft, editorTop, centerW, editorHeight, 
                        SWP_NOZORDER | SWP_NOACTIVATE);
                
                if (hEllEditorWnd)
                    hdwp = DeferWindowPos(hdwp, hEllEditorWnd, NULL, editorLeft, editorTop, centerW, editorHeight,
                        SWP_NOZORDER | SWP_NOACTIVATE);
                
                // 可视化设计器（占据整个中间区域，组件箱作为其子窗口在内部布局）
                if (hVisualDesignerWnd)
                    hdwp = DeferWindowPos(hdwp, hVisualDesignerWnd, NULL, editorLeft, editorTop, centerW, editorHeight,
                        SWP_NOZORDER | SWP_NOACTIVATE);
                
                // 组件箱的布局由可视化设计器的 WM_SIZE 处理（因为它是设计器的子窗口）
                
                // 中间：欢迎页（占据整个编辑器区域）
                if (hWelcomePageWnd) {
                    // 根据当前是否有文件打开决定欢迎页的显示状态
                    TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                    bool hasOpenFiles = (tabData && !tabData->tabs.empty());
                    
                    if (hasOpenFiles) {
                        // 有文件打开，确保欢迎页隐藏
                        hdwp = DeferWindowPos(hdwp, hWelcomePageWnd, NULL, editorLeft, editorTop, centerW, editorHeight,
                            SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
                    } else {
                        // 没有文件打开，显示欢迎页
                        hdwp = DeferWindowPos(hdwp, hWelcomePageWnd, HWND_TOP, editorLeft, editorTop, centerW, editorHeight,
                            SWP_NOACTIVATE | SWP_SHOWWINDOW);
                    }
                }
                
                // 中间下部：输出窗口
                if (hOutputWnd) {
                    if (g_OutputPanelVisible) {
                        hdwp = DeferWindowPos(hdwp, hOutputWnd, NULL, editorLeft, topH + editorH, centerW, outputH, 
                            SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
                    } else {
                        hdwp = DeferWindowPos(hdwp, hOutputWnd, NULL, editorLeft, topH + editorH, centerW, 0, 
                            SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
                    }
                }
                
                // 右侧面板（AI聊天）- 只在非交换模式下处理
                if (!g_PanelsSwapped) {
                    int rightPanelLeft = width - effectiveRightW;
                    // 可视化设计模式下隐藏AI窗口
                    if (hAIChatWnd) {
                        if (g_AIPanelVisible && !g_IsVisualDesignerActive) {
                            hdwp = DeferWindowPos(hdwp, hAIChatWnd, NULL, rightPanelLeft, topH, effectiveRightW, centerTotalH, 
                                SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
                        } else {
                            hdwp = DeferWindowPos(hdwp, hAIChatWnd, NULL, rightPanelLeft, topH, 0, centerTotalH, 
                                SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
                        }
                    }
                    // PropertyGrid 不再在右侧显示（已嵌入左侧资源管理器的属性标签）
                    // 如果 PropertyGrid 仍在主窗口下且不需要显示，隐藏它
                    if (hPropertyGridWnd && GetParent(hPropertyGridWnd) == hMainWnd) {
                        hdwp = DeferWindowPos(hdwp, hPropertyGridWnd, NULL, rightPanelLeft, topH, 0, centerTotalH, 
                            SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
                    }
                }
                
                // 底部状态栏横跨整个宽度（从最左边开始）
                if (hStatusBar) 
                    hdwp = DeferWindowPos(hdwp, hStatusBar, NULL, 0, height - statusH, width, statusH, 
                        SWP_NOZORDER | SWP_NOACTIVATE);
                
                if (hdwp) EndDeferWindowPos(hdwp);
            }
            
            // 更新标题栏按钮位置
            int btnWidth = 46;
            int iconBtnWidth = 32;  // 功能图标按钮宽度（较小）
            int btnHeight = g_TitleBarHeight;
            g_CloseButtonRect = {width - btnWidth, 0, width, btnHeight};
            g_MaxButtonRect = {width - btnWidth * 2, 0, width - btnWidth, btnHeight};
            g_MinButtonRect = {width - btnWidth * 3, 0, width - btnWidth * 2, btnHeight};
            // 功能图标按钮（从右到左：侧边栏、输出、切换、AI）
            int iconStartX = width - btnWidth * 3 - iconBtnWidth * 4 - 8;  // 在最小化按钮左边留8px间隙
            g_ToggleSidebarButtonRect = {iconStartX + iconBtnWidth * 3, 0, iconStartX + iconBtnWidth * 4, btnHeight};
            g_ToggleOutputButtonRect = {iconStartX + iconBtnWidth * 2, 0, iconStartX + iconBtnWidth * 3, btnHeight};
            g_SwapPanelsButtonRect = {iconStartX + iconBtnWidth, 0, iconStartX + iconBtnWidth * 2, btnHeight};
            g_ToggleAIButtonRect = {iconStartX, 0, iconStartX + iconBtnWidth, btnHeight};
            
            // 更新菜单项位置
            int menuX = 50;  // 从标题后开始
            for (size_t i = 0; i < g_MenuItems.size(); i++) {
                g_MenuItems[i].rect = {menuX, 0, menuX + 60, g_TitleBarHeight};
                menuX += 60;
            }
            
            // 只在窗口真正调整大小时才需要完全重绘
            // 面板拖动只改变子窗口布局，不需要重绘标题栏等固定区域
            static int lastWidth = 0, lastHeight = 0;
            if (width != lastWidth || height != lastHeight) {
                lastWidth = width;
                lastHeight = height;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
    
    // 面板边框拖动时的布局更新（只更新面板相关窗口，不重绘标题栏/工具栏/活动栏）
    case WM_UPDATE_PANEL_LAYOUT:
        {
            // 清除消息队列中多余的 WM_UPDATE_PANEL_LAYOUT 消息，只处理最新的
            MSG msg;
            while (PeekMessage(&msg, hWnd, WM_UPDATE_PANEL_LAYOUT, WM_UPDATE_PANEL_LAYOUT, PM_REMOVE)) {
                // 丢弃重复的消息
            }
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            int width = rect.right;
            int height = rect.bottom;
            
            int activityBarW = g_ActivityBarWidth;
            int leftPanelW = g_LeftPanelVisible ? g_LeftPanelWidth : 0;
            // 可视化设计器模式下隐藏AI面板
            int rightPanelW = (g_AIPanelVisible && !g_IsVisualDesignerActive) ? g_RightPanelWidth : 0;
            int outputH = g_OutputPanelVisible ? 150 : 0;
            int topH = g_TotalTopHeight;
            int statusH = g_StatusBarHeight;
            
            int effectiveLeftW = g_PanelsSwapped ? rightPanelW : leftPanelW;
            int effectiveRightW = g_PanelsSwapped ? leftPanelW : rightPanelW;
            
            int centerW = width - activityBarW - effectiveLeftW - effectiveRightW;
            if (centerW < 100) centerW = 100;
            
            int centerTotalH = height - topH - statusH;
            int editorH = centerTotalH - outputH;
            if (editorH < 0) editorH = 0;
            
            // 计算编辑器区域起始X坐标
            int editorLeft = g_PanelsSwapped ? effectiveLeftW : (activityBarW + effectiveLeftW);
            int editorTop = topH + g_TabBarHeight;
            int editorHeight = editorH - g_TabBarHeight;
            
            // 锁定窗口更新，避免中间状态显示
            LockWindowUpdate(hWnd);
            
            // 使用 SetWindowPos 直接更新各窗口，添加 SWP_NOCOPYBITS 避免闪烁
            UINT swpFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS;
            
            // 资源管理器
            if (hRightPanelWnd) {
                int panelLeft = g_PanelsSwapped ? (width - activityBarW - effectiveRightW) : activityBarW;
                int panelWidth = g_PanelsSwapped ? effectiveRightW : effectiveLeftW;
                SetWindowPos(hRightPanelWnd, NULL, panelLeft, topH, panelWidth, centerTotalH, swpFlags);
            }
            
            // AI聊天面板 - 可视化设计模式下隐藏
            if (hAIChatWnd) {
                if (g_IsVisualDesignerActive) {
                    // 可视化设计模式下隐藏AI面板
                    ShowWindow(hAIChatWnd, SW_HIDE);
                } else {
                    int panelLeft = g_PanelsSwapped ? 0 : (width - effectiveRightW);
                    int panelWidth = g_PanelsSwapped ? effectiveLeftW : effectiveRightW;
                    SetWindowPos(hAIChatWnd, NULL, panelLeft, topH, panelWidth, centerTotalH, swpFlags);
                    if (g_AIPanelVisible) {
                        ShowWindow(hAIChatWnd, SW_SHOW);
                    }
                }
            }
            
            // 标签栏 - 位置和大小改变时不复制旧内容
            if (hTabBarWnd)
                SetWindowPos(hTabBarWnd, NULL, editorLeft, topH, centerW, g_TabBarHeight, swpFlags);
            
            // 编辑器
            if (hEditorWnd)
                SetWindowPos(hEditorWnd, NULL, editorLeft, editorTop, centerW, editorHeight, swpFlags);
            
            // ELL编辑器
            if (hEllEditorWnd)
                SetWindowPos(hEllEditorWnd, NULL, editorLeft, editorTop, centerW, editorHeight, swpFlags);
            
            // 可视化设计器
            if (hVisualDesignerWnd)
                SetWindowPos(hVisualDesignerWnd, NULL, editorLeft, editorTop, centerW, editorHeight, swpFlags);
            
            // 欢迎页
            if (hWelcomePageWnd)
                SetWindowPos(hWelcomePageWnd, NULL, editorLeft, editorTop, centerW, editorHeight, swpFlags);
            
            // 输出窗口
            if (hOutputWnd)
                SetWindowPos(hOutputWnd, NULL, editorLeft, topH + editorH, centerW, outputH, swpFlags);
            
            // 解锁窗口更新
            LockWindowUpdate(NULL);
        }
        break;
        
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            
            // 调试输出
            wchar_t debug[256];
            swprintf_s(debug, L"WM_COMMAND received: wmId=%d, wmEvent=%d\n", wmId, wmEvent);
            OutputDebugStringW(debug);
            
            // 标签栏切换事件 - 保存旧标签状态 (wmId=2002, wmEvent=旧标签索引)
            if (wmId == 2002) {
                int oldTabIndex = wmEvent;
                TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                if (tabData && oldTabIndex >= 0 && oldTabIndex < (int)tabData->tabs.size()) {
                    int oldEditorType = tabData->tabs[oldTabIndex].editorType;
                    
                    // 如果旧标签是 VisualDesigner，保存其状态
                    if (oldEditorType == 2 && g_pVisualDesigner) {
                        FormInfo* savedState = new FormInfo(g_pVisualDesigner->GetFormInfo());
                        // 释放旧的数据
                        if (tabData->tabs[oldTabIndex].editorData) {
                            delete (FormInfo*)tabData->tabs[oldTabIndex].editorData;
                        }
                        tabData->tabs[oldTabIndex].editorData = savedState;
                        OutputDebugStringW(L"[TabSwitch] Saved old tab state (VisualDesigner)\n");
                    }
                }
                return 0;
            }
            
            // 标签栏切换事件 - 加载新标签 (wmId=2000, wmEvent=新标签索引)
            if (wmId == 2000) {
                int tabIndex = wmEvent;
                TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                if (tabData && tabIndex >= 0 && tabIndex < (int)tabData->tabs.size()) {
                    int editorType = tabData->tabs[tabIndex].editorType;
                    
                    // 根据编辑器类型显示对应的编辑器，隐藏其他
                    if (editorType == 0) { // YiEditor
                        SwitchToVisualDesignerMode(false);  // 退出可视化设计模式
                        ShowWindow(hEditorWnd, SW_SHOW);
                        ShowWindow(hEllEditorWnd, SW_HIDE);
                        ShowWindow(hVisualDesignerWnd, SW_HIDE);
                        // 切换到对应的文档
                        if (g_EditorData) {
                            int docIndex = g_EditorData->FindDocument(tabData->tabs[tabIndex].filePath);
                            if (docIndex >= 0) {
                                g_EditorData->SwitchToDocument(docIndex);
                                InvalidateRect(hEditorWnd, NULL, TRUE);
                            }
                        }
                    } else if (editorType == 1) { // EllEditor (包括 .ell, .edt 等)
                        SwitchToVisualDesignerMode(false);  // 退出可视化设计模式
                        ShowWindow(hEditorWnd, SW_HIDE);
                        ShowWindow(hEllEditorWnd, SW_SHOW);
                        ShowWindow(hVisualDesignerWnd, SW_HIDE);
                        // 切换到对应的文档
                        EllEditorData* dllData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                        if (dllData) {
                            // 加载对应的文件
                            dllData->LoadFile(tabData->tabs[tabIndex].filePath);
                            InvalidateRect(hEllEditorWnd, NULL, TRUE);
                        }
                    } else if (editorType == 2) { // VisualDesigner
                        // 注意: 旧标签状态已在 2002 消息中保存
                        SwitchToVisualDesignerMode(true);  // 进入可视化设计模式
                        ShowWindow(hEditorWnd, SW_HIDE);
                        ShowWindow(hEllEditorWnd, SW_HIDE);
                        ShowWindow(hVisualDesignerWnd, SW_SHOW);
                        
                        // 加载目标标签的状态
                        if (g_pVisualDesigner) {
                            // 先检查是否有保存的状态
                            FormInfo* savedState = (FormInfo*)tabData->tabs[tabIndex].editorData;
                            if (savedState) {
                                // 恢复保存的状态
                                g_pVisualDesigner->GetFormInfo() = *savedState;
                                g_pVisualDesigner->SetFilePath(tabData->tabs[tabIndex].filePath);
                                g_pVisualDesigner->ClearSelection();
                            } else {
                                // 首次加载，从文件加载
                                g_pVisualDesigner->LoadFile(tabData->tabs[tabIndex].filePath);
                            }
                            InvalidateRect(hVisualDesignerWnd, NULL, TRUE);
                            // 刷新属性窗口
                            UpdatePropertyGridForSelection();
                        }
                    }
                }
                return 0;
            }
            
            // 标签栏关闭事件 (wmId=2001, wmEvent=标签索引)
            if (wmId == 2001) {
                int tabIndex = wmEvent;
                TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                if (tabData && tabIndex >= 0 && tabIndex < (int)tabData->tabs.size()) {
                    int editorType = tabData->tabs[tabIndex].editorType;
                    std::wstring filePath = tabData->tabs[tabIndex].filePath;
                    bool isModified = tabData->tabs[tabIndex].isModified;
                    
                    // 如果文件已修改，询问是否保存
                    if (isModified) {
                        std::wstring fileName = tabData->tabs[tabIndex].fileName;
                        std::wstring msg = L"文件 \u201C" + fileName + L"\u201D 已被修改。\n\n是否保存更改？";
                        int result = MessageBoxW(hWnd, msg.c_str(), L"保存文件", 
                                               MB_YESNOCANCEL | MB_ICONQUESTION);
                        
                        if (result == IDCANCEL) {
                            // 取消关闭
                            return 0;
                        } else if (result == IDYES) {
                            // 保存文件
                            bool saveSuccess = false;
                            
                            // 根据编辑器类型选择保存方法
                            if (editorType == 2) {
                                // VisualDesigner 保存
                                if (g_pVisualDesigner) {
                                    // 如果当前设计器显示的不是这个文件，先从保存的状态恢复
                                    if (g_pVisualDesigner->GetFilePath() != filePath) {
                                        FormInfo* savedState = (FormInfo*)tabData->tabs[tabIndex].editorData;
                                        if (savedState) {
                                            g_pVisualDesigner->GetFormInfo() = *savedState;
                                            g_pVisualDesigner->SetFilePath(filePath);
                                        }
                                    }
                                    saveSuccess = g_pVisualDesigner->SaveFile(filePath);
                                }
                            } else if (g_EditorData) {
                                int docIndex = g_EditorData->FindDocument(filePath);
                                if (docIndex >= 0 && docIndex < (int)g_EditorData->documents.size()) {
                                    EditorDocument* doc = g_EditorData->documents[docIndex];
                                    
                                    // 定义统一的保存函数（lambda）
                                    auto SaveDocument = [](HWND hMainWnd, EditorDocument* doc) -> bool {
                                        if (!doc || doc->filePath.empty()) {
                                            return false;
                                        }
                                        
                                        if (doc->fileType == FILE_TYPE_ELL) {
                                            extern HWND hEllEditorWnd;
                                            EllEditorData* ellData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                                            if (ellData) {
                                                if (ellData->currentFilePath != doc->filePath) {
                                                    ellData->LoadFile(doc->filePath);
                                                }
                                                if (ellData->SaveFile()) {
                                                    doc->modified = false;
                                                    return true;
                                                }
                                            }
                                            return false;
                                        } else {
                                            extern bool SaveFile(const std::wstring& filePath, EditorDocument* doc);
                                            if (SaveFile(doc->filePath, doc)) {
                                                doc->modified = false;
                                                return true;
                                            }
                                            return false;
                                        }
                                    };
                                    
                                    saveSuccess = SaveDocument(hWnd, doc);
                                }
                            }
                            
                            // 检查保存是否成功
                            if (!saveSuccess) {
                                MessageBoxW(hWnd, L"保存文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                return 0;
                            }
                            
                            // 清除修改标记
                            tabData->SetTabModified(tabIndex, false);
                            ExplorerSetFileModified(filePath, false);
                        } else if (result == IDNO) {
                            // IDNO: 不保存，清除修改标记后继续关闭
                            tabData->SetTabModified(tabIndex, false);
                            ExplorerSetFileModified(filePath, false);
                            
                            // 同时清除 EditorDocument 的修改标志
                            if (g_EditorData) {
                                int docIndex = g_EditorData->FindDocument(filePath);
                                if (docIndex >= 0 && docIndex < (int)g_EditorData->documents.size()) {
                                    EditorDocument* doc = g_EditorData->documents[docIndex];
                                    if (doc) {
                                        doc->modified = false;
                                    }
                                }
                            }
                            
                            // 同时清除 EllEditor 的修改标志
                            EllEditorData* ellData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                            if (ellData && ellData->editor && ellData->currentFilePath == filePath) {
                                ellData->editor->SetModified(false);
                            }
                        }
                    }
                    
                    // 从对应的编辑器中关闭文档
                    if (g_EditorData) {
                        int docIndex = g_EditorData->FindDocument(filePath);
                        if (docIndex >= 0) {
                            g_EditorData->CloseDocument(docIndex);
                        }
                    }
                    
                    // 如果关闭的是可视化设计器标签，清理保存的状态并退出设计器模式
                    if (editorType == 2) {
                        // 清理保存的 FormInfo 状态
                        if (tabData->tabs[tabIndex].editorData) {
                            delete (FormInfo*)tabData->tabs[tabIndex].editorData;
                            tabData->tabs[tabIndex].editorData = nullptr;
                        }
                        ShowWindow(hVisualDesignerWnd, SW_HIDE);  // 立即隐藏设计器窗口
                        SwitchToVisualDesignerMode(false);  // 退出设计器模式，隐藏组件箱
                    }
                    
                    // 检查是否还有其他标签（注意：CloseTab会删除标签，所以这里检查删除前的数量）
                    if (tabData->tabs.size() == 1) {
                        // 这是最后一个标签，关闭后显示欢迎页
                        SwitchToVisualDesignerMode(false);  // 确保退出设计器模式
                        ShowWindow(hEditorWnd, SW_HIDE);
                        ShowWindow(hEllEditorWnd, SW_HIDE);
                        ShowWindow(hVisualDesignerWnd, SW_HIDE);
                        ShowWindow(hWelcomePageWnd, SW_SHOW);
                        InvalidateRect(hWelcomePageWnd, NULL, TRUE);
                        UpdateWindow(hWelcomePageWnd);  // 立即更新欢迎页
                    }
                }
                return 0;
            }
            
            // 编辑器修改通知 (wmId=0, wmEvent=0x1000)
            if (wmId == 0 && wmEvent == 0x1000) {
                HWND hSourceWnd = (HWND)lParam;
                TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                
                // 判断通知来源，获取对应的文件路径
                std::wstring filePath;
                
                // 检查是否来自 VisualDesigner（可视化设计器）
                if (g_pVisualDesigner && IsWindow(hSourceWnd)) {
                    if (hSourceWnd == hVisualDesignerWnd || IsChild(hVisualDesignerWnd, hSourceWnd) ||
                        GetParent(hSourceWnd) == hVisualDesignerWnd) {
                        filePath = g_pVisualDesigner->GetFilePath();
                        // 如果没有文件路径，使用文件名
                        if (filePath.empty()) {
                            filePath = g_pVisualDesigner->GetFileName();
                        }
                    }
                }
                
                // 检查是否来自 EllEditor（TableEditor）
                if (filePath.empty()) {
                    EllEditorData* ellData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                    if (ellData && ellData->editor && IsWindow(hSourceWnd)) {
                        // 检查源窗口是否是 EllEditor 或其子窗口
                        if (hSourceWnd == hEllEditorWnd || IsChild(hEllEditorWnd, hSourceWnd) || 
                            GetParent(hSourceWnd) == hEllEditorWnd) {
                            filePath = ellData->currentFilePath;
                        }
                    }
                }
                
                // 如果不是来自 EllEditor，检查是否来自 YiEditor
                if (filePath.empty() && g_EditorData) {
                    EditorDocument* doc = g_EditorData->GetActiveDoc();
                    if (doc) {
                        filePath = doc->filePath;
                    }
                }
                
                // 找到对应的标签页并标记为已修改
                if (tabData && !filePath.empty()) {
                    for (int i = 0; i < (int)tabData->tabs.size(); i++) {
                        if (tabData->tabs[i].filePath == filePath) {
                            if (!tabData->tabs[i].isModified) {
                                tabData->SetTabModified(i, true);
                            }
                            break;
                        }
                    }
                    
                    // 标记资源管理器中的文件为已修改
                    ExplorerSetFileModified(filePath, true);
                }
                
                return 0;
            }
            
            // 组件箱选择控件类型通知 (wmEvent=0x2000)
            if (wmEvent == 0x2000) {
                OutputDebugStringW(L"[ycIDE] 收到组件箱选择通知\n");
                // 获取选中的控件类型
                if (g_pControlToolbox && g_pVisualDesigner && g_IsVisualDesignerActive) {
                    std::wstring controlType = g_pControlToolbox->GetSelectedControlType();
                    OutputDebugStringW((L"[ycIDE] 选中控件类型: " + controlType + L"\n").c_str());
                    if (!controlType.empty()) {
                        // 设置设计器为创建控件模式
                        g_pVisualDesigner->SetToolMode(controlType);
                        
                        // 更改鼠标光标提示用户可以绘制控件
                        SetCursor(LoadCursor(NULL, IDC_CROSS));
                        OutputDebugStringW(L"[ycIDE] 已设置创建模式\n");
                    }
                } else {
                    OutputDebugStringW((L"[ycIDE] 条件不满足: toolbox=" + std::to_wstring((LONG_PTR)g_pControlToolbox) + 
                        L", designer=" + std::to_wstring((LONG_PTR)g_pVisualDesigner) + 
                        L", active=" + std::to_wstring(g_IsVisualDesignerActive) + L"\n").c_str());
                }
                return 0;
            }
            
            // 组件箱停靠/浮动切换通知 (wmEvent=0x2001)
            if (wmEvent == 0x2001) {
                OutputDebugStringW(L"[ycIDE] 收到组件箱停靠/浮动切换通知\n");
                if (g_pControlToolbox) {
                    g_IsToolboxDocked = !g_IsToolboxDocked;
                    g_pControlToolbox->SetDocked(g_IsToolboxDocked);
                    
                    if (g_IsToolboxDocked) {
                        // 切换到停靠模式
                        OutputDebugStringW(L"[ycIDE] 组件箱切换到停靠模式\n");
                        // 保存浮动位置
                        SaveToolboxPosition();
                    } else {
                        // 切换到浮动模式
                        OutputDebugStringW(L"[ycIDE] 组件箱切换到浮动模式\n");
                        // 恢复保存的浮动位置
                        LoadToolboxPosition();
                    }
                    
                    // 触发可视化设计器重新布局
                    if (hVisualDesignerWnd) {
                        RECT rect;
                        GetClientRect(hVisualDesignerWnd, &rect);
                        SendMessage(hVisualDesignerWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.right, rect.bottom));
                    }
                    
                    // 重绘组件箱以更新按钮状态
                    InvalidateRect(hToolboxWnd, NULL, FALSE);
                }
                return 0;
            }
            
            // 组件箱关闭通知 (wmEvent=0x2002)
            if (wmEvent == 0x2002) {
                OutputDebugStringW(L"[ycIDE] 收到组件箱关闭通知\n");
                ShowWindow(hToolboxWnd, SW_HIDE);
                return 0;
            }
            
            // 组件箱显示模式切换通知 (wmEvent=0x2003)
            if (wmEvent == 0x2003) {
                OutputDebugStringW(L"[ycIDE] 收到组件箱显示模式切换通知\n");
                // 如果是列表模式，且在停靠状态，恢复默认宽度
                if (g_pControlToolbox && !g_pControlToolbox->IsIconMode() && g_IsToolboxDocked) {
                    // 列表模式停靠时使用固定宽度
                    if (hToolboxWnd && hVisualDesignerWnd) {
                        RECT designerRect;
                        GetClientRect(hVisualDesignerWnd, &designerRect);
                        int toolboxLeft = designerRect.right - DOCKED_TOOLBOX_WIDTH;
                        SetWindowPos(hToolboxWnd, HWND_TOP, toolboxLeft, 0, 
                            DOCKED_TOOLBOX_WIDTH, designerRect.bottom, SWP_SHOWWINDOW);
                    }
                }
                return 0;
            }
            
            // 资源管理器双击事件
            if (wmId == 1003 && wmEvent == LBN_DBLCLK) {
                std::wstring file = ExplorerGetSelectedFile();
                if (!file.empty()) {
                    // 首先检查TabBar中是否已打开
                    TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                    int existingTabIndex = -1;
                    if (tabData) {
                        existingTabIndex = tabData->FindTab(file);
                    }
                    
                    if (existingTabIndex >= 0) {
                        // 文件已打开，切换到对应标签
                        if (tabData) {
                            tabData->SetActiveTab(existingTabIndex);
                        }
                    } else {
                        // 文件未在TabBar中打开，需要打开新文件
                        // 检查文件是否存在
                        DWORD fileAttr = GetFileAttributesW(file.c_str());
                        if (fileAttr == INVALID_FILE_ATTRIBUTES) {
                            // 文件不存在，询问是否创建
                            std::wstring msg = L"文件不存在：\n" + file + L"\n\n是否创建新文件？";
                            if (MessageBoxW(hWnd, msg.c_str(), L"文件不存在", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                                // 创建空文件
                                HANDLE hFile = CreateFileW(file.c_str(), GENERIC_WRITE, 0, NULL,
                                                          CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                                if (hFile != INVALID_HANDLE_VALUE) {
                                    // 写入默认内容
                                    std::wstring defaultContent = 
                                        L".版本 2\n"
                                        L"\n"
                                        L".程序集 程序集1\n"
                                        L"\n"
                                        L".子程序 _启动子程序\n"
                                        L"\n";
                                    
                                    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, defaultContent.c_str(), -1, NULL, 0, NULL, NULL);
                                    if (utf8Len > 0) {
                                        std::string utf8Content(utf8Len - 1, 0);
                                        WideCharToMultiByte(CP_UTF8, 0, defaultContent.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                                        DWORD bytesWritten;
                                        WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
                                    }
                                    CloseHandle(hFile);
                                } else {
                                    MessageBoxW(hWnd, L"创建文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                    break;
                                }
                            } else {
                                break;
                            }
                        }
                        
                        // 确定编辑器类型
                        int editorType = GetEditorTypeByExtension(file);
                        
                        // 添加到标签栏
                        if (tabData) {
                            size_t lastSlash = file.find_last_of(L"\\/");
                            std::wstring fileName = (lastSlash != std::wstring::npos) ? file.substr(lastSlash + 1) : file;
                            tabData->AddTab(file, fileName, editorType);
                        }
                        
                        // 根据类型打开文件
                        if (editorType == 0 && g_EditorData) {
                            // YiEditor (.eyc文件)
                            // 检查文件是否已在YiEditor中打开
                            int docIndex = g_EditorData->FindDocument(file);
                            if (docIndex >= 0) {
                                // 文件已在编辑器中，切换到该文档
                                g_EditorData->SwitchToDocument(docIndex);
                                ShowWindow(hWelcomePageWnd, SW_HIDE);
                                ShowWindow(hEditorWnd, SW_SHOW);
                                ShowWindow(hEllEditorWnd, SW_HIDE);
                                InvalidateRect(hEditorWnd, NULL, TRUE);
                            } else {
                                // 文件未在编辑器中，打开新文档
                                g_EditorData->AddDocument(file);
                                EditorDocument* doc = g_EditorData->GetActiveDoc();
                                
                                wchar_t debugMsg[512];
                                swprintf_s(debugMsg, L"[文件打开] doc指针: %p, 文件: %s\n", doc, file.c_str());
                                OutputDebugStringW(debugMsg);
                                
                                if (doc) {
                                    bool loadResult = LoadFile(file, doc);
                                    swprintf_s(debugMsg, L"[文件打开] LoadFile返回: %d\n", loadResult);
                                    OutputDebugStringW(debugMsg);
                                    
                                    if (loadResult) {
                                        // 格式化所有命令行（添加括号等）
                                        FormatAllCommandLines(doc);
                                        ShowWindow(hWelcomePageWnd, SW_HIDE);
                                        ShowWindow(hEditorWnd, SW_SHOW);
                                        ShowWindow(hEllEditorWnd, SW_HIDE);
                                        InvalidateRect(hEditorWnd, NULL, TRUE);
                                        RECT rect;
                                        GetClientRect(hEditorWnd, &rect);
                                        SendMessage(hEditorWnd, WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));
                                    } else {
                                        // LoadFile失败，关闭刚创建的文档
                                        OutputDebugStringW(L"[文件打开] LoadFile失败，关闭文档\n");
                                        g_EditorData->CloseDocument(g_EditorData->activeDocIndex);
                                        MessageBoxW(hWnd, L"打开文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                    }
                                }
                            }
                        } else if (editorType == 1) {
                            // EllEditor (.ell/.edt等表格文件)
                            // 首先在 g_EditorData 中创建文档记录
                            if (g_EditorData) {
                                int docIndex = g_EditorData->FindDocument(file);
                                if (docIndex < 0) {
                                    // 创建新的 EditorDocument 用于跟踪修改状态
                                    g_EditorData->AddDocument(file);
                                    EditorDocument* doc = g_EditorData->GetActiveDoc();
                                    if (doc) {
                                        doc->fileType = FILE_TYPE_ELL;
                                    }
                                }
                            }
                            
                            EllEditorData* dllData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                            
                            wchar_t debugMsg[512];
                            swprintf_s(debugMsg, L"[表格文件打开] dllData指针: %p, 文件: %s\n", dllData, file.c_str());
                            OutputDebugStringW(debugMsg);
                            
                            if (dllData) {
                                ShowWindow(hWelcomePageWnd, SW_HIDE);
                                ShowWindow(hEditorWnd, SW_HIDE);
                                ShowWindow(hEllEditorWnd, SW_SHOW);
                                dllData->LoadFile(file);
                                InvalidateRect(hEllEditorWnd, NULL, TRUE);
                                
                                OutputDebugStringW(L"[表格文件打开] 已显示EllEditor并加载文档\n");
                            }
                        }
                    }
                    
                    // 刷新AI聊天窗口显示文件名
                    if (hAIChatWnd) {
                        InvalidateRect(hAIChatWnd, NULL, TRUE);
                    }
                }
            }
            
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_THEME_DARK:
            case IDM_THEME_LIGHT:
                // 旧的固定主题处理（保留兼容性，但实际不再使用）
                break;
            case IDM_THEME_CUSTOM:
                // 打开主题文件夹
                {
                    wchar_t exePath[MAX_PATH];
                    GetModuleFileNameW(NULL, exePath, MAX_PATH);
                    std::wstring themesFolder(exePath);
                    size_t pos = themesFolder.find_last_of(L"\\");
                    if (pos != std::wstring::npos) {
                        themesFolder = themesFolder.substr(0, pos + 1) + L"themes";
                    }
                    // 确保文件夹存在
                    CreateDirectoryW(themesFolder.c_str(), NULL);
                    // 打开文件夹
                    ShellExecuteW(NULL, L"explore", themesFolder.c_str(), NULL, NULL, SW_SHOWNORMAL);
                }
                break;
            case IDM_SYSTEM_CONFIG:
                MessageBoxW(hWnd, L"系统配置功能正在开发中...", L"系统配置", MB_OK | MB_ICONINFORMATION);
                break;
            case IDM_VIEW_TOOLBOX:
                // 切换组件箱显示
                if (hToolboxWnd) {
                    if (IsWindowVisible(hToolboxWnd)) {
                        ShowWindow(hToolboxWnd, SW_HIDE);
                    } else {
                        ShowWindow(hToolboxWnd, SW_SHOW);
                    }
                }
                break;
            case IDM_LIBRARY_CONFIG:
                {
                    // 创建支持库配置窗口
                    WNDCLASSEXW wcex = {0};
                    wcex.cbSize = sizeof(WNDCLASSEX);
                    wcex.lpfnWndProc = LibraryConfigWndProc;
                    wcex.hInstance = hInst;
                    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
                    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
                    wcex.lpszClassName = L"LibraryConfigClass";
                    
                    static bool classRegistered = false;
                    if (!classRegistered) {
                        RegisterClassExW(&wcex);
                        classRegistered = true;
                    }
                    
                    HWND hLibConfigWnd = CreateWindowExW(
                        0, L"LibraryConfigClass", L"支持库配置",
                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                        CW_USEDEFAULT, CW_USEDEFAULT, 800, 500,
                        hWnd, nullptr, hInst, nullptr);
                    
                    if (hLibConfigWnd) {
                        ShowWindow(hLibConfigWnd, SW_SHOW);
                        UpdateWindow(hLibConfigWnd);
                    }
                }
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_NEW:
                {
                    // 创建新文件
                    if (g_EditorData) {
                        g_EditorData->AddDocument(L"");
                        InvalidateRect(hEditorWnd, NULL, TRUE);
                    }
                }
                break;
            case IDM_OPEN:
                {
                    // 打开文件
                    if (g_EditorData) {
                        std::wstring openPath;
                        if (ShowOpenDialog(hWnd, openPath)) {
                            g_EditorData->AddDocument(openPath);
                            EditorDocument* doc = g_EditorData->GetActiveDoc();
                            if (doc) {
                                LoadFile(openPath, doc);
                                // 格式化所有命令行（添加括号等）
                                FormatAllCommandLines(doc);
                            }
                            InvalidateRect(hEditorWnd, NULL, TRUE);
                            // 发送WM_SIZE消息以更新滚动条
                            RECT rect;
                            GetClientRect(hEditorWnd, &rect);
                            SendMessage(hEditorWnd, WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));
                            
                            // 在资源管理器中选中该文件
                            ExplorerSelectFile(openPath);
                            
                            // 刷新AI聊天窗口显示文件名
                            if (hAIChatWnd) {
                                InvalidateRect(hAIChatWnd, NULL, TRUE);
                            }
                        }
                    }
                }
                break;
            case IDM_SAVE:
                {
                    OutputDebugStringW(L"[IDM_SAVE] Save triggered\n");
                    // 保存文件
                    // 检查当前是否是可视化设计器模式
                    if (g_IsVisualDesignerActive && g_pVisualDesigner) {
                        OutputDebugStringW(L"[IDM_SAVE] VisualDesigner active\n");
                        std::wstring filePath = g_pVisualDesigner->GetFilePath();
                        wchar_t debugMsg[512];
                        swprintf_s(debugMsg, L"[IDM_SAVE] FilePath: %s\n", filePath.c_str());
                        OutputDebugStringW(debugMsg);
                        
                        if (!filePath.empty()) {
                            if (g_pVisualDesigner->SaveFile(filePath)) {
                                OutputDebugStringW(L"[IDM_SAVE] SaveFile succeeded\n");
                                // 更新标签页修改状态
                                TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                                if (tabData) {
                                    int tabIndex = tabData->FindTab(filePath);
                                    if (tabIndex >= 0) {
                                        tabData->SetTabModified(tabIndex, false);
                                    }
                                }
                                // 更新资源管理器修改标记
                                ExplorerSetFileModified(filePath, false);
                                InvalidateRect(hVisualDesignerWnd, NULL, TRUE);
                            } else {
                                OutputDebugStringW(L"[IDM_SAVE] SaveFile failed!\n");
                                MessageBoxW(hWnd, L"保存文件失败！", L"错误", MB_OK | MB_ICONERROR);
                            }
                        } else {
                            // 没有文件路径，调用另存为
                            std::wstring savePath;
                            if (ShowSaveDialog(hWnd, savePath)) {
                                if (g_pVisualDesigner->SaveFile(savePath)) {
                                    // 更新标签页修改状态
                                    TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                                    if (tabData) {
                                        int tabIndex = tabData->FindTab(savePath);
                                        if (tabIndex >= 0) {
                                            tabData->SetTabModified(tabIndex, false);
                                        }
                                    }
                                    // 更新资源管理器修改标记
                                    ExplorerSetFileModified(savePath, false);
                                    InvalidateRect(hVisualDesignerWnd, NULL, TRUE);
                                } else {
                                    MessageBoxW(hWnd, L"保存文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                }
                            }
                        }
                    }
                    else if (g_EditorData) {
                        EditorDocument* doc = g_EditorData->GetActiveDoc();
                        if (doc) {
                            if (!doc->filePath.empty()) {
                                // 已有文件路径，直接保存
                                if (!SaveFile(doc->filePath, doc)) {
                                    MessageBoxW(hWnd, L"保存文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                } else {
                                    // 更新标签页修改状态
                                    TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                                    if (tabData) {
                                        int tabIndex = tabData->FindTab(doc->filePath);
                                        if (tabIndex >= 0) {
                                            tabData->SetTabModified(tabIndex, false);
                                        }
                                    }
                                    // 更新资源管理器修改标记
                                    ExplorerSetFileModified(doc->filePath, false);
                                    InvalidateRect(hEditorWnd, NULL, TRUE);
                                }
                            } else {
                                // 没有文件路径，调用另存为
                                std::wstring savePath;
                                if (ShowSaveDialog(hWnd, savePath)) {
                                    if (SaveFile(savePath, doc)) {
                                        size_t pos = savePath.find_last_of(L"\\/");
                                        doc->fileName = (pos != std::wstring::npos) ? savePath.substr(pos + 1) : savePath;
                                        // 更新资源管理器修改标记
                                        ExplorerSetFileModified(savePath, false);
                                        InvalidateRect(hEditorWnd, NULL, TRUE);
                                    } else {
                                        MessageBoxW(hWnd, L"保存文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            case IDM_SAVE_AS:
                {
                    // 另存为
                    if (g_EditorData) {
                        EditorDocument* doc = g_EditorData->GetActiveDoc();
                        if (doc) {
                            std::wstring savePath;
                            if (ShowSaveDialog(hWnd, savePath)) {
                                if (SaveFile(savePath, doc)) {
                                    // 更新文件名
                                    size_t pos = savePath.find_last_of(L"\\/");
                                    doc->fileName = (pos != std::wstring::npos) ? savePath.substr(pos + 1) : savePath;
                                    InvalidateRect(hEditorWnd, NULL, TRUE);
                                } else {
                                    MessageBoxW(hWnd, L"保存文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                }
                            }
                        }
                    }
                }
                break;
            case IDM_OPEN_FOLDER:
                {
                    // 打开文件夹
                    BROWSEINFOW bi = { 0 };
                    bi.hwndOwner = hWnd;
                    bi.lpszTitle = L"选择文件夹";
                    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
                    
                    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
                    if (pidl != nullptr) {
                        WCHAR folderPath[MAX_PATH];
                        if (SHGetPathFromIDListW(pidl, folderPath)) {
                            ExplorerAddFolder(folderPath);
                        }
                        CoTaskMemFree(pidl);
                    }
                }
                break;
            case IDM_CLOSE_FOLDER:
                {
                    // 关闭文件夹
                    ExplorerCloseFolder();
                }
                break;
            case IDM_NEW_PROJECT:
                {
                    // 创建新项目 - 使用新的项目对话框
                    
                    // 检查是否有未保存的文件
                    TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                    bool hasUnsavedFiles = false;
                    if (tabData) {
                        for (const auto& tab : tabData->tabs) {
                            if (tab.isModified) {
                                hasUnsavedFiles = true;
                                break;
                            }
                        }
                    }
                    
                    if (hasUnsavedFiles) {
                        int saveResult = MessageBoxW(hWnd, 
                            L"有文件尚未保存，是否保存更改？\n\n"
                            L"是 - 保存后新建项目\n"
                            L"否 - 不保存直接新建项目\n"
                            L"取消 - 放弃新建", 
                            L"保存更改", MB_YESNOCANCEL | MB_ICONWARNING);
                        if (saveResult == IDCANCEL) {
                            break;  // 取消操作，放弃新建
                        } else if (saveResult == IDYES) {
                            // 保存所有已修改的文件 - 逐个保存
                            for (int i = 0; i < (int)tabData->tabs.size(); i++) {
                                if (tabData->tabs[i].isModified) {
                                    // 切换到该标签并保存
                                    tabData->activeTabIndex = i;
                                    SendMessage(hWnd, WM_COMMAND, IDM_SAVE, 0);
                                }
                            }
                        }
                        // 如果选择"否"，不保存直接继续新建项目
                    }
                    
                    NewProjectResult result = NewProjectDialog::Show(hWnd);
                    if (result.success) {
                        auto& pm = ProjectManager::GetInstance();
                        // 转换项目类型
                        ProjectOutputType outputType;
                        switch (result.type) {
                            case ProjectType::Console: outputType = ProjectOutputType::Console; break;
                            case ProjectType::WindowsApp: outputType = ProjectOutputType::WindowsApp; break;
                            case ProjectType::DynamicLibrary: outputType = ProjectOutputType::DynamicLibrary; break;
                            default: outputType = ProjectOutputType::Console; break;
                        }
                        
                        if (pm.CreateProject(result.projectPath, result.projectName, outputType)) {
                            // 关闭所有打开的文件和标签
                            if (g_EditorData) {
                                while (!g_EditorData->documents.empty()) {
                                    g_EditorData->CloseDocument(0);
                                }
                            }
                            // 清空标签栏
                            if (tabData) {
                                tabData->tabs.clear();
                                tabData->activeTabIndex = -1;
                                InvalidateRect(hTabBarWnd, NULL, TRUE);
                            }
                            
                            // 加载项目到资源管理器
                            ExplorerLoadProject();
                            // 更新菜单（添加"插入"菜单）
                            UpdateMenuItems();
                            InvalidateRect(hEditorWnd, NULL, TRUE);
                            
                            // 自动打开主源代码文件
                            std::wstring mainFile = pm.GetMainFile();
                            if (!mainFile.empty() && g_EditorData) {
                                // 关闭欢迎页
                                g_EditorData->showWelcomePage = false;
                                g_EditorData->AddDocument(mainFile);
                                EditorDocument* doc = g_EditorData->GetActiveDoc();
                                if (doc) {
                                    LoadFile(mainFile, doc);
                                    // 格式化所有命令行（添加括号等）
                                    FormatAllCommandLines(doc);
                                }
                                
                                // 添加到标签栏
                                if (tabData) {
                                    size_t lastSlash = mainFile.find_last_of(L"\\/");
                                    std::wstring fileName = (lastSlash != std::wstring::npos) ? mainFile.substr(lastSlash + 1) : mainFile;
                                    tabData->AddTab(mainFile, fileName, 0);  // editorType = 0 for YiEditor
                                    InvalidateRect(hTabBarWnd, NULL, TRUE);
                                }
                                
                                // 显示编辑器窗口，隐藏欢迎页
                                ShowWindow(hWelcomePageWnd, SW_HIDE);
                                ShowWindow(hEditorWnd, SW_SHOW);
                                // 强制刷新编辑器窗口
                                InvalidateRect(hEditorWnd, NULL, TRUE);
                                UpdateWindow(hEditorWnd);
                                RECT rect;
                                GetClientRect(hEditorWnd, &rect);
                                SendMessage(hEditorWnd, WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));
                            }
                            
                            // 显示成功消息
                            std::wstring msg = L"项目创建成功！\n\n";
                            msg += L"项目名称: " + result.projectName + L"\n";
                            msg += L"项目类型: " + std::wstring(GetProjectTypeName(result.type)) + L"\n";
                            msg += L"保存位置: " + result.projectDir;
                            MessageBoxW(hWnd, msg.c_str(), L"提示", MB_OK | MB_ICONINFORMATION);
                        } else {
                            MessageBoxW(hWnd, L"创建项目失败！", L"错误", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
            case IDM_OPEN_PROJECT:
                {
                    // 打开项目
                    std::wstring projectPath;
                    if (ShowOpenProjectDialog(hWnd, projectPath)) {
                        auto& pm = ProjectManager::GetInstance();
                        if (pm.OpenProject(projectPath)) {
                            // 关闭所有打开的文件
                            if (g_EditorData) {
                                while (!g_EditorData->documents.empty()) {
                                    g_EditorData->CloseDocument(0);
                                }
                            }
                            // 清空标签栏
                            TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                            if (tabData) {
                                tabData->tabs.clear();
                                tabData->activeTabIndex = -1;
                                InvalidateRect(hTabBarWnd, NULL, TRUE);
                            }
                            
                            // 加载项目到资源管理器
                            ExplorerLoadProject();
                            // 更新菜单（添加"插入"菜单）
                            UpdateMenuItems();
                            InvalidateRect(hEditorWnd, NULL, TRUE);
                            
                            // 打开主文件（如果有）
                            std::wstring mainFile = pm.GetMainFile();
                            if (!mainFile.empty() && g_EditorData) {
                                // 关闭欢迎页
                                g_EditorData->showWelcomePage = false;
                                g_EditorData->AddDocument(mainFile);
                                EditorDocument* doc = g_EditorData->GetActiveDoc();
                                if (doc) {
                                    LoadFile(mainFile, doc);
                                    // 格式化所有命令行（添加括号等）
                                    FormatAllCommandLines(doc);
                                }
                                
                                // 添加到标签栏
                                if (tabData) {
                                    size_t lastSlash = mainFile.find_last_of(L"\\/");
                                    std::wstring fileName = (lastSlash != std::wstring::npos) ? mainFile.substr(lastSlash + 1) : mainFile;
                                    tabData->AddTab(mainFile, fileName, 0);  // editorType = 0 for YiEditor
                                    InvalidateRect(hTabBarWnd, NULL, TRUE);
                                }
                                
                                // 显示编辑器窗口，隐藏欢迎页
                                ShowWindow(hWelcomePageWnd, SW_HIDE);
                                ShowWindow(hEditorWnd, SW_SHOW);
                                // 强制刷新编辑器窗口
                                InvalidateRect(hEditorWnd, NULL, TRUE);
                                UpdateWindow(hEditorWnd);
                                RECT rect;
                                GetClientRect(hEditorWnd, &rect);
                                SendMessage(hEditorWnd, WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));
                            }
                        } else {
                            MessageBoxW(hWnd, L"打开项目失败！", L"错误", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
            case IDM_CLOSE_PROJECT:
                {
                    // 关闭项目
                    auto& pm = ProjectManager::GetInstance();
                    if (pm.HasOpenProject()) {
                        pm.CloseProject();
                        ExplorerCloseProject();
                        // 关闭所有打开的文件
                        if (g_EditorData) {
                            while (!g_EditorData->documents.empty()) {
                                g_EditorData->CloseDocument(0);
                            }
                        }
                        // 更新菜单（移除"插入"菜单）
                        UpdateMenuItems();
                        InvalidateRect(hEditorWnd, NULL, TRUE);
                        MessageBoxW(hWnd, L"项目已关闭！", L"提示", MB_OK | MB_ICONINFORMATION);
                    }
                }
                break;
            case IDM_ADD_FILE_TO_PROJECT:
                {
                    // 添加文件到项目
                    auto& pm = ProjectManager::GetInstance();
                    if (!pm.HasOpenProject()) {
                        MessageBoxW(hWnd, L"请先打开或创建一个项目！", L"提示", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    std::wstring filePath;
                    if (ShowOpenDialog(hWnd, filePath)) {
                        if (pm.AddFileToProject(filePath)) {
                            // 刷新资源管理器
                            ExplorerLoadProject();
                            MessageBoxW(hWnd, L"文件已添加到项目！", L"提示", MB_OK | MB_ICONINFORMATION);
                        } else {
                            MessageBoxW(hWnd, L"添加文件失败！文件可能已存在于项目中。", L"错误", MB_OK | MB_ICONERROR);
                        }
                    }
                }
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            
            // 插入菜单命令
            case IDM_INSERT_MODULE:
                {
                    auto& pm = ProjectManager::GetInstance();
                    if (pm.HasOpenProject()) {
                        const ProjectInfo* project = pm.GetCurrentProject();
                        if (project) {
                            // 生成唯一的程序集文件名
                            int counter = 1;
                            std::wstring baseName = L"程序集";
                            std::wstring fileName;
                            std::wstring filePath;
                            do {
                                fileName = baseName + std::to_wstring(counter) + L".eyc";
                                filePath = project->projectDirectory + L"\\" + fileName;
                                counter++;
                            } while (GetFileAttributesW(filePath.c_str()) != INVALID_FILE_ATTRIBUTES);
                            
                            // 创建程序集文件
                            std::wstring content = L".版本 2\n\n.程序集 " + baseName + std::to_wstring(counter - 1) + L"\n\n";
                            
                            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
                            if (utf8Len > 0) {
                                std::string utf8Content(utf8Len - 1, 0);
                                WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                                
                                HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, NULL,
                                                          CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                                if (hFile != INVALID_HANDLE_VALUE) {
                                    DWORD bytesWritten;
                                    WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
                                    CloseHandle(hFile);
                                    
                                    // 添加到项目
                                    pm.AddFileToProject(filePath);
                                    ExplorerLoadProject();
                                    
                                    // 打开文件
                                    SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(1003, LBN_DBLCLK), 0);
                                    ExplorerSelectFile(filePath);
                                    
                                    // 模拟双击打开
                                    if (g_EditorData) {
                                        g_EditorData->AddDocument(filePath);
                                        EditorDocument* doc = g_EditorData->GetActiveDoc();
                                        if (doc) {
                                            LoadFile(filePath, doc);
                                        }
                                        
                                        TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                                        if (tabData) {
                                            tabData->AddTab(filePath, fileName, 0);
                                        }
                                        
                                        ShowWindow(hWelcomePageWnd, SW_HIDE);
                                        ShowWindow(hEditorWnd, SW_SHOW);
                                        ShowWindow(hEllEditorWnd, SW_HIDE);
                                        InvalidateRect(hEditorWnd, NULL, TRUE);
                                    }
                                } else {
                                    MessageBoxW(hWnd, L"创建程序集文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                }
                            }
                        }
                    }
                }
                break;
            case IDM_INSERT_DLL_COMMAND:
                {
                    auto& pm = ProjectManager::GetInstance();
                    if (pm.HasOpenProject()) {
                        const ProjectInfo* project = pm.GetCurrentProject();
                        if (project) {
                            // 生成唯一的DLL命令文件名
                            int counter = 1;
                            std::wstring baseName = L"DLL命令";
                            std::wstring fileName;
                            std::wstring filePath;
                            do {
                                fileName = baseName + std::to_wstring(counter) + L".ell";
                                filePath = project->projectDirectory + L"\\" + fileName;
                                counter++;
                            } while (GetFileAttributesW(filePath.c_str()) != INVALID_FILE_ATTRIBUTES);
                            
                            // 创建DLL命令文件
                            std::wstring content = L".版本 2\n\n";
                            
                            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
                            if (utf8Len > 0) {
                                std::string utf8Content(utf8Len - 1, 0);
                                WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                                
                                HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, NULL,
                                                          CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                                if (hFile != INVALID_HANDLE_VALUE) {
                                    DWORD bytesWritten;
                                    WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
                                    CloseHandle(hFile);
                                    
                                    // 添加到项目
                                    pm.AddFileToProject(filePath);
                                    ExplorerLoadProject();
                                    ExplorerSelectFile(filePath);
                                    
                                    // 打开DLL编辑器
                                    if (g_EditorData) {
                                        g_EditorData->AddDocument(filePath);
                                        EditorDocument* doc = g_EditorData->GetActiveDoc();
                                        if (doc) {
                                            doc->fileType = FILE_TYPE_ELL;
                                        }
                                    }
                                    
                                    TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                                    if (tabData) {
                                        tabData->AddTab(filePath, fileName, 1);
                                    }
                                    
                                    EllEditorData* dllData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                                    if (dllData) {
                                        ShowWindow(hWelcomePageWnd, SW_HIDE);
                                        ShowWindow(hEditorWnd, SW_HIDE);
                                        ShowWindow(hEllEditorWnd, SW_SHOW);
                                        dllData->LoadFile(filePath);
                                        InvalidateRect(hEllEditorWnd, NULL, TRUE);
                                    }
                                } else {
                                    MessageBoxW(hWnd, L"创建DLL命令文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                }
                            }
                        }
                    }
                }
                break;
            case IDM_INSERT_DATATYPE:
                {
                    auto& pm = ProjectManager::GetInstance();
                    if (pm.HasOpenProject()) {
                        const ProjectInfo* project = pm.GetCurrentProject();
                        if (project) {
                            // 检查是否已存在数据类型文件（一个项目只能有一个数据类型文件）
                            bool hasDataTypeFile = false;
                            for (const auto& file : project->files) {
                                std::wstring ext = file.filePath.substr(file.filePath.find_last_of(L'.'));
                                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                                if (ext == L".edt") {
                                    hasDataTypeFile = true;
                                    // 提取文件名
                                    size_t pos = file.filePath.find_last_of(L"\\/");
                                    std::wstring existingFileName = (pos != std::wstring::npos) ? file.filePath.substr(pos + 1) : file.filePath;
                                    MessageBoxW(hWnd, 
                                        (L"项目中已存在数据类型文件：" + existingFileName + L"\n\n一个项目只能有一个数据类型文件！").c_str(), 
                                        L"提示", MB_OK | MB_ICONINFORMATION);
                                    break;
                                }
                            }
                            
                            // 如果不存在数据类型文件，则创建
                            if (!hasDataTypeFile) {
                                std::wstring fileName = L"数据类型.edt";
                                std::wstring filePath = project->projectDirectory + L"\\" + fileName;
                                
                                // 创建数据类型文件
                                std::wstring content = L".版本 2\n\n.数据类型 新数据类型, 公开\n    .成员 成员1, 整数型\n\n";
                                
                                int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
                                if (utf8Len > 0) {
                                    std::string utf8Content(utf8Len - 1, 0);
                                    WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                                    
                                    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, NULL,
                                                              CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                                    if (hFile != INVALID_HANDLE_VALUE) {
                                        DWORD bytesWritten;
                                        WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
                                        CloseHandle(hFile);
                                        
                                        // 添加到项目
                                        pm.AddFileToProject(filePath);
                                        ExplorerLoadProject();
                                        ExplorerSelectFile(filePath);
                                        
                                        // 添加标签并打开数据类型编辑器（通过 EllEditor）
                                        TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                                        if (tabData) {
                                            tabData->AddTab(filePath, fileName, 1); // editorType = 1 for EllEditor
                                            InvalidateRect(hTabBarWnd, NULL, TRUE);
                                        }
                                        
                                        // 通过 EllEditor 打开数据类型文件
                                        EllEditorData* dllData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                                        if (dllData) {
                                            ShowWindow(hWelcomePageWnd, SW_HIDE);
                                            ShowWindow(hEditorWnd, SW_HIDE);
                                            ShowWindow(hEllEditorWnd, SW_SHOW);
                                            dllData->LoadFile(filePath);
                                            InvalidateRect(hEllEditorWnd, NULL, TRUE);
                                        }
                                    } else {
                                        MessageBoxW(hWnd, L"创建数据类型文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            case IDM_INSERT_GLOBAL_VAR:
                {
                    auto& pm = ProjectManager::GetInstance();
                    if (pm.HasOpenProject()) {
                        const ProjectInfo* project = pm.GetCurrentProject();
                        if (project) {
                            // 检查是否已存在全局变量文件（一个项目只能有一个全局变量文件）
                            bool hasGlobalVarFile = false;
                            for (const auto& file : project->files) {
                                std::wstring ext = file.filePath.substr(file.filePath.find_last_of(L'.'));
                                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                                if (ext == L".egv") {
                                    hasGlobalVarFile = true;
                                    // 提取文件名
                                    size_t pos = file.filePath.find_last_of(L"\\/");
                                    std::wstring existingFileName = (pos != std::wstring::npos) ? file.filePath.substr(pos + 1) : file.filePath;
                                    MessageBoxW(hWnd, 
                                        (L"项目中已存在全局变量文件：" + existingFileName + L"\n\n一个项目只能有一个全局变量文件！").c_str(), 
                                        L"提示", MB_OK | MB_ICONINFORMATION);
                                    break;
                                }
                            }
                            
                            // 如果不存在全局变量文件，则创建
                            if (!hasGlobalVarFile) {
                                std::wstring fileName = L"全局变量.egv";
                                std::wstring filePath = project->projectDirectory + L"\\" + fileName;
                                
                                // 创建全局变量文件
                                std::wstring content = L".版本 2\n\n";
                                
                                int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
                                if (utf8Len > 0) {
                                    std::string utf8Content(utf8Len - 1, 0);
                                    WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                                    
                                    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, NULL,
                                                              CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                                    if (hFile != INVALID_HANDLE_VALUE) {
                                        DWORD bytesWritten;
                                        WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
                                        CloseHandle(hFile);
                                        
                                        // 添加到项目
                                        pm.AddFileToProject(filePath);
                                        ExplorerLoadProject();
                                        ExplorerSelectFile(filePath);
                                        
                                        // TODO: 打开全局变量编辑器
                                        MessageBoxW(hWnd, (L"已创建全局变量文件：" + fileName).c_str(), L"提示", MB_OK | MB_ICONINFORMATION);
                                    } else {
                                        MessageBoxW(hWnd, L"创建全局变量文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            case IDM_INSERT_CONSTANT:
                {
                    auto& pm = ProjectManager::GetInstance();
                    if (pm.HasOpenProject()) {
                        const ProjectInfo* project = pm.GetCurrentProject();
                        if (project) {
                            // 生成唯一的常量文件名
                            int counter = 1;
                            std::wstring baseName = L"常量";
                            std::wstring fileName;
                            std::wstring filePath;
                            do {
                                fileName = baseName + std::to_wstring(counter) + L".ecs";
                                filePath = project->projectDirectory + L"\\" + fileName;
                                counter++;
                            } while (GetFileAttributesW(filePath.c_str()) != INVALID_FILE_ATTRIBUTES);
                            
                            // 创建常量文件
                            std::wstring content = L"# 常量定义文件\n# 格式: 常量名, 类型, 值, 公开/私有, 备注\n\n";
                            
                            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
                            if (utf8Len > 0) {
                                std::string utf8Content(utf8Len - 1, 0);
                                WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                                
                                HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, NULL,
                                                          CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                                if (hFile != INVALID_HANDLE_VALUE) {
                                    DWORD bytesWritten;
                                    WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
                                    CloseHandle(hFile);
                                    
                                    // 添加到项目
                                    pm.AddFileToProject(filePath);
                                    ExplorerLoadProject();
                                    ExplorerSelectFile(filePath);
                                    
                                    // TODO: 打开常量编辑器
                                    MessageBoxW(hWnd, (L"已创建常量文件：" + fileName).c_str(), L"提示", MB_OK | MB_ICONINFORMATION);
                                } else {
                                    MessageBoxW(hWnd, L"创建常量文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                }
                            }
                        }
                    }
                }
                break;
            case IDM_INSERT_WINDOW:
                {
                    auto& pm = ProjectManager::GetInstance();
                    if (pm.HasOpenProject()) {
                        const ProjectInfo* project = pm.GetCurrentProject();
                        if (project) {
                            // 生成唯一的窗口文件名
                            int counter = 1;
                            std::wstring baseName = L"窗口";
                            std::wstring fileName;
                            std::wstring filePath;
                            do {
                                fileName = baseName + std::to_wstring(counter) + L".efw";
                                filePath = project->projectDirectory + L"\\" + fileName;
                                counter++;
                            } while (GetFileAttributesW(filePath.c_str()) != INVALID_FILE_ATTRIBUTES);
                            
                            // 创建窗口文件（JSON格式存储窗口设计信息）
                            std::wstring content = L"{\n  \"type\": \"window\",\n  \"name\": \"窗口" + std::to_wstring(counter - 1) + L"\",\n  \"width\": 640,\n  \"height\": 480,\n  \"title\": \"新窗口\",\n  \"controls\": []\n}\n";
                            
                            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
                            if (utf8Len > 0) {
                                std::string utf8Content(utf8Len - 1, 0);
                                WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                                
                                HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, NULL,
                                                          CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                                if (hFile != INVALID_HANDLE_VALUE) {
                                    DWORD bytesWritten;
                                    WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
                                    CloseHandle(hFile);
                                    
                                    // 添加到项目
                                    pm.AddFileToProject(filePath);
                                    ExplorerLoadProject();
                                    ExplorerSelectFile(filePath);
                                    
                                    // TODO: 打开窗口可视化设计器
                                    MessageBoxW(hWnd, (L"已创建窗口文件：" + fileName).c_str(), L"提示", MB_OK | MB_ICONINFORMATION);
                                } else {
                                    MessageBoxW(hWnd, L"创建窗口文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                }
                            }
                        }
                    }
                }
                break;
            case IDM_INSERT_CLASS_MODULE:
                {
                    auto& pm = ProjectManager::GetInstance();
                    if (pm.HasOpenProject()) {
                        const ProjectInfo* project = pm.GetCurrentProject();
                        if (project) {
                            // 生成唯一的类模块文件名
                            int counter = 1;
                            std::wstring baseName = L"类模块";
                            std::wstring fileName;
                            std::wstring filePath;
                            do {
                                fileName = baseName + std::to_wstring(counter) + L".ecm";
                                filePath = project->projectDirectory + L"\\" + fileName;
                                counter++;
                            } while (GetFileAttributesW(filePath.c_str()) != INVALID_FILE_ATTRIBUTES);
                            
                            // 创建类模块文件
                            std::wstring content = L".版本 2\n\n.程序集 类模块" + std::to_wstring(counter - 1) + L"\n\n' 类的初始化方法\n.子程序 _初始化\n\n' 类的销毁方法\n.子程序 _销毁\n\n";
                            
                            int utf8Len = WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, NULL, 0, NULL, NULL);
                            if (utf8Len > 0) {
                                std::string utf8Content(utf8Len - 1, 0);
                                WideCharToMultiByte(CP_UTF8, 0, content.c_str(), -1, &utf8Content[0], utf8Len, NULL, NULL);
                                
                                HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, 0, NULL,
                                                          CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                                if (hFile != INVALID_HANDLE_VALUE) {
                                    DWORD bytesWritten;
                                    WriteFile(hFile, utf8Content.c_str(), (DWORD)utf8Content.size(), &bytesWritten, NULL);
                                    CloseHandle(hFile);
                                    
                                    // 添加到项目
                                    pm.AddFileToProject(filePath);
                                    ExplorerLoadProject();
                                    ExplorerSelectFile(filePath);
                                    
                                    // 打开类模块（使用YiEditor）
                                    if (g_EditorData) {
                                        g_EditorData->AddDocument(filePath);
                                        EditorDocument* doc = g_EditorData->GetActiveDoc();
                                        if (doc) {
                                            LoadFile(filePath, doc);
                                        }
                                        
                                        TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                                        if (tabData) {
                                            tabData->AddTab(filePath, fileName, 0);
                                        }
                                        
                                        ShowWindow(hWelcomePageWnd, SW_HIDE);
                                        ShowWindow(hEditorWnd, SW_SHOW);
                                        ShowWindow(hEllEditorWnd, SW_HIDE);
                                        InvalidateRect(hEditorWnd, NULL, TRUE);
                                    }
                                } else {
                                    MessageBoxW(hWnd, L"创建类模块文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                }
                            }
                        }
                    }
                }
                break;
            
            // 欢迎页按钮命令
            case ID_WELCOME_NEW_PROJECT:
                SendMessage(hWnd, WM_COMMAND, IDM_NEW_PROJECT, 0);
                break;
            case ID_WELCOME_OPEN_PROJECT:
                SendMessage(hWnd, WM_COMMAND, IDM_OPEN_PROJECT, 0);
                break;
            case ID_WELCOME_OPEN_FILE:
                SendMessage(hWnd, WM_COMMAND, IDM_OPEN, 0);
                break;
            case ID_WELCOME_OPEN_FOLDER:
                SendMessage(hWnd, WM_COMMAND, IDM_OPEN_FOLDER, 0);
                break;
            
            // 编译运行命令
            case ID_BUILD_RUN:
                {
                    OutputDebugStringW(L"[Compile] ID_BUILD_RUN triggered\n");
                    
                    // 显示输出面板
                    if (!g_OutputPanelVisible) {
                        g_OutputPanelVisible = true;
                        // 触发布局更新
                        RECT rect;
                        GetClientRect(hWnd, &rect);
                        SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.right, rect.bottom));
                    }
                    
                    // 清空并切换到输出标签
                    if (g_pOutputPanel) {
                        g_pOutputPanel->ClearOutput();
                        g_pOutputPanel->SetActiveTab(OutputTabType::Output);
                    }
                    
                    // 在后台线程中执行编译
                    HWND hMainWnd = hWnd;
                    std::thread compileThread([hMainWnd]() {
                        // 设置编译回调（使用 PostMessage 线程安全地更新 UI）
                        Compiler& compiler = Compiler::GetInstance();
                        compiler.SetCallback([hMainWnd](const CompileMessage& msg) {
                            std::wstring prefix;
                            switch (msg.type) {
                                case CompileMessageType::Error:
                                    prefix = L"[错误] ";
                                    break;
                                case CompileMessageType::Warning:
                                    prefix = L"[警告] ";
                                    break;
                                case CompileMessageType::Success:
                                    prefix = L"[成功] ";
                                    break;
                                default:
                                    prefix = L"";
                                    break;
                            }
                            // 使用 PostMessage 线程安全地更新 UI
                            // 动态分配消息字符串，由主线程负责释放
                            std::wstring* pMsg = new std::wstring(prefix + msg.message + L"\r\n");
                            if (!PostMessage(hMainWnd, WM_COMPILE_OUTPUT, reinterpret_cast<WPARAM>(pMsg), 0)) {
                                delete pMsg;  // 如果发送失败，释放内存
                            }
                        });
                        
                        // 编译项目
                        CompileOptions options;
                        options.debug = true;
                        CompileResult result = compiler.CompileProject(options);
                        
                        // 如果编译成功，运行程序
                        if (result.success) {
                            compiler.RunExecutable(result.outputFile);
                        }
                    });
                    compileThread.detach();  // 分离线程，让它在后台运行
                }
                break;
            
            case ID_BUILD_COMPILE:
                {
                    OutputDebugStringW(L"[Compile] ID_BUILD_COMPILE triggered\n");
                    
                    // 显示输出面板
                    if (!g_OutputPanelVisible) {
                        g_OutputPanelVisible = true;
                        RECT rect;
                        GetClientRect(hWnd, &rect);
                        SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.right, rect.bottom));
                    }
                    
                    // 清空并切换到输出标签
                    if (g_pOutputPanel) {
                        g_pOutputPanel->ClearOutput();
                        g_pOutputPanel->SetActiveTab(OutputTabType::Output);
                    }
                    
                    // 在后台线程中执行编译
                    HWND hMainWnd2 = hWnd;
                    std::thread compileOnlyThread([hMainWnd2]() {
                        // 设置编译回调（使用 PostMessage 线程安全地更新 UI）
                        Compiler& compiler = Compiler::GetInstance();
                        compiler.SetCallback([hMainWnd2](const CompileMessage& msg) {
                            std::wstring prefix;
                            switch (msg.type) {
                                case CompileMessageType::Error:
                                    prefix = L"[错误] ";
                                    break;
                                case CompileMessageType::Warning:
                                    prefix = L"[警告] ";
                                    break;
                                case CompileMessageType::Success:
                                    prefix = L"[成功] ";
                                    break;
                                default:
                                    prefix = L"";
                                    break;
                            }
                            // 使用 PostMessage 线程安全地更新 UI
                            std::wstring* pMsg = new std::wstring(prefix + msg.message + L"\r\n");
                            if (!PostMessage(hMainWnd2, WM_COMPILE_OUTPUT, reinterpret_cast<WPARAM>(pMsg), 0)) {
                                delete pMsg;
                            }
                        });
                        
                        // 仅编译项目
                        CompileOptions options;
                        options.debug = true;
                        compiler.CompileProject(options);
                    });
                    compileOnlyThread.detach();
                }
                break;
            
            case ID_BUILD_DEBUG:
                {
                    OutputDebugStringW(L"[Compile] ID_BUILD_DEBUG triggered\n");
                    // TODO: 实现调试功能
                    MessageBoxW(hWnd, L"调试功能正在开发中...", L"调试", MB_OK | MB_ICONINFORMATION);
                }
                break;
            
            case ID_BUILD_STOP:
                {
                    OutputDebugStringW(L"[Compile] ID_BUILD_STOP triggered\n");
                    Compiler& compiler = Compiler::GetInstance();
                    if (compiler.IsRunning()) {
                        compiler.StopExecutable();
                        if (g_pOutputPanel) {
                            g_pOutputPanel->AppendOutput(L"程序已被用户终止\r\n", false);
                        }
                    } else if (compiler.IsCompiling()) {
                        // TODO: 实现取消编译功能
                        MessageBoxW(hWnd, L"编译过程中无法停止", L"提示", MB_OK | MB_ICONINFORMATION);
                    } else {
                        if (g_pOutputPanel) {
                            g_pOutputPanel->AppendOutput(L"没有正在运行的程序\r\n", false);
                        }
                    }
                }
                break;
            
            default:
                // 处理动态主题菜单项
                if (wmId >= IDM_THEME_FIRST && wmId <= IDM_THEME_LAST) {
                    int themeIndex = wmId - IDM_THEME_FIRST;
                    if (themeIndex >= 0 && themeIndex < (int)g_ThemeList.size()) {
                        // 加载选中的主题
                        SetThemeByFile(g_ThemeList[themeIndex].filename);
                        SaveThemeConfig();
                        
                        // 更新菜单项（重新构建菜单以更新选中标记）
                        UpdateMenuItems();
                        
                        // 销毁旧的菜单弹出窗口，下次点击时会重新创建
                        if (g_MenuPopupWnd) {
                            DestroyWindow(g_MenuPopupWnd);
                            g_MenuPopupWnd = NULL;
                        }
                        
                        // 刷新所有窗口
                        InvalidateRect(hWnd, NULL, TRUE);
                        InvalidateRect(hAIChatWnd, NULL, TRUE);
                        InvalidateRect(hEditorWnd, NULL, TRUE);
                        InvalidateRect(hRightPanelWnd, NULL, TRUE);
                        InvalidateRect(hOutputWnd, NULL, TRUE);
                        if (hTabBarWnd) InvalidateRect(hTabBarWnd, NULL, TRUE);
                        if (hWelcomePageWnd) InvalidateRect(hWelcomePageWnd, NULL, TRUE);
                    }
                    break;
                }
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORLISTBOX:
        {
            HDC hdc = (HDC)wParam;
            HWND hCtrl = (HWND)lParam;
            
            // 状态栏使用主题背景色
            if (hCtrl == hStatusBar) {
                SetTextColor(hdc, g_CurrentTheme.statusBarText);
                SetBkColor(hdc, g_CurrentTheme.statusBarBg);
                return (LRESULT)g_CurrentTheme.hStatusBarBrush;
            }
            
            SetTextColor(hdc, g_CurrentTheme.text);
            SetBkColor(hdc, g_CurrentTheme.bg);
            return (LRESULT)g_CurrentTheme.hBgBrush;
        }
        
    case WM_LBUTTONDOWN:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            
            // 如果菜单正在显示，检查是否点击在菜单窗口外
            if (g_MenuPopupWnd && IsWindowVisible(g_MenuPopupWnd)) {
                POINT pt = {x, y};
                ClientToScreen(hWnd, &pt);
                RECT menuRect;
                GetWindowRect(g_MenuPopupWnd, &menuRect);
                
                // 如果点击不在菜单窗口内，关闭菜单
                if (!PtInRect(&menuRect, pt)) {
                    ReleaseCapture();
                    DestroyWindow(g_MenuPopupWnd);
                    g_MenuPopupWnd = NULL;
                    g_ActiveMenu = -1;
                    InvalidateRect(hWnd, NULL, TRUE);
                    // 继续处理点击事件
                }
                else {
                    // 点击在菜单窗口内，不处理，让菜单窗口自己处理
                    return 0;
                }
            }
            
            // 检查标题栏按钮点击
            if (PtInRect(&g_CloseButtonRect, {x, y})) {
                PostMessage(hWnd, WM_CLOSE, 0, 0);
                return 0;
            }
            if (PtInRect(&g_MaxButtonRect, {x, y})) {
                if (g_IsMaximized) {
                    ShowWindow(hWnd, SW_RESTORE);
                } else {
                    ShowWindow(hWnd, SW_MAXIMIZE);
                }
                return 0;
            }
            if (PtInRect(&g_MinButtonRect, {x, y})) {
                ShowWindow(hWnd, SW_MINIMIZE);
                return 0;
            }
            
            // 检查功能图标按钮点击
            if (PtInRect(&g_ToggleAIButtonRect, {x, y})) {
                // 切换AI面板显示/隐藏
                g_AIPanelVisible = !g_AIPanelVisible;
                // 触发窗口大小调整
                RECT rc;
                GetClientRect(hWnd, &rc);
                SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rc.right, rc.bottom));
                InvalidateRect(hWnd, NULL, TRUE);
                return 0;
            }
            if (PtInRect(&g_SwapPanelsButtonRect, {x, y})) {
                // 交换资源管理器和AI面板的位置
                g_PanelsSwapped = !g_PanelsSwapped;
                // 触发窗口大小调整以重新布局
                RECT rc;
                GetClientRect(hWnd, &rc);
                SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rc.right, rc.bottom));
                
                // 强制AI窗口重新布局其子窗口
                if (hAIChatWnd && IsWindowVisible(hAIChatWnd)) {
                    RECT aiRect;
                    GetClientRect(hAIChatWnd, &aiRect);
                    SendMessage(hAIChatWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(aiRect.right, aiRect.bottom));
                    RedrawWindow(hAIChatWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
                }
                
                InvalidateRect(hWnd, NULL, TRUE);
                return 0;
            }
            if (PtInRect(&g_ToggleOutputButtonRect, {x, y})) {
                // 切换输出面板显示/隐藏
                g_OutputPanelVisible = !g_OutputPanelVisible;
                // TODO: 实际显示/隐藏输出面板
                // 触发窗口大小调整
                RECT rc;
                GetClientRect(hWnd, &rc);
                SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rc.right, rc.bottom));
                InvalidateRect(hWnd, NULL, TRUE);
                return 0;
            }
            if (PtInRect(&g_ToggleSidebarButtonRect, {x, y})) {
                // 切换侧边栏显示/隐藏
                g_ExplorerPanelVisible = !g_ExplorerPanelVisible;
                g_LeftPanelVisible = g_ExplorerPanelVisible;
                // 触发窗口大小调整
                RECT rc;
                GetClientRect(hWnd, &rc);
                SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rc.right, rc.bottom));
                InvalidateRect(hWnd, NULL, TRUE);
                return 0;
            }
            
            // 检查侧边栏切换按钮点击
            POINT ptClick = {x, y};
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            
            // 检查活动栏按钮点击（根据面板交换状态判断位置）
            bool inActivityBarArea = false;
            if (y >= g_TotalTopHeight && y < clientRect.bottom - g_StatusBarHeight) {
                if (g_PanelsSwapped) {
                    // 交换模式：活动栏在右侧
                    inActivityBarArea = (x >= clientRect.right - g_ActivityBarWidth);
                } else {
                    // 正常模式：活动栏在左侧
                    inActivityBarArea = (x < g_ActivityBarWidth);
                }
            }
            
            if (inActivityBarArea) {
                // 检查顶部按钮
                for (const auto& btn : g_ActivityBarButtons) {
                    if (PtInRect(&btn.rect, ptClick)) {
                        // 处理活动栏按钮点击
                        switch (btn.id) {
                            case AB_EXPLORER:
                                // 切换资源管理器面板
                                if (g_ActiveActivityButton == AB_EXPLORER) {
                                    g_LeftPanelVisible = !g_LeftPanelVisible;
                                } else {
                                    g_ActiveActivityButton = AB_EXPLORER;
                                    g_LeftPanelVisible = true;
                                }
                                break;
                            case AB_SEARCH:
                                g_ActiveActivityButton = AB_SEARCH;
                                // TODO: 显示搜索面板
                                MessageBoxW(hWnd, L"搜索功能正在开发中...", L"提示", MB_OK | MB_ICONINFORMATION);
                                break;
                            case AB_SCM:
                                g_ActiveActivityButton = AB_SCM;
                                // TODO: 显示源代码管理面板
                                MessageBoxW(hWnd, L"源代码管理功能正在开发中...", L"提示", MB_OK | MB_ICONINFORMATION);
                                break;
                            case AB_DEBUG:
                                g_ActiveActivityButton = AB_DEBUG;
                                // TODO: 显示调试面板
                                MessageBoxW(hWnd, L"运行和调试功能正在开发中...", L"提示", MB_OK | MB_ICONINFORMATION);
                                break;
                            case AB_EXTENSIONS:
                                g_ActiveActivityButton = AB_EXTENSIONS;
                                // TODO: 显示扩展面板
                                MessageBoxW(hWnd, L"扩展功能正在开发中...", L"提示", MB_OK | MB_ICONINFORMATION);
                                break;
                            // AB_AI 已移到标题栏按钮
                        }
                        
                        // 更新按钮选中状态
                        for (auto& b : g_ActivityBarButtons) {
                            b.selected = (b.id == g_ActiveActivityButton);
                        }
                        
                        // 触发窗口重新布局
                        RECT rect;
                        GetClientRect(hWnd, &rect);
                        SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.right, rect.bottom));
                        InvalidateRect(hWnd, NULL, TRUE);
                        return 0;
                    }
                }
                
                // 检查底部按钮
                for (const auto& btn : g_ActivityBarBottomButtons) {
                    if (PtInRect(&btn.rect, ptClick)) {
                        switch (btn.id) {
                            case AB_ACCOUNT:
                                MessageBoxW(hWnd, L"账号功能正在开发中...", L"提示", MB_OK | MB_ICONINFORMATION);
                                break;
                            case AB_SETTINGS:
                                // 打开设置
                                SendMessage(hWnd, WM_COMMAND, IDM_SYSTEM_CONFIG, 0);
                                break;
                        }
                        return 0;
                    }
                }
            }
            
            // 检查工具栏按钮点击
            if (y >= g_TitleBarHeight && y < g_TitleBarHeight + g_ToolBarHeight) {
                for (size_t i = 0; i < g_ToolBarButtons.size(); i++) {
                    if (PtInRect(&g_ToolBarButtons[i].rect, {x, y})) {
                        // 执行工具栏按钮对应的操作
                        switch (g_ToolBarButtons[i].id) {
                            case TB_NEW_FILE:
                                SendMessage(hWnd, WM_COMMAND, ID_FILE_NEW, 0);
                                break;
                            case TB_OPEN_FILE:
                                SendMessage(hWnd, WM_COMMAND, ID_FILE_OPEN, 0);
                                break;
                            case TB_SAVE_FILE:
                                SendMessage(hWnd, WM_COMMAND, ID_FILE_SAVE, 0);
                                break;
                            case TB_SAVE_ALL:
                                SendMessage(hWnd, WM_COMMAND, ID_FILE_SAVEALL, 0);
                                break;
                            case TB_UNDO:
                                SendMessage(hWnd, WM_COMMAND, ID_EDIT_UNDO, 0);
                                break;
                            case TB_REDO:
                                SendMessage(hWnd, WM_COMMAND, ID_EDIT_REDO, 0);
                                break;
                            case TB_CUT:
                                SendMessage(hWnd, WM_COMMAND, ID_EDIT_CUT, 0);
                                break;
                            case TB_COPY:
                                SendMessage(hWnd, WM_COMMAND, ID_EDIT_COPY, 0);
                                break;
                            case TB_PASTE:
                                SendMessage(hWnd, WM_COMMAND, ID_EDIT_PASTE, 0);
                                break;
                            case TB_RUN:
                                SendMessage(hWnd, WM_COMMAND, ID_BUILD_RUN, 0);
                                break;
                            case TB_DEBUG:
                                SendMessage(hWnd, WM_COMMAND, ID_BUILD_DEBUG, 0);
                                break;
                            case TB_STOP:
                                SendMessage(hWnd, WM_COMMAND, ID_BUILD_STOP, 0);
                                break;
                            case TB_BUILD:
                                SendMessage(hWnd, WM_COMMAND, ID_BUILD_COMPILE, 0);
                                break;
                        }
                        return 0;
                    }
                }
            }
            
            // 检查菜单点击（只在标题栏区域）
            if (y < g_TitleBarHeight) {
                for (size_t i = 0; i < g_MenuItems.size(); i++) {
                    if (PtInRect(&g_MenuItems[i].rect, {x, y})) {
                        if (g_ActiveMenu == (int)i) {
                            // 关闭菜单
                            ReleaseCapture();
                            g_ActiveMenu = -1;
                            if (g_MenuPopupWnd) {
                                ShowWindow(g_MenuPopupWnd, SW_HIDE);
                            }
                        } else {
                            // 打开菜单
                            g_ActiveMenu = (int)i;
                            
                            // 创建弹出窗口
                            int itemCount = (int)g_MenuItems[i].subItems.size();
                            int menuHeight = itemCount * 28 + 6;
                            
                            RECT menuRect = g_MenuItems[i].rect;
                            POINT pt = {menuRect.left, menuRect.bottom};
                            ClientToScreen(hWnd, &pt);
                            
                            g_MenuPopupWnd = CreateWindowExW(
                                WS_EX_TOOLWINDOW,
                                L"MenuPopupClass", nullptr,
                                WS_POPUP,
                                pt.x, pt.y, 200, menuHeight,
                                hWnd, nullptr, hInst, nullptr);
                            
                            if (g_MenuPopupWnd) {
                                ShowWindow(g_MenuPopupWnd, SW_SHOWNOACTIVATE);
                                // 捕获鼠标，确保菜单能接收所有鼠标事件
                                SetCapture(g_MenuPopupWnd);
                            }
                        }
                        InvalidateRect(hWnd, NULL, TRUE);
                        return 0;
                    }
                }
            }
            
            // 点击其他区域关闭菜单
            if (g_ActiveMenu >= 0) {
                g_ActiveMenu = -1;
                if (g_MenuPopupWnd) {
                    DestroyWindow(g_MenuPopupWnd);
                    g_MenuPopupWnd = NULL;
                }
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        return 0;
        
    case WM_LBUTTONUP:
        return 0;
        return 0;
        
    case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            
            // 追踪鼠标离开事件
            if (!g_IsTrackingMouse) {
                TRACKMOUSEEVENT tme = {};
                tme.cbSize = sizeof(TRACKMOUSEEVENT);
                tme.dwFlags = TME_LEAVE;
                tme.hwndTrack = hWnd;
                TrackMouseEvent(&tme);
                g_IsTrackingMouse = true;
            }
            
            // 检查菜单项悬停
            bool menuHoverChanged = false;
            if (y < g_TitleBarHeight) {
                for (size_t i = 0; i < g_MenuItems.size(); i++) {
                    bool wasHovered = g_MenuItems[i].hovered;
                    g_MenuItems[i].hovered = PtInRect(&g_MenuItems[i].rect, {x, y});
                    if (wasHovered != g_MenuItems[i].hovered) {
                        menuHoverChanged = true;
                    }
                }
            } else {
                // 鼠标离开标题栏，清除所有悬停状态
                for (size_t i = 0; i < g_MenuItems.size(); i++) {
                    if (g_MenuItems[i].hovered) {
                        g_MenuItems[i].hovered = false;
                        menuHoverChanged = true;
                    }
                }
            }
            
            if (menuHoverChanged) {
                RECT titleRect = {0, 0, 0, g_TitleBarHeight};
                GetClientRect(hWnd, &titleRect);
                titleRect.bottom = g_TitleBarHeight;
                InvalidateRect(hWnd, &titleRect, FALSE);
            }
            
            // 检查按钮悬停（只在标题栏区域）
            int oldHover = g_HoverButton;
            g_HoverButton = 0;
            if (y < g_TitleBarHeight) {
                POINT ptHover = {x, y};
                if (PtInRect(&g_CloseButtonRect, ptHover)) g_HoverButton = 3;
                else if (PtInRect(&g_MaxButtonRect, ptHover)) g_HoverButton = 2;
                else if (PtInRect(&g_MinButtonRect, ptHover)) g_HoverButton = 1;
                else if (PtInRect(&g_ToggleAIButtonRect, ptHover)) g_HoverButton = 4;
                else if (PtInRect(&g_SwapPanelsButtonRect, ptHover)) g_HoverButton = 5;
                else if (PtInRect(&g_ToggleOutputButtonRect, ptHover)) g_HoverButton = 6;
                else if (PtInRect(&g_ToggleSidebarButtonRect, ptHover)) g_HoverButton = 7;
            }
            
            // 只在悬停状态改变时才重绘
            if (oldHover != g_HoverButton) {
                // 只重绘标题栏区域
                RECT titleRect = {0, 0, 0, g_TitleBarHeight};
                GetClientRect(hWnd, &titleRect);
                titleRect.bottom = g_TitleBarHeight;
                InvalidateRect(hWnd, &titleRect, FALSE);
            }
            
            // 检查工具栏按钮悬停
            int oldToolBarHover = g_HoverToolBarButton;
            g_HoverToolBarButton = -1;
            if (y >= g_TitleBarHeight && y < g_TitleBarHeight + g_ToolBarHeight) {
                POINT pt = {x, y};
                for (size_t i = 0; i < g_ToolBarButtons.size(); i++) {
                    if (PtInRect(&g_ToolBarButtons[i].rect, pt)) {
                        g_HoverToolBarButton = (int)i;
                        break;
                    }
                }
            }
            
            if (oldToolBarHover != g_HoverToolBarButton) {
                // 重绘工具栏区域
                RECT toolBarRect = {0, g_TitleBarHeight, 0, g_TitleBarHeight + g_ToolBarHeight};
                GetClientRect(hWnd, &toolBarRect);
                toolBarRect.top = g_TitleBarHeight;
                toolBarRect.bottom = g_TitleBarHeight + g_ToolBarHeight;
                InvalidateRect(hWnd, &toolBarRect, FALSE);
            }
            
            // 检查活动栏按钮悬停（根据面板交换状态判断位置）
            int oldActivityHover = g_HoverActivityButton;
            g_HoverActivityButton = -1;
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            bool inActivityBarHoverArea = false;
            if (y >= g_TotalTopHeight && y < rect.bottom - g_StatusBarHeight) {
                if (g_PanelsSwapped) {
                    // 交换模式：活动栏在右侧
                    inActivityBarHoverArea = (x >= rect.right - g_ActivityBarWidth);
                } else {
                    // 正常模式：活动栏在左侧
                    inActivityBarHoverArea = (x < g_ActivityBarWidth);
                }
            }
            
            if (inActivityBarHoverArea) {
                POINT pt = {x, y};
                // 检查顶部按钮
                for (const auto& btn : g_ActivityBarButtons) {
                    if (PtInRect(&btn.rect, pt)) {
                        g_HoverActivityButton = btn.id;
                        break;
                    }
                }
                // 检查底部按钮
                if (g_HoverActivityButton == -1) {
                    for (const auto& btn : g_ActivityBarBottomButtons) {
                        if (PtInRect(&btn.rect, pt)) {
                            g_HoverActivityButton = btn.id;
                            break;
                        }
                    }
                }
            }
            
            if (oldActivityHover != g_HoverActivityButton) {
                // 重绘活动栏区域（根据位置计算）
                int activityBarX = g_PanelsSwapped ? (rect.right - g_ActivityBarWidth) : 0;
                RECT activityRect = {activityBarX, g_TotalTopHeight, activityBarX + g_ActivityBarWidth, rect.bottom - g_StatusBarHeight};
                InvalidateRect(hWnd, &activityRect, FALSE);
            }
        }
        return 0;
        
    case WM_MOUSELEAVE:
        {
            // 鼠标离开窗口，清除所有悬停状态
            g_IsTrackingMouse = false;
            
            bool needRepaint = false;
            bool needToolBarRepaint = false;
            bool needActivityBarRepaint = false;
            
            // 清除按钮悬停状态
            if (g_HoverButton != 0) {
                g_HoverButton = 0;
                needRepaint = true;
            }
            
            // 清除菜单项悬停状态
            for (size_t i = 0; i < g_MenuItems.size(); i++) {
                if (g_MenuItems[i].hovered) {
                    g_MenuItems[i].hovered = false;
                    needRepaint = true;
                }
            }
            
            // 清除工具栏按钮悬停状态
            if (g_HoverToolBarButton != -1) {
                g_HoverToolBarButton = -1;
                needToolBarRepaint = true;
            }
            
            // 清除活动栏按钮悬停状态
            if (g_HoverActivityButton != -1) {
                g_HoverActivityButton = -1;
                needActivityBarRepaint = true;
            }
            
            if (needRepaint) {
                RECT titleRect = {0, 0, 0, g_TitleBarHeight};
                GetClientRect(hWnd, &titleRect);
                titleRect.bottom = g_TitleBarHeight;
                InvalidateRect(hWnd, &titleRect, FALSE);
            }
            
            if (needToolBarRepaint) {
                RECT toolBarRect = {0, g_TitleBarHeight, 0, g_TitleBarHeight + g_ToolBarHeight};
                GetClientRect(hWnd, &toolBarRect);
                toolBarRect.top = g_TitleBarHeight;
                toolBarRect.bottom = g_TitleBarHeight + g_ToolBarHeight;
                InvalidateRect(hWnd, &toolBarRect, FALSE);
            }
            
            if (needActivityBarRepaint) {
                RECT rect;
                GetClientRect(hWnd, &rect);
                // 根据面板交换状态计算活动栏位置
                int activityBarX = g_PanelsSwapped ? (rect.right - g_ActivityBarWidth) : 0;
                RECT activityRect = {activityBarX, g_TotalTopHeight, activityBarX + g_ActivityBarWidth, rect.bottom - g_StatusBarHeight};
                InvalidateRect(hWnd, &activityRect, FALSE);
            }
        }
        return 0;
        
    case WM_NCCALCSIZE:
        // 返回0表示整个窗口都是客户区
        if (wParam == TRUE) {
            return 0;
        }
        break;
    
    case WM_NCACTIVATE:
        // 阻止系统绘制非客户区边框（灰色/白色边框）
        // 返回TRUE表示允许窗口激活状态改变，但不绘制默认边框
        // 窗口失去焦点时关闭菜单
        if (!wParam) {
            if (g_MenuPopupWnd && IsWindowVisible(g_MenuPopupWnd)) {
                ShowWindow(g_MenuPopupWnd, SW_HIDE);
                g_ActiveMenu = -1;
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        return TRUE;
    
    case WM_NCPAINT:
        // 阻止系统绘制非客户区
        return 0;
    
    case WM_MOVE:
        {
            // 窗口移动时关闭菜单
            if (g_MenuPopupWnd) {
                DestroyWindow(g_MenuPopupWnd);
                g_MenuPopupWnd = NULL;
                g_ActiveMenu = -1;
                InvalidateRect(hWnd, NULL, TRUE);
            }
            // 注意：组件箱现在是可视化设计器的子窗口，会自动跟随父窗口移动，无需手动处理
        }
        break;
        
    case WM_ENTERSIZEMOVE:
        // 窗口移动时关闭菜单
        if (g_MenuPopupWnd) {
            DestroyWindow(g_MenuPopupWnd);
            g_MenuPopupWnd = NULL;
            g_ActiveMenu = -1;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
        
    case WM_NCHITTEST:
        {
            LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(hWnd, &pt);
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            // 边框调整大小（优先级最高）
            const int borderSize = 5;
            // 左右边缘始终可以调整窗口大小
            bool onLeft = pt.x < borderSize;
            bool onRight = pt.x > rect.right - borderSize;
            bool onTop = pt.y < borderSize;
            bool onBottom = pt.y > rect.bottom - borderSize;
            
            if (onTop && onLeft) return HTTOPLEFT;
            if (onTop && onRight) return HTTOPRIGHT;
            if (onBottom && onLeft) return HTBOTTOMLEFT;
            if (onBottom && onRight) return HTBOTTOMRIGHT;
            if (onTop) return HTTOP;
            if (onBottom) return HTBOTTOM;
            if (onLeft) return HTLEFT;
            if (onRight) return HTRIGHT;
            
            // 检查是否在菜单区域
            bool inMenuArea = false;
            if (pt.y < g_TitleBarHeight) {
                for (size_t i = 0; i < g_MenuItems.size(); i++) {
                    if (PtInRect(&g_MenuItems[i].rect, pt)) {
                        inMenuArea = true;
                        break;
                    }
                }
            }
            
            // 检查是否在功能图标按钮区域（AI、切换、输出、侧边栏）
            bool inIconButtons = false;
            if (pt.y < g_TitleBarHeight) {
                if (PtInRect(&g_ToggleAIButtonRect, pt) ||
                    PtInRect(&g_SwapPanelsButtonRect, pt) ||
                    PtInRect(&g_ToggleOutputButtonRect, pt) ||
                    PtInRect(&g_ToggleSidebarButtonRect, pt)) {
                    inIconButtons = true;
                }
            }
            
            // 检查是否在活动栏区域（根据面板交换状态判断位置）
            bool inActivityBar = false;
            if (pt.y >= g_TotalTopHeight) {
                if (g_PanelsSwapped) {
                    // 交换模式：活动栏在右侧
                    inActivityBar = (pt.x >= rect.right - g_ActivityBarWidth);
                } else {
                    // 正常模式：活动栏在左侧
                    inActivityBar = (pt.x < g_ActivityBarWidth);
                }
            }
            
            // 标题栏区域可以拖动窗口（但排除菜单区域、功能图标和右侧按钮区域）
            if (!inMenuArea && !inIconButtons && pt.y < g_TitleBarHeight && pt.x < rect.right - 46 * 3) {
                return HTCAPTION;
            }
            
            // 活动栏区域不能拖动窗口
            if (inActivityBar) {
                return HTCLIENT;
            }
            
            return HTCLIENT;
        }
    
    case WM_SETCURSOR:
        {
            // 根据WM_NCHITTEST的结果设置光标
            WORD hitTest = LOWORD(lParam);
            switch (hitTest) {
            case HTTOP:
            case HTBOTTOM:
                SetCursor(LoadCursor(NULL, IDC_SIZENS));
                return TRUE;
            case HTLEFT:
            case HTRIGHT:
                SetCursor(LoadCursor(NULL, IDC_SIZEWE));
                return TRUE;
            case HTTOPLEFT:
            case HTBOTTOMRIGHT:
                SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
                return TRUE;
            case HTTOPRIGHT:
            case HTBOTTOMLEFT:
                SetCursor(LoadCursor(NULL, IDC_SIZENESW));
                return TRUE;
            }
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            // 创建双缓冲
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
            
            // === 绘制标题栏 ===
            RECT titleBarRect = {0, 0, rect.right, g_TitleBarHeight};
            HBRUSH titleBrush = CreateSolidBrush(g_CurrentTheme.titleBarBg);
            FillRect(memDC, &titleBarRect, titleBrush);
            DeleteObject(titleBrush);
            
            // 绘制标题栏底部1像素边框
            RECT borderRect = {0, g_TitleBarHeight - 1, rect.right, g_TitleBarHeight};
            HBRUSH borderBrush = CreateSolidBrush(g_CurrentTheme.border);
            FillRect(memDC, &borderRect, borderBrush);
            DeleteObject(borderBrush);
            
            // 绘制标题文本
            SetBkMode(memDC, TRANSPARENT);
            SetTextColor(memDC, g_CurrentTheme.text);
            HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
            HFONT oldFont = (HFONT)SelectObject(memDC, hFont);
            
            RECT titleTextRect = {10, 0, 200, g_TitleBarHeight};
            DrawTextW(memDC, szTitle, -1, &titleTextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            
            // 绘制菜单项
            Graphics graphics(memDC);
            graphics.SetSmoothingMode(SmoothingModeAntiAlias);
            
            for (size_t i = 0; i < g_MenuItems.size(); i++) {
                RECT menuRect = g_MenuItems[i].rect;
                
                // 绘制悬停或激活状态的圆角矩形背景
                if (g_MenuItems[i].hovered || g_ActiveMenu == (int)i) {
                    Color hoverColor(255, 45, 46, 46);
                    SolidBrush hoverBrush(hoverColor);
                    
                    // 绘制圆角矩形
                    RectF rectF((REAL)menuRect.left, (REAL)menuRect.top, 
                        (REAL)(menuRect.right - menuRect.left), 
                        (REAL)(menuRect.bottom - menuRect.top));
                    GraphicsPath path;
                    float radius = 4.0f;
                    path.AddArc(rectF.X, rectF.Y, radius * 2, radius * 2, 180, 90);
                    path.AddArc(rectF.X + rectF.Width - radius * 2, rectF.Y, radius * 2, radius * 2, 270, 90);
                    path.AddArc(rectF.X + rectF.Width - radius * 2, rectF.Y + rectF.Height - radius * 2, radius * 2, radius * 2, 0, 90);
                    path.AddArc(rectF.X, rectF.Y + rectF.Height - radius * 2, radius * 2, radius * 2, 90, 90);
                    path.CloseFigure();
                    graphics.FillPath(&hoverBrush, &path);
                }
                
                DrawTextW(memDC, g_MenuItems[i].text.c_str(), -1, &g_MenuItems[i].rect, 
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            
            // 绘制标题栏按钮
            auto DrawButton = [&](RECT rc, const wchar_t* symbol, int btnId) {
                if (g_HoverButton == btnId) {
                    COLORREF hoverColor = (btnId == 3) ? g_CurrentTheme.closeButtonHover : g_CurrentTheme.menuHoverBg;
                    HBRUSH hoverBrush = CreateSolidBrush(hoverColor);
                    FillRect(memDC, &rc, hoverBrush);
                    DeleteObject(hoverBrush);
                }
                DrawTextW(memDC, symbol, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            };
            
            DrawButton(g_MinButtonRect, L"—", 1);
            DrawButton(g_MaxButtonRect, g_IsMaximized ? L"❐" : L"□", 2);
            DrawButton(g_CloseButtonRect, L"✕", 3);
            
            // 绘制功能图标按钮（使用 Segoe Fluent Icons）
            auto DrawIconButton = [&](RECT rc, const wchar_t* iconChar, int btnId, bool isActive = false) {
                // 悬停背景
                if (g_HoverButton == btnId) {
                    Color hoverColor(255, GetRValue(g_CurrentTheme.menuHoverBg), 
                                    GetGValue(g_CurrentTheme.menuHoverBg), 
                                    GetBValue(g_CurrentTheme.menuHoverBg));
                    SolidBrush hoverBrush(hoverColor);
                    RectF rectF((REAL)rc.left + 2, (REAL)rc.top + 4, 
                               (REAL)(rc.right - rc.left - 4), (REAL)(rc.bottom - rc.top - 8));
                    GraphicsPath path;
                    float radius = 4.0f;
                    path.AddArc(rectF.X, rectF.Y, radius * 2, radius * 2, 180, 90);
                    path.AddArc(rectF.X + rectF.Width - radius * 2, rectF.Y, radius * 2, radius * 2, 270, 90);
                    path.AddArc(rectF.X + rectF.Width - radius * 2, rectF.Y + rectF.Height - radius * 2, radius * 2, radius * 2, 0, 90);
                    path.AddArc(rectF.X, rectF.Y + rectF.Height - radius * 2, radius * 2, radius * 2, 90, 90);
                    path.CloseFigure();
                    graphics.FillPath(&hoverBrush, &path);
                }
                
                // 绘制图标
                HFONT iconFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe Fluent Icons");
                HFONT oldIconFont = (HFONT)SelectObject(memDC, iconFont);
                
                // 根据激活状态设置颜色
                if (isActive) {
                    SetTextColor(memDC, g_CurrentTheme.activityBarIndicator);
                } else if (g_HoverButton == btnId) {
                    SetTextColor(memDC, g_CurrentTheme.activityBarIconHover);
                } else {
                    SetTextColor(memDC, g_CurrentTheme.activityBarIcon);
                }
                
                DrawTextW(memDC, iconChar, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                
                SelectObject(memDC, oldIconFont);
                DeleteObject(iconFont);
            };
            
            // AI助手按钮 (使用 AI/机器人图标)
            DrawIconButton(g_ToggleAIButtonRect, L"\uE8D4", 4, g_AIPanelVisible);
            // 左右面板切换按钮 (使用双向箭头图标)
            DrawIconButton(g_SwapPanelsButtonRect, L"\uE8AB", 5, g_PanelsSwapped);
            // 输出面板按钮 (使用终端图标)
            DrawIconButton(g_ToggleOutputButtonRect, L"\uE756", 6, g_OutputPanelVisible);
            // 侧边栏按钮 (使用面板图标)
            DrawIconButton(g_ToggleSidebarButtonRect, L"\uE700", 7, g_ExplorerPanelVisible);
            
            // === 绘制工具栏 ===
            {
                // 工具栏背景
                RECT toolBarRect = {0, g_TitleBarHeight, rect.right, g_TitleBarHeight + g_ToolBarHeight};
                HBRUSH toolBarBrush = CreateSolidBrush(g_CurrentTheme.toolBarBg);
                FillRect(memDC, &toolBarRect, toolBarBrush);
                DeleteObject(toolBarBrush);
                
                // 工具栏底部边框
                RECT toolBarBorder = {0, g_TitleBarHeight + g_ToolBarHeight - 1, rect.right, g_TitleBarHeight + g_ToolBarHeight};
                HBRUSH toolBarBorderBrush = CreateSolidBrush(g_CurrentTheme.border);
                FillRect(memDC, &toolBarBorder, toolBarBorderBrush);
                DeleteObject(toolBarBorderBrush);
                
                // 绘制工具栏按钮
                for (size_t i = 0; i < g_ToolBarButtons.size(); i++) {
                    const auto& btn = g_ToolBarButtons[i];
                    
                    // 悬停背景
                    if (g_HoverToolBarButton == (int)i) {
                        Color hoverColor(255, 55, 55, 55);
                        SolidBrush hoverBrush(hoverColor);
                        RectF rectF((REAL)btn.rect.left, (REAL)btn.rect.top,
                            (REAL)(btn.rect.right - btn.rect.left),
                            (REAL)(btn.rect.bottom - btn.rect.top));
                        GraphicsPath path;
                        float radius = 4.0f;
                        path.AddArc(rectF.X, rectF.Y, radius * 2, radius * 2, 180, 90);
                        path.AddArc(rectF.X + rectF.Width - radius * 2, rectF.Y, radius * 2, radius * 2, 270, 90);
                        path.AddArc(rectF.X + rectF.Width - radius * 2, rectF.Y + rectF.Height - radius * 2, radius * 2, radius * 2, 0, 90);
                        path.AddArc(rectF.X, rectF.Y + rectF.Height - radius * 2, radius * 2, radius * 2, 90, 90);
                        path.CloseFigure();
                        graphics.FillPath(&hoverBrush, &path);
                    }
                    
                    // 绘制图标
                    if (g_ToolBarImage && g_ToolBarImage->GetLastStatus() == Ok && btn.iconIndex >= 0) {
                        int srcX = btn.iconIndex * g_ToolBarIconSize;
                        int destX = btn.rect.left + (g_ToolBarButtonSize - g_ToolBarIconSize) / 2;
                        int destY = btn.rect.top + (g_ToolBarButtonSize - g_ToolBarIconSize) / 2;
                        
                        // 如果按钮禁用，降低透明度
                        if (!btn.enabled) {
                            ColorMatrix colorMatrix = {
                                1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.3f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f, 1.0f
                            };
                            ImageAttributes imgAttr;
                            imgAttr.SetColorMatrix(&colorMatrix);
                            graphics.DrawImage(g_ToolBarImage,
                                RectF((REAL)destX, (REAL)destY, (REAL)g_ToolBarIconSize, (REAL)g_ToolBarIconSize),
                                (REAL)srcX, 0, (REAL)g_ToolBarIconSize, (REAL)g_ToolBarIconSize,
                                UnitPixel, &imgAttr);
                        } else {
                            graphics.DrawImage(g_ToolBarImage, destX, destY, 
                                srcX, 0, g_ToolBarIconSize, g_ToolBarIconSize, UnitPixel);
                        }
                    } else {
                        // 没有图标时绘制占位符文本
                        SetTextColor(memDC, g_CurrentTheme.textDim);
                        DrawTextW(memDC, L"?", -1, (LPRECT)&btn.rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    }
                }
            }
            
            // === 绘制活动栏 (VS Code风格) ===
            {
                // 计算活动栏X位置（根据面板交换状态）
                int activityBarX = g_PanelsSwapped ? (rect.right - g_ActivityBarWidth) : 0;
                
                // 活动栏背景（与主背景色一致）
                RECT activityBarRect = {activityBarX, g_TotalTopHeight, activityBarX + g_ActivityBarWidth, rect.bottom - g_StatusBarHeight};
                HBRUSH activityBarBrush = CreateSolidBrush(g_CurrentTheme.activityBarBg);
                FillRect(memDC, &activityBarRect, activityBarBrush);
                DeleteObject(activityBarBrush);
                
                // 活动栏边框（正常模式在右侧，交换模式在左侧）
                HPEN activityBorderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.border);
                HPEN oldActPen = (HPEN)SelectObject(memDC, activityBorderPen);
                int borderX = g_PanelsSwapped ? activityBarX : (activityBarX + g_ActivityBarWidth - 1);
                MoveToEx(memDC, borderX, g_TotalTopHeight, NULL);
                LineTo(memDC, borderX, rect.bottom - g_StatusBarHeight);
                SelectObject(memDC, oldActPen);
                DeleteObject(activityBorderPen);
                
                // 绘制活动栏图标的辅助函数
                auto DrawActivityIcon = [&](const RECT& btnRect, const wchar_t* iconChar, bool isSelected, bool isHovered) {
                    // 选中状态的指示条（正常模式在左侧，交换模式在右侧）
                    if (isSelected) {
                        int indicatorX = g_PanelsSwapped ? (btnRect.right - 2) : btnRect.left;
                        RECT indicatorRect = {indicatorX, btnRect.top + 8, indicatorX + 2, btnRect.bottom - 8};
                        HBRUSH indicatorBrush = CreateSolidBrush(g_CurrentTheme.activityBarIndicator);
                        FillRect(memDC, &indicatorRect, indicatorBrush);
                        DeleteObject(indicatorBrush);
                    }
                    
                    // 悬停时不再绘制背景色，只通过图标前景色变化来指示悬停状态
                    
                    // 绘制图标（使用Segoe Fluent Icons或文本替代）
                    HFONT iconFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe Fluent Icons");
                    HFONT oldIconFont = (HFONT)SelectObject(memDC, iconFont);
                    
                    // 根据选中/悬停状态设置颜色
                    if (isSelected) {
                        SetTextColor(memDC, g_CurrentTheme.activityBarIconActive);
                    } else if (isHovered) {
                        SetTextColor(memDC, g_CurrentTheme.activityBarIconHover);
                    } else {
                        SetTextColor(memDC, g_CurrentTheme.activityBarIcon);
                    }
                    
                    DrawTextW(memDC, iconChar, -1, (LPRECT)&btnRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                    
                    SelectObject(memDC, oldIconFont);
                    DeleteObject(iconFont);
                };
                
                // 绘制顶部按钮
                for (const auto& btn : g_ActivityBarButtons) {
                    bool isHovered = (g_HoverActivityButton == btn.id);
                    bool isSelected = (g_ActiveActivityButton == btn.id);
                    
                    // 根据按钮ID选择图标
                    const wchar_t* iconChar = L"?";
                    switch (btn.id) {
                        case AB_EXPLORER:   iconChar = L"\uE8B7"; break;  // 文件图标
                        case AB_SEARCH:     iconChar = L"\uE721"; break;  // 搜索图标
                        case AB_SCM:        iconChar = L"\uE943"; break;  // 分支图标
                        case AB_DEBUG:      iconChar = L"\uEBE8"; break;  // 播放调试图标
                        case AB_EXTENSIONS: iconChar = L"\uE74C"; break;  // 扩展图标
                        case AB_AI:         iconChar = L"\uE8D4"; break;  // AI/机器人图标
                    }
                    
                    DrawActivityIcon(btn.rect, iconChar, isSelected, isHovered);
                }
                
                // 绘制底部按钮
                for (const auto& btn : g_ActivityBarBottomButtons) {
                    bool isHovered = (g_HoverActivityButton == btn.id);
                    
                    const wchar_t* iconChar = L"?";
                    switch (btn.id) {
                        case AB_ACCOUNT:    iconChar = L"\uE77B"; break;  // 用户图标
                        case AB_SETTINGS:   iconChar = L"\uE713"; break;  // 设置图标
                    }
                    
                    DrawActivityIcon(btn.rect, iconChar, false, isHovered);
                }
            }
            
            // 绘制编辑器/欢迎页与AI面板之间的边框
            if (g_AIPanelVisible) {
                HPEN editorBorderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.border);
                HPEN oldEditorBorderPen = (HPEN)SelectObject(memDC, editorBorderPen);
                
                if (g_PanelsSwapped) {
                    // 交换模式：AI在左侧，绘制编辑器左边框
                    int leftPanelW = g_RightPanelWidth;  // 交换模式下左侧是AI面板
                    MoveToEx(memDC, leftPanelW, g_TotalTopHeight, NULL);
                    LineTo(memDC, leftPanelW, rect.bottom - g_StatusBarHeight);
                } else {
                    // 正常模式：AI在右侧，绘制编辑器右边框
                    int rightPanelLeft = rect.right - g_RightPanelWidth;
                    MoveToEx(memDC, rightPanelLeft, g_TotalTopHeight, NULL);
                    LineTo(memDC, rightPanelLeft, rect.bottom - g_StatusBarHeight);
                }
                
                SelectObject(memDC, oldEditorBorderPen);
                DeleteObject(editorBorderPen);
            }
            
            // 绘制状态栏顶部分隔线
            HPEN statusSepPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.border);
            HPEN oldStatusPen = (HPEN)SelectObject(memDC, statusSepPen);
            int statusBarY = rect.bottom - g_StatusBarHeight;
            MoveToEx(memDC, 0, statusBarY, NULL);
            LineTo(memDC, rect.right, statusBarY);
            SelectObject(memDC, oldStatusPen);
            DeleteObject(statusSepPen);
            
            // 绘制窗口边框（1像素灰色）
            HPEN borderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.windowBorder);
            HPEN oldBorderPen = (HPEN)SelectObject(memDC, borderPen);
            HBRUSH hOldBrush = (HBRUSH)SelectObject(memDC, GetStockObject(NULL_BRUSH));
            Rectangle(memDC, 0, 0, rect.right, rect.bottom);
            SelectObject(memDC, hOldBrush);
            SelectObject(memDC, oldBorderPen);
            DeleteObject(borderPen);
            
            // 复制到屏幕
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
            
            SelectObject(memDC, oldFont);
            DeleteObject(hFont);
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_CLOSE:
        {
            // 保存组件箱位置
            SaveToolboxPosition();
            
            // 保存面板布局状态
            SavePanelLayoutState();
            
            // 统一的保存文档函数
            auto SaveDocument = [](HWND hMainWnd, EditorDocument* doc) -> bool {
                if (!doc || doc->filePath.empty()) {
                    return false;
                }
                
                if (doc->fileType == FILE_TYPE_ELL) {
                    // .ell 文件通过 EllEditorData 保存
                    extern HWND hEllEditorWnd;
                    EllEditorData* ellData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                    if (ellData) {
                        // 先加载要保存的文件到编辑器
                        if (ellData->currentFilePath != doc->filePath) {
                            ellData->LoadFile(doc->filePath);
                        }
                        if (ellData->SaveFile()) {
                            doc->modified = false;
                            return true;
                        }
                    }
                    return false;
                } else {
                    // .eyc 文件使用普通 SaveFile
                    extern bool SaveFile(const std::wstring& filePath, EditorDocument* doc);
                    if (SaveFile(doc->filePath, doc)) {
                        doc->modified = false;
                        return true;
                    }
                    return false;
                }
            };
            
            // 检查是否有未保存的文件
            bool hasUnsavedFiles = false;
            std::vector<std::wstring> unsavedFileNames;
            
            extern EditorData* g_EditorData;
            if (g_EditorData) {
                for (auto doc : g_EditorData->documents) {
                    if (doc && doc->modified) {
                        hasUnsavedFiles = true;
                        // 获取文件名
                        std::wstring fileName = doc->fileName;
                        if (fileName.empty()) {
                            fileName = L"未命名";
                        }
                        unsavedFileNames.push_back(fileName);
                    }
                }
            }
            
            if (hasUnsavedFiles) {
                // 构建提示消息
                std::wstring message = L"以下文件有未保存的更改：\n\n";
                for (const auto& name : unsavedFileNames) {
                    message += L"  • " + name + L"\n";
                }
                message += L"\n是否保存这些更改？";
                
                // 弹出对话框
                int result = MessageBoxW(hWnd, message.c_str(), L"未保存的更改", 
                                        MB_YESNOCANCEL | MB_ICONWARNING);
                
                if (result == IDYES) {
                    // 保存所有文件
                    for (size_t i = 0; i < g_EditorData->documents.size(); i++) {
                        auto doc = g_EditorData->documents[i];
                        if (doc && doc->modified) {
                            // 如果是未命名文件，需要弹出另存为对话框
                            if (doc->filePath.empty()) {
                                OPENFILENAMEW ofn = {0};
                                WCHAR szFile[MAX_PATH] = {0};
                                
                                ofn.lStructSize = sizeof(ofn);
                                ofn.hwndOwner = hWnd;
                                ofn.lpstrFile = szFile;
                                ofn.nMaxFile = MAX_PATH;
                                ofn.lpstrFilter = L"易语言源文件 (*.eyc)\0*.eyc\0DLL声明文件 (*.ell)\0*.ell\0所有文件 (*.*)\0*.*\0";
                                ofn.nFilterIndex = 1;
                                ofn.lpstrFileTitle = NULL;
                                ofn.nMaxFileTitle = 0;
                                ofn.lpstrInitialDir = NULL;
                                ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
                                
                                if (GetSaveFileNameW(&ofn)) {
                                    doc->filePath = szFile;
                                    // 更新文件名
                                    size_t lastSlash = doc->filePath.find_last_of(L"\\/");
                                    doc->fileName = (lastSlash != std::wstring::npos) ? 
                                                   doc->filePath.substr(lastSlash + 1) : doc->filePath;
                                } else {
                                    // 用户取消了保存，不退出程序
                                    return 0;
                                }
                            }
                            
                            // 使用统一的保存函数
                            if (!SaveDocument(hWnd, doc)) {
                                MessageBoxW(hWnd, (L"无法保存文件: " + doc->filePath).c_str(), 
                                          L"错误", MB_OK | MB_ICONERROR);
                                return 0;
                            }
                        }
                    }
                    // 保存完成后继续关闭
                    DestroyWindow(hWnd);
                } else if (result == IDNO) {
                    // 不保存，直接关闭
                    DestroyWindow(hWnd);
                } else {
                    // 取消，不关闭
                    return 0;
                }
            } else {
                // 没有未保存的文件，直接关闭
                DestroyWindow(hWnd);
            }
        }
        return 0;
    
    case WM_USER + 200:
        // 切换到选择模式，清除组件箱的选择状态
        if (g_pControlToolbox) {
            g_pControlToolbox->ClearSelection();
        }
        return 0;
    
    case WM_COMPILE_OUTPUT:
        // 线程安全的编译输出消息处理
        // wParam 指向动态分配的 wstring*
        if (wParam) {
            std::wstring* pMsg = reinterpret_cast<std::wstring*>(wParam);
            if (g_pOutputPanel) {
                g_pOutputPanel->AppendOutput(*pMsg, false);
            }
            delete pMsg;  // 释放内存
        }
        return 0;
        
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 支持库配置窗口过程
LRESULT CALLBACK LibraryConfigWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hLibraryList = NULL;
    static HWND hDescText = NULL;
    
    switch (message)
    {
    case WM_CREATE:
        {
            // 创建带复选框的支持库列表框
            hLibraryList = CreateWindowW(L"LISTBOX", nullptr,
                WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS,
                20, 50, 400, 300, hWnd, (HMENU)IDC_LIBRARY_LIST, hInst, nullptr);
            
            // 创建描述文本框
            hDescText = CreateWindowW(L"EDIT", L"支持库信息将在此显示",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                440, 50, 300, 300, hWnd, (HMENU)IDC_LIBRARY_DESC, hInst, nullptr);
            
            // 创建按钮
            CreateWindowW(L"BUTTON", L"全选",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                20, 370, 80, 30, hWnd, (HMENU)IDC_BTN_SELECT_ALL, hInst, nullptr);
            
            CreateWindowW(L"BUTTON", L"全不选",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                110, 370, 80, 30, hWnd, (HMENU)IDC_BTN_DESELECT_ALL, hInst, nullptr);
            
            CreateWindowW(L"BUTTON", L"刷新列表",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                200, 370, 90, 30, hWnd, (HMENU)IDC_BTN_REFRESH, hInst, nullptr);
            
            CreateWindowW(L"BUTTON", L"加载并保存",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                520, 370, 120, 30, hWnd, (HMENU)IDC_BTN_LOAD_LIBS, hInst, nullptr);
            
            CreateWindowW(L"BUTTON", L"关闭",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                650, 370, 90, 30, hWnd, (HMENU)IDCANCEL, hInst, nullptr);
            
            // 创建标签
            CreateWindowW(L"STATIC", L"可用支持库（勾选后点击'加载并保存'）：",
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                20, 20, 400, 25, hWnd, NULL, hInst, nullptr);
            
            CreateWindowW(L"STATIC", L"支持库信息：",
                WS_CHILD | WS_VISIBLE | SS_LEFT,
                440, 20, 300, 25, hWnd, NULL, hInst, nullptr);
            
            // 设置字体
            HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
            
            SendMessage(hLibraryList, WM_SETFONT, (WPARAM)hFont, TRUE);
            SendMessage(hDescText, WM_SETFONT, (WPARAM)hFont, TRUE);
            
            // 扫描支持库文件夹
            auto& libConfig = LibraryConfigManager::GetInstance();
            libConfig.ScanLibraryFolders();
            libConfig.LoadConfig();  // 加载之前保存的配置
            
            // 填充列表框
            const auto& libraries = libConfig.GetLibraries();
            
            // 调试信息：显示扫描结果
            wchar_t debugMsg[256];
            swprintf_s(debugMsg, L"扫描到 %zu 个支持库", libraries.size());
            SetWindowTextW(hDescText, debugMsg);
            
            for (size_t i = 0; i < libraries.size(); i++) {
                const auto& lib = libraries[i];
                std::wstring displayName = (lib.loaded ? L"☑ " : L"☐ ") + lib.name;
                SendMessageW(hLibraryList, LB_ADDSTRING, 0, (LPARAM)displayName.c_str());
            }
            
            return 0;
        }
        
    case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {
            case IDC_BTN_SELECT_ALL:
                {
                    auto& libConfig = LibraryConfigManager::GetInstance();
                    const auto& libraries = libConfig.GetLibraries();
                    
                    // 设置所有支持库为已加载
                    for (size_t i = 0; i < libraries.size(); i++) {
                        libConfig.SetLibraryLoaded(i, true);
                    }
                    
                    // 刷新列表显示
                    SendMessageW(hLibraryList, LB_RESETCONTENT, 0, 0);
                    for (size_t i = 0; i < libraries.size(); i++) {
                        const auto& lib = libraries[i];
                        std::wstring displayName = L"☑ " + lib.name;
                        SendMessageW(hLibraryList, LB_ADDSTRING, 0, (LPARAM)displayName.c_str());
                    }
                }
                break;
                
            case IDC_BTN_DESELECT_ALL:
                {
                    auto& libConfig = LibraryConfigManager::GetInstance();
                    const auto& libraries = libConfig.GetLibraries();
                    
                    // 设置所有支持库为未加载
                    for (size_t i = 0; i < libraries.size(); i++) {
                        libConfig.SetLibraryLoaded(i, false);
                    }
                    
                    // 刷新列表显示
                    SendMessageW(hLibraryList, LB_RESETCONTENT, 0, 0);
                    for (size_t i = 0; i < libraries.size(); i++) {
                        const auto& lib = libraries[i];
                        std::wstring displayName = L"☐ " + lib.name;
                        SendMessageW(hLibraryList, LB_ADDSTRING, 0, (LPARAM)displayName.c_str());
                    }
                }
                break;
                
            case IDC_BTN_REFRESH:
                {
                    // 重新扫描
                    auto& libConfig = LibraryConfigManager::GetInstance();
                    libConfig.ScanLibraryFolders();
                    libConfig.LoadConfig();
                    
                    // 刷新列表
                    SendMessageW(hLibraryList, LB_RESETCONTENT, 0, 0);
                    const auto& libraries = libConfig.GetLibraries();
                    for (size_t i = 0; i < libraries.size(); i++) {
                        const auto& lib = libraries[i];
                        std::wstring displayName = (lib.loaded ? L"☑ " : L"☐ ") + lib.name;
                        SendMessageW(hLibraryList, LB_ADDSTRING, 0, (LPARAM)displayName.c_str());
                    }
                }
                break;
                
            case IDC_BTN_LOAD_LIBS:
                {
                    // 加载选中的支持库
                    auto& libConfig = LibraryConfigManager::GetInstance();
                    int count = libConfig.LoadSelectedLibraries();
                    
                    // 保存配置
                    libConfig.SaveConfig();
                    
                    wchar_t msg[256];
                    swprintf_s(msg, 256, L"成功加载 %d 个支持库！\n配置已保存。", count);
                    MessageBoxW(hWnd, msg, L"加载完成", MB_OK | MB_ICONINFORMATION);
                    DestroyWindow(hWnd);
                }
                break;
                
            case IDCANCEL:
                DestroyWindow(hWnd);
                return 0;
                
            case IDC_LIBRARY_LIST:
                if (HIWORD(wParam) == LBN_SELCHANGE)
                {
                    int sel = (int)SendMessageW(hLibraryList, LB_GETCURSEL, 0, 0);
                    auto& libConfig = LibraryConfigManager::GetInstance();
                    const auto& libraries = libConfig.GetLibraries();
                    
                    if (sel != LB_ERR && sel < (int)libraries.size()) {
                        const auto& lib = libraries[sel];
                        std::wstring info = L"支持库名称：\r\n";
                        info += lib.name;
                        info += L"\r\n\r\n支持库路径：\r\n";
                        info += lib.filePath;
                        info += L"\r\n\r\n状态：";
                        info += lib.loaded ? L"已勾选" : L"未勾选";
                        info += L"\r\n\r\n提示：\r\n点击列表项可切换勾选状态。";
                        SetWindowTextW(hDescText, info.c_str());
                    }
                }
                else if (HIWORD(wParam) == LBN_DBLCLK)
                {
                    // 双击切换勾选状态
                    int sel = (int)SendMessageW(hLibraryList, LB_GETCURSEL, 0, 0);
                    auto& libConfig = LibraryConfigManager::GetInstance();
                    const auto& libraries = libConfig.GetLibraries();
                    
                    if (sel != LB_ERR && sel < (int)libraries.size()) {
                        // 切换状态
                        libConfig.SetLibraryLoaded(sel, !libraries[sel].loaded);
                        
                        // 更新显示
                        std::wstring displayName = (libraries[sel].loaded ? L"☑ " : L"☐ ") + libraries[sel].name;
                        SendMessageW(hLibraryList, LB_DELETESTRING, sel, 0);
                        SendMessageW(hLibraryList, LB_INSERTSTRING, sel, (LPARAM)displayName.c_str());
                        SendMessageW(hLibraryList, LB_SETCURSEL, sel, 0);
                        
                        // 更新信息显示
                        const auto& lib = libraries[sel];
                        std::wstring info = L"支持库名称：\r\n";
                        info += lib.name;
                        info += L"\r\n\r\n支持库路径：\r\n";
                        info += lib.filePath;
                        info += L"\r\n\r\n状态：";
                        info += lib.loaded ? L"已勾选" : L"未勾选";
                        info += L"\r\n\r\n提示：\r\n点击列表项可切换勾选状态。";
                        SetWindowTextW(hDescText, info.c_str());
                    }
                }
                break;
            }
        }
        break;
    
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
        {
            HDC hdcStatic = (HDC)wParam;
            // 关于对话框使用固定色
            SetTextColor(hdcStatic, g_CurrentTheme.text);
            SetBkColor(hdcStatic, g_CurrentTheme.bg);
            return (INT_PTR)g_CurrentTheme.hBgBrush;
        }
        
    case WM_DESTROY:
        break;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// "关于"框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// 模型设置对话框
INT_PTR CALLBACK ModelSettingsDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int selectedModel = -1;
    static HWND hModelList = NULL;
    static HWND hEditName = NULL;
    static HWND hEditProvider = NULL;
    static HWND hEditUrl = NULL;
    static HWND hEditKey = NULL;
    
    switch (message)
    {
    case WM_INITDIALOG:
        {
            // 设置对话框标题和大小
            SetWindowTextW(hDlg, L"AI模型管理");
            SetWindowPos(hDlg, NULL, 0, 0, 620, 480, SWP_NOMOVE | SWP_NOZORDER);
            
            // 创建模型列表框（左侧）
            hModelList = CreateWindowW(L"LISTBOX", nullptr,
                WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
                10, 10, 180, 350, hDlg, (HMENU)IDC_MODEL_LIST, hInst, nullptr);
            
            // 添加所有模型到列表
            for (size_t i = 0; i < g_AIModels.size(); i++) {
                SendMessageW(hModelList, LB_ADDSTRING, 0, (LPARAM)g_AIModels[i].name.c_str());
            }
            
            // 新增模型按钮（左下角）
            CreateWindowW(L"BUTTON", L"新增模型",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 370, 85, 30, hDlg, (HMENU)IDC_BTN_ADD, hInst, nullptr);
            
            // 删除模型按钮（新增按钮右侧）
            CreateWindowW(L"BUTTON", L"删除模型",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                105, 370, 85, 30, hDlg, (HMENU)IDC_BTN_DELETE, hInst, nullptr);
            
            // 创建标签和编辑框（右侧）
            int leftX = 200;
            int labelWidth = 80;
            int editX = leftX + labelWidth + 5;
            int editWidth = 400;
            int y = 10;
            int rowHeight = 60;
            
            // 模型名称
            CreateWindowW(L"STATIC", L"模型名称：",
                WS_CHILD | WS_VISIBLE,
                leftX, y + 3, labelWidth, 20, hDlg, (HMENU)IDC_LABEL_NAME, hInst, nullptr);
            hEditName = CreateWindowW(L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                editX, y, editWidth, 24, hDlg, (HMENU)IDC_MODEL_NAME, hInst, nullptr);
            y += rowHeight;
            
            // 提供商
            CreateWindowW(L"STATIC", L"提供商：",
                WS_CHILD | WS_VISIBLE,
                leftX, y + 3, labelWidth, 20, hDlg, (HMENU)IDC_LABEL_PROVIDER, hInst, nullptr);
            hEditProvider = CreateWindowW(L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                editX, y, editWidth, 24, hDlg, (HMENU)IDC_MODEL_PROVIDER, hInst, nullptr);
            y += rowHeight;
            
            // API地址
            CreateWindowW(L"STATIC", L"API地址：",
                WS_CHILD | WS_VISIBLE,
                leftX, y + 3, labelWidth, 20, hDlg, (HMENU)IDC_LABEL_URL, hInst, nullptr);
            hEditUrl = CreateWindowW(L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                editX, y, editWidth, 24, hDlg, (HMENU)IDC_MODEL_URL, hInst, nullptr);
            y += rowHeight;
            
            // API Key
            CreateWindowW(L"STATIC", L"API Key：",
                WS_CHILD | WS_VISIBLE,
                leftX, y + 3, labelWidth, 20, hDlg, (HMENU)IDC_LABEL_KEY, hInst, nullptr);
            hEditKey = CreateWindowW(L"EDIT", L"",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_PASSWORD,
                editX, y, editWidth, 24, hDlg, (HMENU)IDC_MODEL_KEY, hInst, nullptr);
            y += rowHeight;
            
            // 保存按钮
            CreateWindowW(L"BUTTON", L"保存配置",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                editX, y, 100, 30, hDlg, (HMENU)IDC_BTN_SAVE, hInst, nullptr);
            
            // 关闭按钮
            CreateWindowW(L"BUTTON", L"关闭",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                editX + 110, y, 100, 30, hDlg, (HMENU)IDCANCEL, hInst, nullptr);
            
            // 提示信息
            CreateWindowW(L"STATIC", L"← 选择左侧模型进行配置，或点击\n「新增模型」添加自定义模型",
                WS_CHILD | WS_VISIBLE,
                leftX, 350, 400, 60, hDlg, NULL, hInst, nullptr);
        }
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            
            // 列表框选择改变
            if (wmId == IDC_MODEL_LIST && wmEvent == LBN_SELCHANGE) {
                selectedModel = (int)SendMessageW(hModelList, LB_GETCURSEL, 0, 0);
                if (selectedModel >= 0 && selectedModel < (int)g_AIModels.size()) {
                    AIModel& model = g_AIModels[selectedModel];
                    SetWindowTextW(hEditName, model.name.c_str());
                    SetWindowTextW(hEditProvider, model.provider.c_str());
                    SetWindowTextW(hEditUrl, model.apiUrl.c_str());
                    SetWindowTextW(hEditKey, model.apiKey.c_str());
                }
            }
            // 新增模型按钮
            else if (wmId == IDC_BTN_ADD) {
                // 创建新模型
                AIModel newModel(L"新模型", L"自定义", L"https://");
                g_AIModels.push_back(newModel);
                
                // 添加到列表框
                int index = (int)SendMessageW(hModelList, LB_ADDSTRING, 0, (LPARAM)newModel.name.c_str());
                SendMessageW(hModelList, LB_SETCURSEL, index, 0);
                
                // 选中新模型
                selectedModel = (int)g_AIModels.size() - 1;
                SetWindowTextW(hEditName, newModel.name.c_str());
                SetWindowTextW(hEditProvider, newModel.provider.c_str());
                SetWindowTextW(hEditUrl, newModel.apiUrl.c_str());
                SetWindowTextW(hEditKey, L"");
                
                // 聚焦到名称输入框
                SetFocus(hEditName);
            }
            // 删除模型按钮
            else if (wmId == IDC_BTN_DELETE) {
                if (selectedModel >= 0 && selectedModel < (int)g_AIModels.size()) {
                    // 确认删除
                    std::wstring msg = L"确定要删除模型 \"" + g_AIModels[selectedModel].name + L"\" 吗？";
                    if (MessageBoxW(hDlg, msg.c_str(), L"确认删除", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                        // 从列表框删除
                        SendMessageW(hModelList, LB_DELETESTRING, selectedModel, 0);
                        
                        // 从全局数组删除
                        g_AIModels.erase(g_AIModels.begin() + selectedModel);
                        
                        // 清空编辑框
                        SetWindowTextW(hEditName, L"");
                        SetWindowTextW(hEditProvider, L"");
                        SetWindowTextW(hEditUrl, L"");
                        SetWindowTextW(hEditKey, L"");
                        selectedModel = -1;
                        
                        // 更新模型选择下拉框
                        if (hChatModelCombo) {
                            SendMessageW(hChatModelCombo, CB_RESETCONTENT, 0, 0);
                            for (const auto& model : g_AIModels) {
                                SendMessageW(hChatModelCombo, CB_ADDSTRING, 0, (LPARAM)model.name.c_str());
                            }
                            SendMessageW(hChatModelCombo, CB_ADDSTRING, 0, (LPARAM)L"--- 管理模型 ---");
                            SendMessageW(hChatModelCombo, CB_SETCURSEL, 0, 0);
                        }
                    }
                }
            }
            // 保存按钮
            else if (wmId == IDC_BTN_SAVE) {
                if (selectedModel >= 0 && selectedModel < (int)g_AIModels.size()) {
                    AIModel& model = g_AIModels[selectedModel];
                    
                    // 获取编辑框内容
                    wchar_t buffer[1024];
                    
                    GetWindowTextW(hEditName, buffer, 1024);
                    model.name = buffer;
                    
                    GetWindowTextW(hEditProvider, buffer, 1024);
                    model.provider = buffer;
                    
                    GetWindowTextW(hEditUrl, buffer, 1024);
                    model.apiUrl = buffer;
                    
                    GetWindowTextW(hEditKey, buffer, 1024);
                    model.apiKey = buffer;
                    
                    // 更新列表框显示
                    SendMessageW(hModelList, LB_DELETESTRING, selectedModel, 0);
                    SendMessageW(hModelList, LB_INSERTSTRING, selectedModel, (LPARAM)model.name.c_str());
                    SendMessageW(hModelList, LB_SETCURSEL, selectedModel, 0);
                    
                    // 更新模型选择下拉框
                    if (hChatModelCombo) {
                        SendMessageW(hChatModelCombo, CB_RESETCONTENT, 0, 0);
                        for (const auto& m : g_AIModels) {
                            SendMessageW(hChatModelCombo, CB_ADDSTRING, 0, (LPARAM)m.name.c_str());
                        }
                        SendMessageW(hChatModelCombo, CB_ADDSTRING, 0, (LPARAM)L"--- 管理模型 ---");
                        SendMessageW(hChatModelCombo, CB_SETCURSEL, std::min(selectedModel, (int)g_AIModels.size() - 1), 0);
                    }
                    
                    MessageBoxW(hDlg, L"配置已保存！", L"提示", MB_OK | MB_ICONINFORMATION);
                    
                    // 保存到配置文件
                    SaveModelsConfig();
                }
            }
            // 关闭按钮
            else if (wmId == IDCANCEL) {
                EndDialog(hDlg, IDCANCEL);
                return (INT_PTR)TRUE;
            }
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// 注册 AI聊天窗口类
ATOM RegisterAIChatClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = AIChatWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = nullptr;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = szAIChatClass;
    wcex.hIconSm        = nullptr;

    return RegisterClassExW(&wcex);
}

// 简单菜单窗口过程
LRESULT CALLBACK SimpleMenuWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int hoverIndex = -1;
    
    switch (message)
    {
    case WM_ERASEBKGND:
        return 1;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            // 填充背景
            HBRUSH bgBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
            FillRect(hdc, &rect, bgBrush);
            DeleteObject(bgBrush);
            
            // 绘制边框
            HPEN borderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
            HPEN oldPen = (HPEN)SelectObject(hdc, borderPen);
            HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, 0, 0, rect.right, rect.bottom);
            SelectObject(hdc, oldPen);
            SelectObject(hdc, oldBrush);
            DeleteObject(borderPen);
            
            // 绘制菜单项
            if (g_ActiveMenu >= 0 && g_ActiveMenu < (int)g_MenuItems.size()) {
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, g_CurrentTheme.text);
                HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
                HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
                
                int menuItemHeight = 28;
                for (size_t i = 0; i < g_MenuItems[g_ActiveMenu].subItems.size(); i++) {
                    RECT itemRect = {5, (int)i * menuItemHeight + 3, rect.right - 5, (int)(i + 1) * menuItemHeight + 3};
                    
                    bool isEnabled = g_MenuItems[g_ActiveMenu].subItems[i].enabled;
                    
                    // 绘制悬停背景（只有启用的菜单项才显示悬停效果）
                    if ((int)i == hoverIndex && isEnabled) {
                        HBRUSH hoverBrush = CreateSolidBrush(g_CurrentTheme.menuHoverBg);
                        FillRect(hdc, &itemRect, hoverBrush);
                        DeleteObject(hoverBrush);
                    }
                    
                    // 禁用的菜单项显示灰色
                    if (!isEnabled) {
                        SetTextColor(hdc, g_CurrentTheme.textDim);
                    } else {
                        SetTextColor(hdc, g_CurrentTheme.text);
                    }
                    
                    DrawTextW(hdc, g_MenuItems[g_ActiveMenu].subItems[i].text.c_str(), -1, &itemRect,
                        DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                }
                
                SelectObject(hdc, oldFont);
                DeleteObject(hFont);
            }
            
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_MOUSEMOVE:
        {
            POINT pt = {LOWORD(lParam), HIWORD(lParam)};
            int newHover = -1;
            
            if (g_ActiveMenu >= 0 && g_ActiveMenu < (int)g_MenuItems.size()) {
                int menuItemHeight = 28;
                for (size_t i = 0; i < g_MenuItems[g_ActiveMenu].subItems.size(); i++) {
                    RECT itemRect = {5, (int)i * menuItemHeight + 3, 195, (int)(i + 1) * menuItemHeight + 3};
                    if (PtInRect(&itemRect, pt)) {
                        newHover = (int)i;
                        break;
                    }
                }
            }
            
            if (newHover != hoverIndex) {
                hoverIndex = newHover;
                InvalidateRect(hWnd, NULL, FALSE);
            }
        }
        return 0;
        
    case WM_LBUTTONDOWN:
        {
            POINT pt = {LOWORD(lParam), HIWORD(lParam)};
            
            if (g_ActiveMenu >= 0 && g_ActiveMenu < (int)g_MenuItems.size()) {
                int menuItemHeight = 28;
                for (size_t i = 0; i < g_MenuItems[g_ActiveMenu].subItems.size(); i++) {
                    RECT itemRect = {5, (int)i * menuItemHeight + 3, 195, (int)(i + 1) * menuItemHeight + 3};
                    
                    if (PtInRect(&itemRect, pt)) {
                        // 禁用的菜单项不响应点击
                        if (!g_MenuItems[g_ActiveMenu].subItems[i].enabled) {
                            return 0;
                        }
                        int cmdId = g_MenuItems[g_ActiveMenu].subItems[i].id;
                        
                        // 保存主窗口句柄
                        HWND hMain = hMainWnd;
                        
                        // 释放鼠标捕获
                        ReleaseCapture();
                        
                        // 先隐藏菜单但不销毁
                        ShowWindow(hWnd, SW_HIDE);
                        // 重置状态
                        g_ActiveMenu = -1;
                        hoverIndex = -1;
                        // 刷新主窗口
                        InvalidateRect(hMain, NULL, TRUE);
                        UpdateWindow(hMain);
                        
                        // 立即执行命令
                        SendMessage(hMain, WM_COMMAND, MAKEWPARAM(cmdId, 0), 0);
                        
                        // 最后销毁窗口
                        g_MenuPopupWnd = NULL;
                        DestroyWindow(hWnd);
                        return 0;
                    }
                }
            }
            
            // 释放鼠标捕获
            ReleaseCapture();
            
            // 点击空白区域，关闭菜单
            g_MenuPopupWnd = NULL;
            g_ActiveMenu = -1;
            hoverIndex = -1;
            DestroyWindow(hWnd);
        }
        return 0;
        
    case WM_KILLFOCUS:
        // 不自动关闭，让用户点击外部区域来关闭
        return 0;
        
    case WM_ACTIVATE:
        // 不自动关闭
        return 0;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 注册简单菜单窗口类
ATOM RegisterMenuPopupClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DROPSHADOW;
    wcex.lpfnWndProc = SimpleMenuWndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"MenuPopupClass";
    wcex.hIconSm = nullptr;
    return RegisterClassExW(&wcex);
}



// 输出窗口子类化过程 - 自定义绘制边框
LRESULT CALLBACK OutputWindowSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (message)
    {
    case WM_NCPAINT:
        {
            // 调用默认处理
            LRESULT result = DefSubclassProc(hWnd, message, wParam, lParam);
            
            // 绘制自定义边框
            HDC hdc = GetWindowDC(hWnd);
            if (hdc)
            {
                RECT rect;
                GetWindowRect(hWnd, &rect);
                OffsetRect(&rect, -rect.left, -rect.top); // 转换为相对坐标
                
                // 使用主题的grid颜色绘制边框
                HPEN hPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.grid);
                HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                
                Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
                
                SelectObject(hdc, hOldBrush);
                SelectObject(hdc, hOldPen);
                DeleteObject(hPen);
                ReleaseDC(hWnd, hdc);
            }
            return result;
        }
        
    case WM_NCCALCSIZE:
        // 保留1像素边框空间
        if (wParam)
        {
            NCCALCSIZE_PARAMS* params = (NCCALCSIZE_PARAMS*)lParam;
            params->rgrc[0].left += 1;
            params->rgrc[0].top += 1;
            params->rgrc[0].right -= 1;
            params->rgrc[0].bottom -= 1;
            return 0;
        }
        break;
        
    case WM_NCDESTROY:
        RemoveWindowSubclass(hWnd, OutputWindowSubclassProc, uIdSubclass);
        break;
    }
    
    return DefSubclassProc(hWnd, message, wParam, lParam);
}

// 状态栏子类化过程 - 让下边缘穿透，并绘制顶部边框
LRESULT CALLBACK StatusBarSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (message)
    {
    case WM_PAINT:
        {
            // 先让默认处理绘制文本
            LRESULT result = DefSubclassProc(hWnd, message, wParam, lParam);
            
            // 然后在顶部绘制1像素边框线
            HDC hdc = GetDC(hWnd);
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            HPEN borderPen = CreatePen(PS_SOLID, 1, g_CurrentTheme.border);
            HPEN oldPen = (HPEN)SelectObject(hdc, borderPen);
            MoveToEx(hdc, 0, 0, NULL);
            LineTo(hdc, rect.right, 0);
            SelectObject(hdc, oldPen);
            DeleteObject(borderPen);
            
            ReleaseDC(hWnd, hdc);
            return result;
        }
        
    case WM_NCHITTEST:
        {
            // 让下边缘5px的鼠标消息穿透到主窗口，用于调整窗口大小
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ScreenToClient(hWnd, &pt);
            RECT rect;
            GetClientRect(hWnd, &rect);
            if (pt.y > rect.bottom - 5) {
                return HTTRANSPARENT;
            }
            return HTCLIENT;
        }
        
    case WM_NCDESTROY:
        RemoveWindowSubclass(hWnd, StatusBarSubclassProc, uIdSubclass);
        break;
    }
    
    return DefSubclassProc(hWnd, message, wParam, lParam);
}

// 主题配置对话框 - 由于是动态创建，不使用.rc资源
// 直接使用CreateWindowEx创建对话框控件
INT_PTR CALLBACK ThemeConfigDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 临时实现 - 显示提示信息
    static bool initialized = false;
    
    switch (message)
    {
    case WM_INITDIALOG:
        SetWindowTextW(hDlg, L"主题配置");
        MessageBoxW(hDlg, 
            L"主题配置功能正在开发中...\n\n"
            L"当前可用功能：\n"
            L"- 深色主题（默认）\n"
            L"- 浅色主题\n\n"
            L"自定义颜色配置将在下个版本中提供。",
            L"提示", MB_OK | MB_ICONINFORMATION);
        EndDialog(hDlg, IDOK);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// ==================== 可视化设计器相关窗口 ====================

// 可视化设计器窗口过程
LRESULT CALLBACK VisualDesignerWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    VisualDesigner* pDesigner = (VisualDesigner*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    switch (message)
    {
    case WM_CREATE:
        {
            pDesigner = new VisualDesigner(hWnd, nullptr);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pDesigner);
            g_pVisualDesigner = pDesigner;
        }
        return 0;
        
    case WM_DESTROY:
        if (pDesigner) {
            delete pDesigner;
            g_pVisualDesigner = nullptr;
        }
        SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        return 0;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (pDesigner) {
                pDesigner->OnPaint(hdc);
            }
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_SIZE:
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            
            // 布局组件箱（作为可视化设计器的子窗口）
            if (hToolboxWnd && g_IsVisualDesignerActive) {
                if (g_IsToolboxDocked) {
                    // 停靠模式：组件箱在设计器内部右侧
                    int toolboxLeft = width - DOCKED_TOOLBOX_WIDTH;
                    SetWindowPos(hToolboxWnd, HWND_TOP, toolboxLeft, 0, DOCKED_TOOLBOX_WIDTH, height,
                        SWP_SHOWWINDOW);
                } else {
                    // 浮动模式：子窗口会自动跟随父窗口移动
                    // 只需要检查边界并确保可见
                    
                    // 使用 GetWindowRect 获取子窗口相对于父窗口的位置
                    RECT toolboxRect;
                    GetWindowRect(hToolboxWnd, &toolboxRect);
                    int toolboxW = toolboxRect.right - toolboxRect.left;
                    int toolboxH = toolboxRect.bottom - toolboxRect.top;
                    
                    // 获取组件箱在父窗口中的位置（子窗口坐标是相对于父窗口的）
                    RECT childRect;
                    GetWindowRect(hToolboxWnd, &childRect);
                    MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&childRect, 2);
                    int posX = childRect.left;
                    int posY = childRect.top;
                    
                    bool needReposition = false;
                    
                    // 检查是否需要调整位置（只有超出边界时才调整）
                    if (posX + toolboxW > width) {
                        posX = width - toolboxW;
                        needReposition = true;
                    }
                    if (posY + toolboxH > height) {
                        posY = height - toolboxH;
                        needReposition = true;
                    }
                    if (posX < 0) {
                        posX = 0;
                        needReposition = true;
                    }
                    if (posY < 0) {
                        posY = 0;
                        needReposition = true;
                    }
                    
                    if (needReposition) {
                        SetWindowPos(hToolboxWnd, HWND_TOP, posX, posY, 0, 0,
                            SWP_NOSIZE | SWP_SHOWWINDOW);
                    } else {
                        // 只确保可见，不改变位置
                        ShowWindow(hToolboxWnd, SW_SHOW);
                    }
                }
            }
            
            if (pDesigner) {
                // 通知设计器可用区域变化（如果停靠，需要减去组件箱宽度）
                int designerWidth = g_IsToolboxDocked ? (width - DOCKED_TOOLBOX_WIDTH) : width;
                pDesigner->OnSize(designerWidth, height);
            }
        }
        return 0;
        
    case WM_LBUTTONDOWN:
        if (pDesigner) {
            pDesigner->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (UINT)wParam);
        }
        return 0;
        
    case WM_LBUTTONUP:
        if (pDesigner) {
            pDesigner->OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (UINT)wParam);
        }
        return 0;
        
    case WM_MOUSEMOVE:
        if (pDesigner) {
            pDesigner->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (UINT)wParam);
        }
        return 0;
        
    case WM_KEYDOWN:
        if (pDesigner) {
            pDesigner->OnKeyDown((UINT)wParam, (UINT)lParam);
        }
        return 0;
        
    case WM_SETCURSOR:
        {
            if (LOWORD(lParam) == HTCLIENT) {
                // 首先让 VisualDesigner 处理调整大小光标
                if (pDesigner) {
                    HCURSOR hCursor = pDesigner->GetResizeCursor();
                    if (hCursor) {
                        SetCursor(hCursor);
                        return TRUE;
                    }
                }
                
                // 检查组件箱是否有选中的控件类型
                if (g_pControlToolbox) {
                    std::wstring selectedType = g_pControlToolbox->GetSelectedControlType();
                    if (!selectedType.empty()) {
                        // 有选中控件，使用十字光标
                        SetCursor(LoadCursor(NULL, IDC_CROSS));
                        return TRUE;
                    }
                }
                // 没有选中控件，使用默认光标
                SetCursor(LoadCursor(NULL, IDC_ARROW));
                return TRUE;
            }
        }
        break;
        
    case WM_ERASEBKGND:
        return 1;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 属性窗口过程
LRESULT CALLBACK PropertyGridWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PropertyGrid* pGrid = (PropertyGrid*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    switch (message)
    {
    case WM_CREATE:
        {
            pGrid = new PropertyGrid(hWnd);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pGrid);
            g_pPropertyGrid = pGrid;
        }
        return 0;
        
    case WM_DESTROY:
        if (pGrid) {
            delete pGrid;
            g_pPropertyGrid = nullptr;
        }
        SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        return 0;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (pGrid) {
                pGrid->OnPaint(hdc);
            }
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_SIZE:
        if (pGrid) {
            pGrid->OnSize(LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
        
    case WM_LBUTTONDOWN:
        if (pGrid) {
            pGrid->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        return 0;
        
    case WM_LBUTTONUP:
        if (pGrid) {
            pGrid->OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        return 0;
        
    case WM_MOUSEMOVE:
        if (pGrid) {
            pGrid->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        // 当鼠标进入 PropertyGrid 时，清除资源管理器的边框悬停状态
        if (g_ExplorerData.isBorderHover) {
            g_ExplorerData.isBorderHover = false;
            if (hRightPanelWnd) {
                InvalidateRect(hRightPanelWnd, NULL, FALSE);
            }
        }
        return 0;
        
    case WM_SETCURSOR:
        // 在 PropertyGrid 内部，始终使用箭头光标
        // 这样当鼠标从资源管理器边框拖动区域移入时，光标会正确变回箭头
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        return TRUE;
        
    case WM_LBUTTONDBLCLK:
        if (pGrid) {
            pGrid->OnLButtonDblClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        return 0;
        
    case WM_VSCROLL:
        if (pGrid) {
            pGrid->OnVScroll(wParam);
        }
        return 0;
        
    case WM_ERASEBKGND:
        return 1;
        
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
        {
            // 设置编辑框和下拉列表的背景和文字颜色，使用主题颜色
            HDC hdcCtrl = (HDC)wParam;
            SetTextColor(hdcCtrl, g_CurrentTheme.text);
            SetBkColor(hdcCtrl, g_CurrentTheme.editorBg);
            
            // 创建或获取背景画刷
            static HBRUSH hBrushEdit = NULL;
            static COLORREF lastBgColor = 0;
            if (hBrushEdit == NULL || lastBgColor != g_CurrentTheme.editorBg) {
                if (hBrushEdit) DeleteObject(hBrushEdit);
                hBrushEdit = CreateSolidBrush(g_CurrentTheme.editorBg);
                lastBgColor = g_CurrentTheme.editorBg;
            }
            return (LRESULT)hBrushEdit;
        }
        
    case WM_COMMAND:
        {
            int notifyCode = HIWORD(wParam);
            // 处理下拉框关闭事件（选择完成后下拉列表关闭时触发）
            if (notifyCode == CBN_CLOSEUP) {
                // 使用 PostMessage 延迟处理，避免在消息处理中销毁控件
                PostMessage(hWnd, WM_USER + 100, 0, 0);
                return 0;
            }
            // 处理编辑框内容变化 - 实时更新
            if (notifyCode == EN_CHANGE) {
                // 使用 PostMessage 延迟处理，实时应用变更
                PostMessage(hWnd, WM_USER + 101, 0, 0);
                return 0;
            }
            // 处理编辑框失去焦点
            if (notifyCode == EN_KILLFOCUS) {
                // 使用 PostMessage 延迟处理
                PostMessage(hWnd, WM_USER + 100, 0, 0);
                return 0;
            }
        }
        break;
        
    case WM_USER + 100:
        // 延迟处理：完成编辑
        if (pGrid) {
            pGrid->FinishEdit(true);
        }
        return 0;
        
    case WM_USER + 101:
        // 延迟处理：实时更新属性（不结束编辑）
        if (pGrid) {
            pGrid->ApplyEditWithoutFinish();
        }
        return 0;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 组件箱窗口过程（浮动窗口）
LRESULT CALLBACK ToolboxWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool isDragging = false;
    static bool isResizing = false;      // 正在调整大小
    static int resizeEdge = 0;           // 0=无, 1=左边, 2=右边
    static POINT dragStart;
    static RECT windowRect;
    static int originalWidth = 0;
    
    ControlToolbox* pToolbox = (ControlToolbox*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    switch (message)
    {
    case WM_CREATE:
        {
            pToolbox = new ControlToolbox(hWnd);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pToolbox);
            g_pControlToolbox = pToolbox;
            
            // 初始化控件渲染器并加载到组件箱
            if (!g_pControlRenderer) {
                g_pControlRenderer = new ControlRenderer();
            }
            pToolbox->LoadFromRenderer(g_pControlRenderer);
        }
        return 0;
        
    case WM_DESTROY:
        // 保存组件箱位置
        SaveToolboxPosition();
        
        if (pToolbox) {
            delete pToolbox;
            g_pControlToolbox = nullptr;
        }
        SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        return 0;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (pToolbox) {
                pToolbox->OnPaint(hdc);
            }
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_SIZE:
        if (pToolbox) {
            pToolbox->OnSize(LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
        
    case WM_LBUTTONDOWN:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            
            // 检查是否点击在标题栏区域（顶部28像素，与 ControlToolbox::TITLEBAR_HEIGHT 一致）
            if (y < 28) {
                // 先检查是否点击了标题栏按钮
                if (pToolbox) {
                    ControlToolbox::TitlebarButton btn = pToolbox->HitTestTitlebarButton(x, y);
                    if (btn != ControlToolbox::BTN_NONE) {
                        // 标题栏按钮点击，交给 ControlToolbox 处理
                        pToolbox->OnLButtonDown(x, y);
                        return 0;
                    }
                }
                // 停靠模式下不允许拖动
                if (!g_IsToolboxDocked) {
                    // 浮动模式 - 开始拖动
                    isDragging = true;
                    
                    // 获取当前窗口位置（相对于父窗口）
                    RECT wndRect;
                    GetWindowRect(hWnd, &wndRect);
                    HWND hParent = GetParent(hWnd);
                    if (hParent) {
                        POINT pos = { wndRect.left, wndRect.top };
                        ScreenToClient(hParent, &pos);
                        windowRect.left = pos.x;
                        windowRect.top = pos.y;
                        
                        // 获取鼠标在父窗口中的位置作为拖动起始点
                        POINT mousePos;
                        GetCursorPos(&mousePos);
                        ScreenToClient(hParent, &mousePos);
                        dragStart.x = mousePos.x;
                        dragStart.y = mousePos.y;
                    }
                    SetCapture(hWnd);
                }
            } else if (pToolbox) {
                pToolbox->OnLButtonDown(x, y);
            }
        }
        return 0;
        
    case WM_LBUTTONUP:
        if (isDragging) {
            isDragging = false;
            ReleaseCapture();
            // 保存浮动位置
            SaveToolboxPosition();
        } else if (pToolbox) {
            pToolbox->OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        return 0;
        
    case WM_MOUSEMOVE:
        {
            // 启用鼠标离开追踪
            TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
        }
        if (isDragging) {
            // 获取鼠标当前位置（相对于父窗口）
            POINT pt;
            GetCursorPos(&pt);
            HWND hParent = GetParent(hWnd);
            if (hParent) {
                ScreenToClient(hParent, &pt);
                
                // 获取组件箱当前大小
                RECT toolboxRect;
                GetWindowRect(hWnd, &toolboxRect);
                int toolboxW = toolboxRect.right - toolboxRect.left;
                int toolboxH = toolboxRect.bottom - toolboxRect.top;
                
                // 获取父窗口（可视化设计器）大小
                RECT parentRect;
                GetClientRect(hParent, &parentRect);
                
                // 计算新位置：初始窗口位置 + 鼠标移动偏移
                int newX = windowRect.left + (pt.x - dragStart.x);
                int newY = windowRect.top + (pt.y - dragStart.y);
                
                // 严格限制在父窗口范围内（组件箱边框不能超出设计器边框）
                // 左边界
                if (newX < 0) {
                    newX = 0;
                }
                // 右边界
                if (newX + toolboxW > parentRect.right) {
                    newX = parentRect.right - toolboxW;
                }
                // 上边界
                if (newY < 0) {
                    newY = 0;
                }
                // 下边界
                if (newY + toolboxH > parentRect.bottom) {
                    newY = parentRect.bottom - toolboxH;
                }
                
                // 如果父窗口太小，至少让组件箱左上角可见
                if (newX < 0) newX = 0;
                if (newY < 0) newY = 0;
                
                SetWindowPos(hWnd, NULL, newX, newY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
                
                // 更新拖动起始点和窗口位置
                dragStart.x = pt.x;
                dragStart.y = pt.y;
                windowRect.left = newX;
                windowRect.top = newY;
            }
        } else if (pToolbox) {
            pToolbox->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        return 0;
        
    case WM_LBUTTONDBLCLK:
        if (pToolbox) {
            pToolbox->OnLButtonDblClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        return 0;
        
    case WM_VSCROLL:
        if (pToolbox) {
            pToolbox->OnVScroll(wParam);
        }
        return 0;
        
    case WM_MOUSEWHEEL:
        if (pToolbox) {
            pToolbox->OnMouseWheel(wParam);
        }
        return 0;
        
    case WM_MOUSELEAVE:
        // 鼠标离开窗口，重置悬停状态
        if (pToolbox) {
            pToolbox->OnMouseMove(-1, -1);  // 用无效坐标触发清除悬停
        }
        return 0;
        
    case WM_ERASEBKGND:
        return 1;
        
    case WM_SETCURSOR:
        {
            // 图标模式下，在边框区域显示调整大小光标
            if (pToolbox && pToolbox->IsIconMode()) {
                POINT pt;
                GetCursorPos(&pt);
                ScreenToClient(hWnd, &pt);
                
                RECT rect;
                GetClientRect(hWnd, &rect);
                const int BORDER_WIDTH = 5;
                
                if (pt.y >= 28) {  // 标题栏下方
                    if (g_IsToolboxDocked) {
                        // 停靠模式：只有左边框
                        if (pt.x <= BORDER_WIDTH) {
                            SetCursor(LoadCursor(NULL, IDC_SIZEWE));
                            return TRUE;
                        }
                    } else {
                        // 浮动模式：左右边框
                        if (pt.x <= BORDER_WIDTH || pt.x >= rect.right - BORDER_WIDTH) {
                            SetCursor(LoadCursor(NULL, IDC_SIZEWE));
                            return TRUE;
                        }
                    }
                }
            }
            // 默认箭头光标
            SetCursor(LoadCursor(NULL, IDC_ARROW));
            return TRUE;
        }
        
    case WM_NCHITTEST:
        {
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient(hWnd, &pt);
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            const int BORDER_WIDTH = 5;
            
            // 图标模式下支持调整宽度
            if (pToolbox && pToolbox->IsIconMode()) {
                if (g_IsToolboxDocked) {
                    // 停靠模式：只能调整左边框
                    if (pt.x <= BORDER_WIDTH && pt.y >= 28) {  // 28 是标题栏高度
                        return HTLEFT;
                    }
                } else {
                    // 浮动模式：左右边框都可以调整
                    if (pt.x <= BORDER_WIDTH && pt.y >= 28) {
                        return HTLEFT;
                    }
                    if (pt.x >= rect.right - BORDER_WIDTH && pt.y >= 28) {
                        return HTRIGHT;
                    }
                }
            }
            
            // 其他区域返回客户区，拖动由 WM_LBUTTONDOWN/WM_MOUSEMOVE 处理
            return HTCLIENT;
        }
    
    case WM_CLOSE:
        // 点击关闭按钮时只隐藏窗口，不销毁
        ShowWindow(hWnd, SW_HIDE);
        return 0;  // 返回0表示已处理，不调用默认的DestroyWindow
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 输出面板窗口过程
LRESULT CALLBACK OutputPanelWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    OutputPanel* pPanel = (OutputPanel*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    switch (message)
    {
    case WM_CREATE:
        {
            pPanel = new OutputPanel(hWnd);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pPanel);
            g_pOutputPanel = pPanel;
        }
        return 0;
        
    case WM_DESTROY:
        if (pPanel) {
            delete pPanel;
            g_pOutputPanel = nullptr;
        }
        SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        return 0;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            if (pPanel) {
                pPanel->OnPaint(hdc);
            }
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_SIZE:
        if (pPanel) {
            pPanel->OnSize(LOWORD(lParam), HIWORD(lParam));
        }
        return 0;
        
    case WM_LBUTTONDOWN:
        if (pPanel) {
            pPanel->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        return 0;
    
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
        {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, g_CurrentTheme.text);
            SetBkColor(hdcEdit, g_CurrentTheme.editorBg);
            static HBRUSH hBrush = NULL;
            if (hBrush) DeleteObject(hBrush);
            hBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
            return (LRESULT)hBrush;
        }
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 注册可视化设计器窗口类
ATOM RegisterVisualDesignerClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = VisualDesignerWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szVisualDesignerClass;
    return RegisterClassExW(&wcex);
}

// 注册属性窗口类
ATOM RegisterPropertyGridClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = PropertyGridWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szPropertyGridClass;
    return RegisterClassExW(&wcex);
}

// 注册组件箱窗口类
ATOM RegisterToolboxClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = ToolboxWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szToolboxClass;
    return RegisterClassExW(&wcex);
}

ATOM RegisterOutputPanelClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = OutputPanelWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szOutputPanelClass;
    return RegisterClassExW(&wcex);
}

// ==================== 可视化设计器辅助函数 ====================

// 切换可视化设计器模式
void SwitchToVisualDesignerMode(bool enable)
{
    if (g_IsVisualDesignerActive == enable) return;
    
    g_IsVisualDesignerActive = enable;
    
    if (enable) {
        // 进入可视化设计模式
        // 组件箱是可视化设计器的子窗口，显示它
        if (hToolboxWnd) {
            // 同步 ControlToolbox 的停靠状态
            if (g_pControlToolbox) {
                g_pControlToolbox->SetDocked(g_IsToolboxDocked);
            }
            
            // 显示组件箱
            ShowWindow(hToolboxWnd, SW_SHOW);
            
            // 如果是浮动模式，加载保存的位置
            if (!g_IsToolboxDocked) {
                LoadToolboxPosition();
            }
        }
        
        // 切换资源管理器到属性标签
        ExplorerSwitchToTab(TAB_PROPERTY);
        
        // 设置设计器的选择变更回调
        if (g_pVisualDesigner) {
            g_pVisualDesigner->SetSelectionChangedCallback(UpdatePropertyGridForSelection);
        }
        
        // 设置属性窗口的属性变更回调
        if (g_pPropertyGrid) {
            g_pPropertyGrid->SetPropertyChangedCallback([](const std::wstring& propName, const std::wstring& newValue) {
                if (!g_pVisualDesigner) return;
                
                auto selectedControls = g_pVisualDesigner->GetSelectedControls();
                if (selectedControls.empty()) {
                    // 修改窗体属性
                    g_pVisualDesigner->SetFormProperty(propName, newValue);
                    
                    // 如果修改的是"控制按钮"，需要刷新属性面板以更新子选项的禁用状态
                    if (propName == L"控制按钮") {
                        UpdatePropertyGridForSelection();
                    }
                } else if (selectedControls.size() == 1) {
                    // 修改单个控件属性
                    g_pVisualDesigner->SetControlProperty(selectedControls[0]->id, propName, newValue);
                }
            });
        }
        
        // 刷新属性窗口
        UpdatePropertyGridForSelection();
    } else {
        // 退出可视化设计模式
        // 隐藏组件箱
        if (hToolboxWnd) {
            if (!g_IsToolboxDocked) {
                // 浮动模式 - 保存位置
                SaveToolboxPosition();
            }
            ShowWindow(hToolboxWnd, SW_HIDE);
        }
        
        // 切换资源管理器回项目标签
        ExplorerSwitchToTab(TAB_PROJECT);
    }
    
    // 触发窗口重新布局
    RECT rect;
    GetClientRect(hMainWnd, &rect);
    SendMessage(hMainWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.right, rect.bottom));
    
    // 进入设计器模式时，额外触发可视化设计器的 WM_SIZE 来布局组件箱
    if (enable && hVisualDesignerWnd) {
        RECT designerRect;
        GetClientRect(hVisualDesignerWnd, &designerRect);
        SendMessage(hVisualDesignerWnd, WM_SIZE, SIZE_RESTORED, 
            MAKELPARAM(designerRect.right, designerRect.bottom));
    }
    
    // 更新菜单状态（组件箱菜单项的启用/禁用状态）
    UpdateMenuItems();
}

// 更新属性窗口显示选中控件的属性
void UpdatePropertyGridForSelection()
{
    if (!g_pPropertyGrid || !g_pVisualDesigner) {
        return;
    }
    
    // 检查是否选中了窗体
    if (g_pVisualDesigner->IsFormSelected()) {
        // 显示窗体属性
        FormInfo& formInfo = g_pVisualDesigner->GetFormInfo();
        
        // 尝试从支持库加载窗体属性
        std::map<std::wstring, std::wstring> formProps;
        formProps[L"名称"] = formInfo.name;
        formProps[L"标题"] = formInfo.title;
        formProps[L"宽度"] = std::to_wstring(formInfo.width);
        formProps[L"高度"] = std::to_wstring(formInfo.height);
        
        // 添加布尔属性（控制按钮、最大化按钮、最小化按钮）
        formProps[L"控制按钮"] = formInfo.hasControlBox ? L"真" : L"假";
        formProps[L"最大化按钮"] = formInfo.hasMaxButton ? L"真" : L"假";
        formProps[L"最小化按钮"] = formInfo.hasMinButton ? L"真" : L"假";
        
        // 添加窗体的自定义属性
        for (const auto& [key, value] : formInfo.properties) {
            formProps[key] = value;
        }
        
        g_pPropertyGrid->LoadPropertiesFromLibrary(L"窗口", formProps);
        
        // 如果支持库没有窗口定义（检查是否加载了属性），使用基本属性
        // 注意：支持库中属性名是"左边"、"宽度"等，不是"名称"
        if (g_pPropertyGrid->GetPropertyValue(L"宽度").empty()) {
            std::vector<PropertyDef> props;
            
            PropertyDef nameProp;
            nameProp.name = L"名称";
            nameProp.displayName = L"名称";
            nameProp.category = L"基本";
            nameProp.editorType = PropertyEditorType::Text;
            nameProp.value = formInfo.name;
            props.push_back(nameProp);
            
            PropertyDef titleProp;
            titleProp.name = L"标题";
            titleProp.displayName = L"标题";
            titleProp.category = L"基本";
            titleProp.editorType = PropertyEditorType::Text;
            titleProp.value = formInfo.title;
            props.push_back(titleProp);
            
            PropertyDef widthProp;
            widthProp.name = L"宽度";
            widthProp.displayName = L"宽度";
            widthProp.category = L"位置";
            widthProp.editorType = PropertyEditorType::Number;
            widthProp.value = std::to_wstring(formInfo.width);
            props.push_back(widthProp);
            
            PropertyDef heightProp;
            heightProp.name = L"高度";
            heightProp.displayName = L"高度";
            heightProp.category = L"位置";
            heightProp.editorType = PropertyEditorType::Number;
            heightProp.value = std::to_wstring(formInfo.height);
            props.push_back(heightProp);
            
            g_pPropertyGrid->SetProperties(props);
        }
        
        g_pPropertyGrid->SetObjectInfo(L"窗体", formInfo.name);
        InvalidateRect(hPropertyGridWnd, NULL, TRUE);
        return;
    }
    
    // 获取选中的控件
    auto selectedControls = g_pVisualDesigner->GetSelectedControls();
    
    if (selectedControls.empty()) {
        // 没有选中任何控件也没有选中窗体，清空属性窗口
        g_pPropertyGrid->Clear();
        g_pPropertyGrid->SetObjectInfo(L"", L"");
        
    } else if (selectedControls.size() == 1) {
        // 选中单个控件
        auto ctrl = selectedControls[0];
        
        // 准备当前属性值
        std::map<std::wstring, std::wstring> currentValues = ctrl->properties;
        
        // 添加位置和大小属性
        currentValues[L"左边"] = std::to_wstring(ctrl->bounds.X);
        currentValues[L"顶边"] = std::to_wstring(ctrl->bounds.Y);
        currentValues[L"宽度"] = std::to_wstring(ctrl->bounds.Width);
        currentValues[L"高度"] = std::to_wstring(ctrl->bounds.Height);
        currentValues[L"名称"] = ctrl->name;
        
        // 从支持库加载属性
        g_pPropertyGrid->LoadPropertiesFromLibrary(ctrl->type, currentValues);
        g_pPropertyGrid->SetObjectInfo(ctrl->type, ctrl->name);
        
    } else {
        // 选中多个控件
        g_pPropertyGrid->SetObjectInfo(L"多个控件", std::to_wstring(selectedControls.size()) + L" 个控件");
        g_pPropertyGrid->Clear();
    }
    
    InvalidateRect(hPropertyGridWnd, NULL, TRUE);
}
