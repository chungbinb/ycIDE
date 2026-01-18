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
#include <vector>
#include <string>
#include <fstream>
#include <commdlg.h>
#include <gdiplus.h>
#include <shlobj.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <commctrl.h>

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
HWND hWelcomePageWnd;  // 欢迎页窗口
HWND hRightPanelWnd;
HWND hOutputWnd;
HWND hStatusBar = NULL;  // 状态栏

// 标签栏高度
const int g_TabBarHeight = 35;

// 分隔条相关
int g_LeftPanelWidth = 400;  // AI聊天框宽度(默认400)
int g_SavedLeftPanelWidth = 400;  // 保存收起前的宽度
bool g_LeftPanelVisible = true;  // 左侧面板是否可见
int g_RightPanelWidth = 250; // 右侧面板宽度
const int g_StatusBarHeight = 24;  // 状态栏高度
bool g_IsDraggingLeftSplitter = false;  // 是否正在拖拽左侧分隔条
int g_SplitterWidth = 1;  // 分隔条宽度（1像素边框）
int g_MinLeftPanelWidth = 330;  // AI聊天框最小宽度
bool g_IsHoveringLeftSplitter = false;  // 鼠标是否悬停在左侧分隔条上

// 自绘标题栏相关
const int g_TitleBarHeight = 32;  // 标题栏高度
const int g_TotalTopHeight = g_TitleBarHeight;  // 顶部总高度
bool g_IsMaximized = false;
bool g_IsDraggingWindow = false;
POINT g_DragStartPos;
RECT g_DragStartRect;

// 标题栏按钮区域
RECT g_CloseButtonRect = {0};
RECT g_MaxButtonRect = {0};
RECT g_MinButtonRect = {0};
int g_HoverButton = 0;  // 0=无, 1=最小化, 2=最大化, 3=关闭, 4=侧边栏切换
bool g_IsTrackingMouse = false;  // 是否正在追踪鼠标离开事件
RECT g_SidebarToggleRect = {0};  // 侧边栏切换按钮区域

// 菜单项
struct MenuItem {
    std::wstring text;
    RECT rect;
    int id;
    bool hovered;
    std::vector<MenuItem> subItems;
};
std::vector<MenuItem> g_MenuItems;
int g_ActiveMenu = -1;  // 当前激活的菜单
HWND g_MenuPopupWnd = NULL;  // 菜单弹出窗口

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                RegisterAIChatClass(HINSTANCE hInstance);
ATOM                RegisterMenuPopupClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    AIChatWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    MenuPopupWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    OutputWindowSubclassProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK    StatusBarSubclassProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK    LibraryConfigWndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ModelSettingsDlg(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ThemeConfigDlg(HWND, UINT, WPARAM, LPARAM);
void                UpdateMenuItems();  // 更新菜单项（根据项目状态）

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
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
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
    MenuItem fileMenu = {L"文件", {0}, IDM_FILE, false, {}};
    fileMenu.subItems = {
        {L"新建项目", {0}, IDM_NEW_PROJECT, false, {}},
        {L"打开项目", {0}, IDM_OPEN_PROJECT, false, {}},
        {L"关闭项目", {0}, IDM_CLOSE_PROJECT, false, {}},
        {L"添加文件到项目", {0}, IDM_ADD_FILE_TO_PROJECT, false, {}},
        {L"新建文件", {0}, IDM_NEW, false, {}},
        {L"打开文件", {0}, IDM_OPEN, false, {}},
        {L"打开文件夹", {0}, IDM_OPEN_FOLDER, false, {}},
        {L"关闭文件夹", {0}, IDM_CLOSE_FOLDER, false, {}},
        {L"保存", {0}, IDM_SAVE, false, {}},
        {L"另存为", {0}, IDM_SAVE_AS, false, {}},
        {L"退出", {0}, IDM_EXIT, false, {}}
    };
    
    // 插入菜单（仅当项目打开时显示）
    MenuItem insertMenu = {L"插入", {0}, IDM_INSERT, false, {}};
    insertMenu.subItems = {
        {L"程序集", {0}, IDM_INSERT_MODULE, false, {}},
        {L"DLL命令", {0}, IDM_INSERT_DLL_COMMAND, false, {}},
        {L"自定义数据类型", {0}, IDM_INSERT_DATATYPE, false, {}},
        {L"全局变量", {0}, IDM_INSERT_GLOBAL_VAR, false, {}},
        {L"常量", {0}, IDM_INSERT_CONSTANT, false, {}},
        {L"窗口", {0}, IDM_INSERT_WINDOW, false, {}},
        {L"类模块", {0}, IDM_INSERT_CLASS_MODULE, false, {}}
    };
    
    // 主题菜单
    MenuItem themeMenu = {L"主题", {0}, IDM_THEME, false, {}};
    themeMenu.subItems = {
        {L"深色主题", {0}, IDM_THEME_DARK, false, {}},
        {L"浅色主题", {0}, IDM_THEME_LIGHT, false, {}},
        {L"自定义主题...", {0}, IDM_THEME_CUSTOM, false, {}}
    };
    
    // 工具菜单
    MenuItem toolsMenu = {L"工具", {0}, IDM_TOOLS, false, {}};
    toolsMenu.subItems = {
        {L"系统配置", {0}, IDM_SYSTEM_CONFIG, false, {}},
        {L"支持库配置", {0}, IDM_LIBRARY_CONFIG, false, {}}
    };
    
    // 帮助菜单
    MenuItem helpMenu = {L"帮助", {0}, IDM_HELP, false, {}};
    helpMenu.subItems = {
        {L"关于", {0}, IDM_ABOUT, false, {}}
    };
    
    // 添加菜单项
    g_MenuItems.push_back(fileMenu);
    
    // 检查是否有打开的项目
    auto& pm = ProjectManager::GetInstance();
    if (pm.HasOpenProject()) {
        g_MenuItems.push_back(insertMenu);
    }
    
    g_MenuItems.push_back(themeMenu);
    g_MenuItems.push_back(toolsMenu);
    g_MenuItems.push_back(helpMenu);
    
    // 重新计算菜单位置
    int menuX = 50;
    for (size_t i = 0; i < g_MenuItems.size(); i++) {
        g_MenuItems[i].rect = {menuX, 0, menuX + 60, g_TitleBarHeight};
        menuX += 60;
    }
    
    // 更新侧边栏切换按钮位置
    extern RECT g_SidebarToggleRect;
    g_SidebarToggleRect = {menuX + 10, 0, menuX + 42, g_TitleBarHeight};
    
    // 刷新标题栏
    extern HWND hMainWnd;
    if (hMainWnd) {
        RECT titleRect = {0, 0, 0, g_TitleBarHeight};
        GetClientRect(hMainWnd, &titleRect);
        titleRect.bottom = g_TitleBarHeight;
        InvalidateRect(hMainWnd, &titleRect, FALSE);
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
      WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX,
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
            
            // 加载主题配置
            LoadThemeConfig();
            
            // 1. 左侧: AI 聊天框（使用自定义类）
            hAIChatWnd = CreateWindowW(szAIChatClass, nullptr,
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hWnd, (HMENU)1001, hInst, nullptr);

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

            // 3. 右侧: 资源管理器 (自定义类)
            hRightPanelWnd = CreateWindowW(L"ResourceExplorer", nullptr,
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hWnd, (HMENU)1003, hInst, nullptr);
            
            // 4. 底部: 输出信息
            hOutputWnd = CreateWindowW(L"EDIT", L"输出信息:\r\n就绪.",
                WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
                0, 0, 0, 0, hWnd, (HMENU)1004, hInst, nullptr);
            
            // 设置输出窗口字体
            HFONT hOutputFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
            SendMessage(hOutputWnd, WM_SETFONT, (WPARAM)hOutputFont, TRUE);
            
            // 子类化输出窗口以自定义绘制边框
            SetWindowSubclass(hOutputWnd, OutputWindowSubclassProc, 0, 0);
            
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

            int leftW = g_LeftPanelVisible ? g_LeftPanelWidth : 0;
            int rightW = g_RightPanelWidth;
            int outputH = 150;  // 输出窗口高度
            int topH = g_TotalTopHeight;  // 标题栏+工具栏高度
            int statusH = g_StatusBarHeight;  // 状态栏高度
            
            // 确保中间区域不为负（窗口之间不留间距，分隔线绘制在窗口上）
            int centerW = width - leftW - rightW;
            if (centerW < 100) {
                centerW = 100;
                leftW = width - centerW - rightW;
                if (leftW < 100) leftW = 100;
            }
            
            // 中间区域可用高度（减去状态栏）
            int centerTotalH = height - topH - statusH;
            // 编辑器高度 = 中间总高度 - 输出窗口高度
            int editorH = centerTotalH - outputH;
            if (editorH < 0) editorH = 0;

            // 调整各窗口位置（使用 DeferWindowPos 批量更新，减少闪烁）
            HDWP hdwp = BeginDeferWindowPos(8);
            if (hdwp) {
                // 左侧AI聊天窗口占满整个高度（不包括状态栏）
                if (hAIChatWnd) {
                    if (g_LeftPanelVisible) {
                        hdwp = DeferWindowPos(hdwp, hAIChatWnd, NULL, 0, topH, leftW, centerTotalH, 
                            SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
                    } else {
                        hdwp = DeferWindowPos(hdwp, hAIChatWnd, NULL, 0, topH, 0, centerTotalH, 
                            SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
                    }
                }
                
                // 中间上部：标签栏
                if (hTabBarWnd)
                    hdwp = DeferWindowPos(hdwp, hTabBarWnd, NULL, leftW, topH, centerW, g_TabBarHeight,
                        SWP_NOZORDER | SWP_NOACTIVATE);
                
                // 中间上部：编辑器（在标签栏下方）
                int editorTop = topH + g_TabBarHeight;
                int editorHeight = editorH - g_TabBarHeight;
                
                if (hEditorWnd) 
                    hdwp = DeferWindowPos(hdwp, hEditorWnd, NULL, leftW, editorTop, centerW, editorHeight, 
                        SWP_NOZORDER | SWP_NOACTIVATE);
                
                if (hEllEditorWnd)
                    hdwp = DeferWindowPos(hdwp, hEllEditorWnd, NULL, leftW, editorTop, centerW, editorHeight,
                        SWP_NOZORDER | SWP_NOACTIVATE);
                
                // 中间：欢迎页（占据整个编辑器区域）
                if (hWelcomePageWnd) {
                    // 根据当前是否有文件打开决定欢迎页的显示状态
                    TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                    bool hasOpenFiles = (tabData && !tabData->tabs.empty());
                    
                    if (hasOpenFiles) {
                        // 有文件打开，确保欢迎页隐藏
                        hdwp = DeferWindowPos(hdwp, hWelcomePageWnd, NULL, leftW, editorTop, centerW, editorHeight,
                            SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
                    } else {
                        // 没有文件打开，显示欢迎页
                        hdwp = DeferWindowPos(hdwp, hWelcomePageWnd, HWND_TOP, leftW, editorTop, centerW, editorHeight,
                            SWP_NOACTIVATE | SWP_SHOWWINDOW);
                    }
                }
                
                // 中间下部：输出窗口
                if (hOutputWnd) 
                    hdwp = DeferWindowPos(hdwp, hOutputWnd, NULL, leftW, topH + editorH, centerW, outputH, 
                        SWP_NOZORDER | SWP_NOACTIVATE);
                // 右侧资源管理器占满整个高度（不包括状态栏）
                if (hRightPanelWnd) 
                    hdwp = DeferWindowPos(hdwp, hRightPanelWnd, NULL, leftW + centerW, topH, rightW, centerTotalH, 
                        SWP_NOZORDER | SWP_NOACTIVATE);
                // 底部状态栏横跨整个宽度（左边留10px边距）
                if (hStatusBar) 
                    hdwp = DeferWindowPos(hdwp, hStatusBar, NULL, 10, height - statusH, width - 10, statusH, 
                        SWP_NOZORDER | SWP_NOACTIVATE);
                
                if (hdwp) EndDeferWindowPos(hdwp);
            }
            
            // 更新标题栏按钮位置
            int btnWidth = 46;
            int btnHeight = g_TitleBarHeight;
            g_CloseButtonRect = {width - btnWidth, 0, width, btnHeight};
            g_MaxButtonRect = {width - btnWidth * 2, 0, width - btnWidth, btnHeight};
            g_MinButtonRect = {width - btnWidth * 3, 0, width - btnWidth * 2, btnHeight};
            
            // 更新菜单项位置
            int menuX = 50;  // 从标题后开始
            for (size_t i = 0; i < g_MenuItems.size(); i++) {
                g_MenuItems[i].rect = {menuX, 0, menuX + 60, g_TitleBarHeight};
                menuX += 60;
            }
            
            // 更新侧边栏切换按钮位置（在菜单右边）
            g_SidebarToggleRect = {menuX + 10, 0, menuX + 42, g_TitleBarHeight};
            
            InvalidateRect(hWnd, NULL, TRUE);
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
            
            // 标签栏切换事件 (wmId=2000, wmEvent=标签索引)
            if (wmId == 2000) {
                int tabIndex = wmEvent;
                TabBarData* tabData = (TabBarData*)GetWindowLongPtr(hTabBarWnd, GWLP_USERDATA);
                if (tabData && tabIndex >= 0 && tabIndex < (int)tabData->tabs.size()) {
                    int editorType = tabData->tabs[tabIndex].editorType;
                    
                    // 根据编辑器类型显示对应的编辑器，隐藏其他
                    if (editorType == 0) { // YiEditor
                        ShowWindow(hEditorWnd, SW_SHOW);
                        ShowWindow(hEllEditorWnd, SW_HIDE);
                        // 切换到对应的文档
                        if (g_EditorData) {
                            int docIndex = g_EditorData->FindDocument(tabData->tabs[tabIndex].filePath);
                            if (docIndex >= 0) {
                                g_EditorData->SwitchToDocument(docIndex);
                                InvalidateRect(hEditorWnd, NULL, TRUE);
                            }
                        }
                    } else if (editorType == 1) { // EllEditor (包括 .ell, .edt 等)
                        ShowWindow(hEditorWnd, SW_HIDE);
                        ShowWindow(hEllEditorWnd, SW_SHOW);
                        // 切换到对应的文档
                        EllEditorData* dllData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                        if (dllData) {
                            // 加载对应的文件
                            dllData->LoadFile(tabData->tabs[tabIndex].filePath);
                            InvalidateRect(hEllEditorWnd, NULL, TRUE);
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
                            // 保存文件 - 使用统一的保存逻辑
                            if (g_EditorData) {
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
                                    
                                    if (!SaveDocument(hWnd, doc)) {
                                        MessageBoxW(hWnd, L"保存文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                        return 0;
                                    }
                                }
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
                    
                    // 检查是否还有其他标签（注意：CloseTab会删除标签，所以这里检查删除前的数量）
                    if (tabData->tabs.size() == 1) {
                        // 这是最后一个标签，关闭后显示欢迎页
                        ShowWindow(hEditorWnd, SW_HIDE);
                        ShowWindow(hEllEditorWnd, SW_HIDE);
                        ShowWindow(hWelcomePageWnd, SW_SHOW);
                        InvalidateRect(hWelcomePageWnd, NULL, TRUE);
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
                
                // 检查是否来自 EllEditor（TableEditor）
                EllEditorData* ellData = (EllEditorData*)GetWindowLongPtr(hEllEditorWnd, GWLP_USERDATA);
                if (ellData && ellData->editor && IsWindow(hSourceWnd)) {
                    // 检查源窗口是否是 EllEditor 或其子窗口
                    if (hSourceWnd == hEllEditorWnd || IsChild(hEllEditorWnd, hSourceWnd) || 
                        GetParent(hSourceWnd) == hEllEditorWnd) {
                        filePath = ellData->currentFilePath;
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
                SetTheme(true);
                SaveThemeConfig();
                // 销毁旧的菜单弹出窗口，下次点击时会重新创建
                if (g_MenuPopupWnd) {
                    DestroyWindow(g_MenuPopupWnd);
                    g_MenuPopupWnd = NULL;
                }
                InvalidateRect(hWnd, NULL, TRUE);
                InvalidateRect(hAIChatWnd, NULL, TRUE);
                InvalidateRect(hEditorWnd, NULL, TRUE);
                InvalidateRect(hRightPanelWnd, NULL, TRUE);
                InvalidateRect(hOutputWnd, NULL, TRUE);
                break;
            case IDM_THEME_LIGHT:
                SetTheme(false);
                SaveThemeConfig();
                // 销毁旧的菜单弹出窗口，下次点击时会重新创建
                if (g_MenuPopupWnd) {
                    DestroyWindow(g_MenuPopupWnd);
                    g_MenuPopupWnd = NULL;
                }
                InvalidateRect(hWnd, NULL, TRUE);
                InvalidateRect(hAIChatWnd, NULL, TRUE);
                InvalidateRect(hEditorWnd, NULL, TRUE);
                InvalidateRect(hRightPanelWnd, NULL, TRUE);
                InvalidateRect(hOutputWnd, NULL, TRUE);
                break;
            case IDM_THEME_CUSTOM:
                MessageBoxW(hWnd, 
                    L"主题配置功能正在开发中...\n\n"
                    L"当前可用功能：\n"
                    L"✓ 深色主题（默认）\n"
                    L"✓ 浅色主题\n\n"
                    L"即将推出：\n"
                    L"• 自定义标题栏颜色\n"
                    L"• 自定义菜单文本色\n"
                    L"• 自定义代码编辑器配色\n"
                    L"• 自定义AI聊天气泡颜色\n"
                    L"• 自定义语法高亮配色\n\n"
                    L"敬请期待！",
                    L"自定义主题配置", MB_OK | MB_ICONINFORMATION);
                break;
            case IDM_SYSTEM_CONFIG:
                MessageBoxW(hWnd, L"系统配置功能正在开发中...", L"系统配置", MB_OK | MB_ICONINFORMATION);
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
                    // 保存文件
                    if (g_EditorData) {
                        EditorDocument* doc = g_EditorData->GetActiveDoc();
                        if (doc) {
                            if (!doc->filePath.empty()) {
                                // 已有文件路径，直接保存
                                if (!SaveFile(doc->filePath, doc)) {
                                    MessageBoxW(hWnd, L"保存文件失败！", L"错误", MB_OK | MB_ICONERROR);
                                } else {
                                    InvalidateRect(hEditorWnd, NULL, TRUE);
                                }
                            } else {
                                // 没有文件路径，调用另存为
                                std::wstring savePath;
                                if (ShowSaveDialog(hWnd, savePath)) {
                                    if (SaveFile(savePath, doc)) {
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
                    // 创建新项目
                    std::wstring projectPath, projectName;
                    if (ShowCreateProjectDialog(hWnd, projectPath, projectName)) {
                        auto& pm = ProjectManager::GetInstance();
                        if (pm.CreateProject(projectPath, projectName)) {
                            // 关闭所有打开的文件
                            if (g_EditorData) {
                                while (!g_EditorData->documents.empty()) {
                                    g_EditorData->CloseDocument(0);
                                }
                            }
                            // 加载项目到资源管理器
                            ExplorerLoadProject();
                            // 更新菜单（添加"插入"菜单）
                            UpdateMenuItems();
                            InvalidateRect(hEditorWnd, NULL, TRUE);
                            MessageBoxW(hWnd, L"项目创建成功！", L"提示", MB_OK | MB_ICONINFORMATION);
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
                            // 加载项目到资源管理器
                            ExplorerLoadProject();
                            // 更新菜单（添加"插入"菜单）
                            UpdateMenuItems();
                            InvalidateRect(hEditorWnd, NULL, TRUE);
                            
                            // 打开主文件（如果有）
                            std::wstring mainFile = pm.GetMainFile();
                            if (!mainFile.empty() && g_EditorData) {
                                g_EditorData->AddDocument(mainFile);
                                EditorDocument* doc = g_EditorData->GetActiveDoc();
                                if (doc) {
                                    LoadFile(mainFile, doc);
                                }
                                InvalidateRect(hEditorWnd, NULL, TRUE);
                                RECT rect;
                                GetClientRect(hEditorWnd, &rect);
                                SendMessage(hEditorWnd, WM_SIZE, 0, MAKELPARAM(rect.right, rect.bottom));
                            }
                            
                            MessageBoxW(hWnd, L"项目打开成功！", L"提示", MB_OK | MB_ICONINFORMATION);
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
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORLISTBOX:
        {
            HDC hdc = (HDC)wParam;
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
            
            // 检查侧边栏切换按钮点击
            POINT ptClick = {x, y};
            if (PtInRect(&g_SidebarToggleRect, ptClick)) {
                g_LeftPanelVisible = !g_LeftPanelVisible;
                if (g_LeftPanelVisible) {
                    // 恢复之前保存的宽度
                    g_LeftPanelWidth = g_SavedLeftPanelWidth;
                } else {
                    // 保存当前宽度
                    g_SavedLeftPanelWidth = g_LeftPanelWidth;
                }
                // 触发窗口重新布局
                RECT rect;
                GetClientRect(hWnd, &rect);
                SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rect.right, rect.bottom));
                return 0;
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
                else if (PtInRect(&g_SidebarToggleRect, ptHover)) g_HoverButton = 4;
            }
            
            // 只在悬停状态改变时才重绘
            if (oldHover != g_HoverButton) {
                // 只重绘标题栏区域
                RECT titleRect = {0, 0, 0, g_TitleBarHeight};
                GetClientRect(hWnd, &titleRect);
                titleRect.bottom = g_TitleBarHeight;
                InvalidateRect(hWnd, &titleRect, FALSE);
            }
        }
        return 0;
        
    case WM_MOUSELEAVE:
        {
            // 鼠标离开窗口，清除所有悬停状态
            g_IsTrackingMouse = false;
            
            bool needRepaint = false;
            
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
            
            if (needRepaint) {
                RECT titleRect = {0, 0, 0, g_TitleBarHeight};
                GetClientRect(hWnd, &titleRect);
                titleRect.bottom = g_TitleBarHeight;
                InvalidateRect(hWnd, &titleRect, FALSE);
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
            // 当AI窗口隐藏时，左边缘可以调整窗口大小（扩大到8像素更容易抓取）
            const int leftBorderSize = g_LeftPanelVisible ? 0 : 8;
            bool onLeft = pt.x < leftBorderSize;
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
            
            // 检查是否在侧边栏切换按钮区域
            bool inSidebarButton = PtInRect(&g_SidebarToggleRect, pt);
            
            // 标题栏区域可以拖动窗口（但排除菜单区域、侧边栏按钮和右侧按钮区域）
            if (!inMenuArea && !inSidebarButton && pt.y < g_TitleBarHeight && pt.x < rect.right - 46 * 3) {
                return HTCAPTION;
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
            HBRUSH titleBrush = CreateSolidBrush(RGB(24, 24, 24));
            FillRect(memDC, &titleBarRect, titleBrush);
            DeleteObject(titleBrush);
            
            // 绘制标题栏底部1像素边框
            RECT borderRect = {0, g_TitleBarHeight - 1, rect.right, g_TitleBarHeight};
            HBRUSH borderBrush = CreateSolidBrush(RGB(45, 45, 45));
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
                    COLORREF hoverColor = (btnId == 3) ? RGB(232, 17, 35) : g_CurrentTheme.grid;
                    HBRUSH hoverBrush = CreateSolidBrush(hoverColor);
                    FillRect(memDC, &rc, hoverBrush);
                    DeleteObject(hoverBrush);
                }
                DrawTextW(memDC, symbol, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            };
            
            DrawButton(g_MinButtonRect, L"—", 1);
            DrawButton(g_MaxButtonRect, g_IsMaximized ? L"❐" : L"□", 2);
            DrawButton(g_CloseButtonRect, L"✕", 3);
            
            // 绘制侧边栏切换按钮
            {
                RECT rc = g_SidebarToggleRect;
                
                // 悬停背景
                if (g_HoverButton == 4) {
                    Color hoverColor(255, 45, 46, 46);
                    SolidBrush hoverBrush(hoverColor);
                    RectF rectF((REAL)rc.left, (REAL)rc.top, 
                        (REAL)(rc.right - rc.left), (REAL)(rc.bottom - rc.top));
                    GraphicsPath path;
                    float radius = 4.0f;
                    path.AddArc(rectF.X, rectF.Y, radius * 2, radius * 2, 180, 90);
                    path.AddArc(rectF.X + rectF.Width - radius * 2, rectF.Y, radius * 2, radius * 2, 270, 90);
                    path.AddArc(rectF.X + rectF.Width - radius * 2, rectF.Y + rectF.Height - radius * 2, radius * 2, radius * 2, 0, 90);
                    path.AddArc(rectF.X, rectF.Y + rectF.Height - radius * 2, radius * 2, radius * 2, 90, 90);
                    path.CloseFigure();
                    graphics.FillPath(&hoverBrush, &path);
                }
                
                // 加载并绘制 ai.png 图标
                wchar_t exePath[MAX_PATH];
                GetModuleFileNameW(NULL, exePath, MAX_PATH);
                std::wstring basePath(exePath);
                size_t lastSlash = basePath.find_last_of(L"\\");
                if (lastSlash != std::wstring::npos) {
                    basePath = basePath.substr(0, lastSlash);
                }
                
                // 尝试多个可能的路径
                std::wstring iconPath;
                std::vector<std::wstring> tryPaths = {
                    basePath + L"\\img\\ai.png",           // exe同目录下的img
                    basePath + L"\\..\\img\\ai.png",       // 上一级目录的img (x64\Debug -> x64)
                    basePath + L"\\..\\..\\img\\ai.png"    // 上两级目录的img (x64\Debug -> ycIDE)
                };
                
                Image* aiIcon = nullptr;
                for (const auto& path : tryPaths) {
                    aiIcon = Image::FromFile(path.c_str());
                    if (aiIcon && aiIcon->GetLastStatus() == Ok) {
                        break;
                    }
                    if (aiIcon) {
                        delete aiIcon;
                        aiIcon = nullptr;
                    }
                }
                
                if (aiIcon && aiIcon->GetLastStatus() == Ok) {
                    int iconSize = 20;
                    int cx = (rc.left + rc.right) / 2;
                    int cy = (rc.top + rc.bottom) / 2;
                    
                    // 如果侧边栏隐藏，降低图标透明度
                    if (!g_LeftPanelVisible) {
                        ColorMatrix colorMatrix = {
                            1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 0.5f, 0.0f,
                            0.0f, 0.0f, 0.0f, 0.0f, 1.0f
                        };
                        ImageAttributes imgAttr;
                        imgAttr.SetColorMatrix(&colorMatrix);
                        graphics.DrawImage(aiIcon, 
                            RectF((REAL)(cx - iconSize/2), (REAL)(cy - iconSize/2), (REAL)iconSize, (REAL)iconSize),
                            0, 0, (REAL)aiIcon->GetWidth(), (REAL)aiIcon->GetHeight(),
                            UnitPixel, &imgAttr);
                    } else {
                        graphics.DrawImage(aiIcon, cx - iconSize/2, cy - iconSize/2, iconSize, iconSize);
                    }
                    delete aiIcon;
                }
            }
            
            // 绘制状态栏顶部分隔线
            HPEN statusSepPen = CreatePen(PS_SOLID, 1, RGB(45, 45, 45));
            HPEN oldStatusPen = (HPEN)SelectObject(memDC, statusSepPen);
            int statusBarY = rect.bottom - g_StatusBarHeight;
            MoveToEx(memDC, 0, statusBarY, NULL);
            LineTo(memDC, rect.right, statusBarY);
            SelectObject(memDC, oldStatusPen);
            DeleteObject(statusSepPen);
            
            // 绘制窗口边框（1像素灰色）
            HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
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
            SetTextColor(hdcStatic, RGB(0, 0, 0));      // 黑色文字
            SetBkColor(hdcStatic, RGB(255, 255, 255));  // 白色背景
            return (INT_PTR)GetStockObject(WHITE_BRUSH);
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
                    
                    // 绘制悬停背景
                    if ((int)i == hoverIndex) {
                        HBRUSH hoverBrush = CreateSolidBrush(RGB(50, 50, 50));
                        FillRect(hdc, &itemRect, hoverBrush);
                        DeleteObject(hoverBrush);
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

// 状态栏子类化过程 - 让下边缘穿透
LRESULT CALLBACK StatusBarSubclassProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    switch (message)
    {
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
