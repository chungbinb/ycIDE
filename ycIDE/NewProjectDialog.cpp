#include "NewProjectDialog.h"
#include "Theme.h"
#include <shlobj.h>
#include <commctrl.h>
#include <windowsx.h>
#include <vector>
#include <algorithm>  // for std::min

// 静态 GDI 资源（用于主题）
static HBRUSH s_hBgBrush = NULL;
static HBRUSH s_hEditBgBrush = NULL;

// 控件ID
#define IDC_PROJECT_LIST        1001
#define IDC_PROJECT_NAME        1002
#define IDC_PROJECT_LOCATION    1003
#define IDC_BROWSE_BUTTON       1004
#define IDC_PROJECT_DESC        1005
#define IDC_CREATE_BUTTON       IDOK
#define IDC_CANCEL_BUTTON       IDCANCEL
#define IDC_CLOSE_BUTTON        1010
#define IDC_INFO_TYPE           1011
#define IDC_INFO_DESC           1012
#define IDC_INFO_OUTPUT         1013

// 自定义标题栏高度
#define TITLE_BAR_HEIGHT        32
#define BORDER_WIDTH            1

// 项目类型信息
struct ProjectTypeInfo {
    ProjectType type;
    const wchar_t* name;
    const wchar_t* description;
    const wchar_t* iconChar;  // 使用字符作为简单图标
};

static const ProjectTypeInfo g_projectTypes[] = {
    { ProjectType::Console, L"Windows控制台程序", 
      L"创建命令行控制台应用程序", L">" },
    { ProjectType::WindowsApp, L"Windows窗口程序", 
      L"创建带有界面的桌面应用程序", L"□" },
    { ProjectType::DynamicLibrary, L"动态链接库 (DLL)", 
      L"创建可被其它程序调用的动态链接库", L"▣" }
};

// 获取输出类型
const wchar_t* GetProjectOutputType(ProjectType type) {
    switch (type) {
    case ProjectType::Console:
    case ProjectType::WindowsApp:
        return L".exe";
    case ProjectType::DynamicLibrary:
        return L".dll";
    default:
        return L".exe";
    }
}

// 获取默认项目名称
const wchar_t* GetDefaultProjectName(ProjectType type) {
    switch (type) {
    case ProjectType::Console:
        return L"新控制台项目";
    case ProjectType::WindowsApp:
        return L"新窗口项目";
    case ProjectType::DynamicLibrary:
        return L"新DLL项目";
    default:
        return L"新项目";
    }
}

// 静态成员初始化
NewProjectResult NewProjectDialog::s_result;

// 获取项目类型名称
const wchar_t* GetProjectTypeName(ProjectType type) {
    for (const auto& info : g_projectTypes) {
        if (info.type == type) return info.name;
    }
    return L"未知";
}

// 获取项目类型简短描述（用于左侧列表）
const wchar_t* GetProjectTypeDescription(ProjectType type) {
    for (const auto& info : g_projectTypes) {
        if (info.type == type) return info.description;
    }
    return L"";
}

// 获取项目类型详细描述（用于右侧详情面板）
const wchar_t* GetProjectTypeFullDescription(ProjectType type) {
    switch (type) {
    case ProjectType::Console:
        return L"创建在命令行中运行的控制台应用程序。适用于自动化任务、批处理程序、命令行工具等场景。";
    case ProjectType::WindowsApp:
        return L"创建带有图形界面的Windows窗口程序。可以设计窗体和控件，适用于桌面应用开发。";
    case ProjectType::DynamicLibrary:
        return L"创建可被其他程序调用的动态链接库文件。适用于封装功能模块、插件开发等场景。";
    default:
        return L"";
    }
}

// 显示新建项目对话框
NewProjectResult NewProjectDialog::Show(HWND hParent) {
    s_result = NewProjectResult();
    
    // 创建模态对话框
    // 由于没有资源模板，我们动态创建对话框
    
    // 对话框模板结构
    #pragma pack(push, 4)
    struct {
        DLGTEMPLATE dlg;
        WORD menu;
        WORD windowClass;
        WCHAR title[32];
    } dlgTemplate;
    #pragma pack(pop)
    
    memset(&dlgTemplate, 0, sizeof(dlgTemplate));
    // 无边框弹出窗口
    dlgTemplate.dlg.style = WS_POPUP | DS_CENTER;
    dlgTemplate.dlg.dwExtendedStyle = 0;
    dlgTemplate.dlg.cdit = 0;  // 控件数量，我们在WM_INITDIALOG中动态创建
    dlgTemplate.dlg.x = 0;
    dlgTemplate.dlg.y = 0;
    dlgTemplate.dlg.cx = 480;  // 对话框宽度（对话框单位）- 加宽以适应左右结构
    dlgTemplate.dlg.cy = 260;  // 对话框高度（对话框单位）- 增加标题栏高度
    dlgTemplate.menu = 0;
    dlgTemplate.windowClass = 0;
    wcscpy_s(dlgTemplate.title, L"新建项目");
    
    INT_PTR result = DialogBoxIndirectW(
        GetModuleHandle(NULL),
        &dlgTemplate.dlg,
        hParent,
        DialogProc
    );
    
    if (result == IDOK) {
        s_result.success = true;
    }
    
    return s_result;
}

// 绘制自定义标题栏
static void DrawTitleBar(HWND hDlg, HDC hdc) {
    RECT rcClient;
    GetClientRect(hDlg, &rcClient);
    
    // 标题栏区域
    RECT rcTitle = { BORDER_WIDTH, BORDER_WIDTH, rcClient.right - BORDER_WIDTH, TITLE_BAR_HEIGHT };
    HBRUSH hTitleBrush = CreateSolidBrush(g_CurrentTheme.titleBarBg);
    FillRect(hdc, &rcTitle, hTitleBrush);
    DeleteObject(hTitleBrush);
    
    // 绘制标题文字
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, g_CurrentTheme.text);
    HFONT hFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    
    RECT rcText = rcTitle;
    rcText.left += 12;
    DrawTextW(hdc, L"新建项目", -1, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
    
    // 绘制窗口边框
    HPEN hPen = CreatePen(PS_SOLID, BORDER_WIDTH, g_CurrentTheme.border);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, 0, 0, rcClient.right, rcClient.bottom);
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

// 绘制关闭按钮
static void DrawCloseButton(HWND hBtn, LPDRAWITEMSTRUCT lpDIS) {
    HDC hdc = lpDIS->hDC;
    RECT rc = lpDIS->rcItem;
    
    // 背景色
    COLORREF bgColor = g_CurrentTheme.titleBarBg;
    if (lpDIS->itemState & ODS_SELECTED) {
        bgColor = g_CurrentTheme.closeButtonHover;
    } else {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(hBtn, &pt);
        if (PtInRect(&rc, pt)) {
            bgColor = g_CurrentTheme.closeButtonHover;
        }
    }
    
    HBRUSH hBrush = CreateSolidBrush(bgColor);
    FillRect(hdc, &rc, hBrush);
    DeleteObject(hBrush);
    
    // 绘制 X
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, g_CurrentTheme.text);
    HFONT hFont = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe MDL2 Assets");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    DrawTextW(hdc, L"\xE8BB", -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);  // Chrome close icon
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

// 绘制普通按钮（主题色）
static void DrawThemedButton(LPDRAWITEMSTRUCT lpDIS, bool isDefault = false) {
    HDC hdc = lpDIS->hDC;
    RECT rc = lpDIS->rcItem;
    
    // 获取按钮文本
    wchar_t text[64] = {0};
    GetWindowTextW(lpDIS->hwndItem, text, 64);
    
    // 获取蓝色强调色
    COLORREF accentColor = g_CurrentTheme.statusBarBg;
    int r = GetRValue(accentColor);
    int g = GetGValue(accentColor);
    int b = GetBValue(accentColor);
    if (r + g + b < 100) {
        accentColor = RGB(0, 122, 204);  // VS Code 蓝色
    }
    
    // 背景色
    COLORREF bgColor, textColor, borderColor;
    if (lpDIS->itemState & ODS_SELECTED) {
        // 按下状态 - 稍微变暗
        bgColor = RGB(GetRValue(accentColor) * 80 / 100, 
                      GetGValue(accentColor) * 80 / 100, 
                      GetBValue(accentColor) * 80 / 100);
        textColor = RGB(255, 255, 255);
        borderColor = bgColor;
    } else if (isDefault) {
        // 默认按钮使用强调色
        bgColor = accentColor;
        textColor = RGB(255, 255, 255);
        borderColor = accentColor;
    } else {
        // 普通按钮
        bgColor = g_CurrentTheme.bg;
        textColor = g_CurrentTheme.text;
        borderColor = g_CurrentTheme.border;
    }
    
    // 检查鼠标悬停
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(lpDIS->hwndItem, &pt);
    if (PtInRect(&rc, pt) && !(lpDIS->itemState & ODS_SELECTED)) {
        if (isDefault) {
            // 默认按钮悬停时稍微变亮
            bgColor = RGB((std::min)(255, GetRValue(accentColor) + 20), 
                          (std::min)(255, GetGValue(accentColor) + 20), 
                          (std::min)(255, GetBValue(accentColor) + 20));
        } else {
            bgColor = g_CurrentTheme.menuHoverBg;
        }
    }
    
    // 填充背景
    HBRUSH hBrush = CreateSolidBrush(bgColor);
    FillRect(hdc, &rc, hBrush);
    DeleteObject(hBrush);
    
    // 绘制边框
    HPEN hPen = CreatePen(PS_SOLID, 1, borderColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    
    // 绘制文本
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, textColor);
    HFONT hFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    DrawTextW(hdc, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

// 对话框过程
INT_PTR CALLBACK NewProjectDialog::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        // 创建主题画刷
        s_hBgBrush = CreateSolidBrush(g_CurrentTheme.bg);
        s_hEditBgBrush = CreateSolidBrush(g_CurrentTheme.editorBg);
        OnInitDialog(hDlg);
        return TRUE;
    
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hDlg, &ps);
            DrawTitleBar(hDlg, hdc);
            EndPaint(hDlg, &ps);
        }
        return TRUE;
    
    case WM_NCHITTEST:
        {
            // 允许通过标题栏拖动窗口
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient(hDlg, &pt);
            
            RECT rcClient;
            GetClientRect(hDlg, &rcClient);
            
            // 标题栏区域（排除关闭按钮）
            if (pt.y >= BORDER_WIDTH && pt.y < TITLE_BAR_HEIGHT && 
                pt.x >= BORDER_WIDTH && pt.x < rcClient.right - 46) {
                SetWindowLongPtr(hDlg, DWLP_MSGRESULT, HTCAPTION);
                return TRUE;
            }
        }
        break;
        
    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_CLOSE_BUTTON) {
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        OnCommand(hDlg, wParam, lParam);
        return TRUE;
        
    case WM_DRAWITEM:
        {
            LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
            if (wParam == IDC_CLOSE_BUTTON) {
                DrawCloseButton(GetDlgItem(hDlg, IDC_CLOSE_BUTTON), lpDIS);
                return TRUE;
            }
            if (wParam == IDC_CREATE_BUTTON) {
                DrawThemedButton(lpDIS, true);  // 默认按钮
                return TRUE;
            }
            if (wParam == IDC_CANCEL_BUTTON || wParam == IDC_BROWSE_BUTTON) {
                DrawThemedButton(lpDIS, false);
                return TRUE;
            }
            OnDrawItem(hDlg, lpDIS);
        }
        return TRUE;
    
    case WM_CTLCOLORDLG:
        return (INT_PTR)s_hBgBrush;
        
    case WM_CTLCOLORSTATIC:
        SetBkColor((HDC)wParam, g_CurrentTheme.bg);
        SetTextColor((HDC)wParam, g_CurrentTheme.text);
        return (INT_PTR)s_hBgBrush;
        
    case WM_CTLCOLOREDIT:
        SetBkColor((HDC)wParam, g_CurrentTheme.editorBg);
        SetTextColor((HDC)wParam, g_CurrentTheme.text);
        return (INT_PTR)s_hEditBgBrush;
        
    case WM_CTLCOLORLISTBOX:
        SetBkColor((HDC)wParam, g_CurrentTheme.explorerBg);
        SetTextColor((HDC)wParam, g_CurrentTheme.text);
        return (INT_PTR)s_hBgBrush;
        
    case WM_DESTROY:
        // 清理画刷
        if (s_hBgBrush) { DeleteObject(s_hBgBrush); s_hBgBrush = NULL; }
        if (s_hEditBgBrush) { DeleteObject(s_hEditBgBrush); s_hEditBgBrush = NULL; }
        break;
        
    case WM_CLOSE:
        EndDialog(hDlg, IDCANCEL);
        return TRUE;
    }
    return FALSE;
}

// 初始化对话框
void NewProjectDialog::OnInitDialog(HWND hDlg) {
    // 获取对话框客户区大小
    RECT rcClient;
    GetClientRect(hDlg, &rcClient);
    int dlgWidth = rcClient.right;
    int dlgHeight = rcClient.bottom;
    
    // 内容区域起始Y（标题栏下方）
    int contentTop = TITLE_BAR_HEIGHT + BORDER_WIDTH;
    
    // 边距和间距
    int margin = 15;
    int spacing = 10;
    int leftPanelWidth = 320;  // 左侧面板宽度
    int rightPanelX = leftPanelWidth + margin + spacing + BORDER_WIDTH;  // 右侧面板起始X
    int rightPanelWidth = dlgWidth - rightPanelX - margin - BORDER_WIDTH;  // 右侧面板宽度
    
    // 创建字体
    HFONT hFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
    
    HFONT hFontBold = CreateFontW(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
    
    // ========== 关闭按钮 ==========
    HWND hClose = CreateWindowW(L"BUTTON", L"",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        dlgWidth - 46 - BORDER_WIDTH, BORDER_WIDTH, 46, TITLE_BAR_HEIGHT - BORDER_WIDTH,
        hDlg, (HMENU)IDC_CLOSE_BUTTON, GetModuleHandle(NULL), NULL);
    
    // ========== 左侧面板：项目类型 ==========
    int leftY = contentTop + margin;
    
    // 左侧标题 "项目类型"
    HWND hLabelLeft = CreateWindowW(L"STATIC", L"项目类型",
        WS_CHILD | WS_VISIBLE,
        margin + BORDER_WIDTH, leftY, leftPanelWidth, 20,
        hDlg, NULL, GetModuleHandle(NULL), NULL);
    SendMessage(hLabelLeft, WM_SETFONT, (WPARAM)hFontBold, TRUE);
    leftY += 25;
    
    // 项目类型列表 (使用ListBox，自绘模式，无边框)
    int listHeight = dlgHeight - leftY - margin - 45 - BORDER_WIDTH;  // 留出底部按钮空间
    HWND hList = CreateWindowExW(0, L"LISTBOX", NULL,
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS,
        margin + BORDER_WIDTH, leftY, leftPanelWidth, listHeight,
        hDlg, (HMENU)IDC_PROJECT_LIST, GetModuleHandle(NULL), NULL);
    SendMessage(hList, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(hList, LB_SETITEMHEIGHT, 0, 60);  // 每项高度
    
    // 添加项目类型
    for (const auto& info : g_projectTypes) {
        int idx = (int)SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)info.name);
        SendMessageW(hList, LB_SETITEMDATA, idx, (LPARAM)info.type);
    }
    SendMessage(hList, LB_SETCURSEL, 0, 0);  // 默认选中第一项
    
    // ========== 右侧面板：项目配置 + 详细信息 ==========
    int rightY = contentTop + margin;
    
    // 项目名称标签
    HWND hLabel2 = CreateWindowW(L"STATIC", L"项目名称:",
        WS_CHILD | WS_VISIBLE,
        rightPanelX, rightY, 80, 20,
        hDlg, NULL, GetModuleHandle(NULL), NULL);
    SendMessage(hLabel2, WM_SETFONT, (WPARAM)hFont, TRUE);
    rightY += 22;
    
    // 项目名称输入框（根据默认项目类型设置默认名称）
    HWND hName = CreateWindowExW(0, L"EDIT", GetDefaultProjectName(ProjectType::Console),
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
        rightPanelX, rightY, rightPanelWidth, 24,
        hDlg, (HMENU)IDC_PROJECT_NAME, GetModuleHandle(NULL), NULL);
    SendMessage(hName, WM_SETFONT, (WPARAM)hFont, TRUE);
    rightY += 32;
    
    // 项目位置标签
    HWND hLabel3 = CreateWindowW(L"STATIC", L"保存位置:",
        WS_CHILD | WS_VISIBLE,
        rightPanelX, rightY, 80, 20,
        hDlg, NULL, GetModuleHandle(NULL), NULL);
    SendMessage(hLabel3, WM_SETFONT, (WPARAM)hFont, TRUE);
    rightY += 22;
    
    // 获取默认位置：我的文档\ycIDE Projects
    wchar_t defaultPath[MAX_PATH] = { 0 };
    if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, 0, defaultPath))) {
        wcscat_s(defaultPath, L"\\ycIDE Projects");
    }
    
    // 项目位置输入框
    HWND hLocation = CreateWindowExW(0, L"EDIT", defaultPath,
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_BORDER,
        rightPanelX, rightY, rightPanelWidth - 35, 24,
        hDlg, (HMENU)IDC_PROJECT_LOCATION, GetModuleHandle(NULL), NULL);
    SendMessage(hLocation, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // 浏览按钮（自绘）
    HWND hBrowse = CreateWindowW(L"BUTTON", L"...",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        rightPanelX + rightPanelWidth - 30, rightY, 30, 24,
        hDlg, (HMENU)IDC_BROWSE_BUTTON, GetModuleHandle(NULL), NULL);
    SendMessage(hBrowse, WM_SETFONT, (WPARAM)hFont, TRUE);
    rightY += 38;
    
    // ========== 项目详细信息 ==========
    // 标题 "项目详细信息"
    HWND hLabelRight = CreateWindowW(L"STATIC", L"项目详细信息",
        WS_CHILD | WS_VISIBLE,
        rightPanelX, rightY, rightPanelWidth, 20,
        hDlg, NULL, GetModuleHandle(NULL), NULL);
    SendMessage(hLabelRight, WM_SETFONT, (WPARAM)hFontBold, TRUE);
    rightY += 25;
    
    // 类型标签
    HWND hTypeLabel = CreateWindowW(L"STATIC", L"类型：",
        WS_CHILD | WS_VISIBLE,
        rightPanelX, rightY, 50, 20,
        hDlg, NULL, GetModuleHandle(NULL), NULL);
    SendMessage(hTypeLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // 类型值
    HWND hTypeValue = CreateWindowW(L"STATIC", L"Console 控制台程序",
        WS_CHILD | WS_VISIBLE,
        rightPanelX + 50, rightY, rightPanelWidth - 50, 20,
        hDlg, (HMENU)IDC_INFO_TYPE, GetModuleHandle(NULL), NULL);
    SendMessage(hTypeValue, WM_SETFONT, (WPARAM)hFont, TRUE);
    rightY += 25;
    
    // 说明标签
    HWND hDescLabel = CreateWindowW(L"STATIC", L"说明：",
        WS_CHILD | WS_VISIBLE,
        rightPanelX, rightY, rightPanelWidth, 20,
        hDlg, NULL, GetModuleHandle(NULL), NULL);
    SendMessage(hDescLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    rightY += 22;
    
    // 说明内容
    HWND hDesc = CreateWindowW(L"STATIC", GetProjectTypeFullDescription(ProjectType::Console),
        WS_CHILD | WS_VISIBLE | SS_LEFT,
        rightPanelX, rightY, rightPanelWidth, 45,
        hDlg, (HMENU)IDC_INFO_DESC, GetModuleHandle(NULL), NULL);
    SendMessage(hDesc, WM_SETFONT, (WPARAM)hFont, TRUE);
    rightY += 50;
    
    // 输出类型标签
    HWND hOutputLabel = CreateWindowW(L"STATIC", L"输出类型：",
        WS_CHILD | WS_VISIBLE,
        rightPanelX, rightY, 80, 20,
        hDlg, NULL, GetModuleHandle(NULL), NULL);
    SendMessage(hOutputLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // 输出类型值
    HWND hOutputValue = CreateWindowW(L"STATIC", L".exe",
        WS_CHILD | WS_VISIBLE,
        rightPanelX + 80, rightY, rightPanelWidth - 80, 20,
        hDlg, (HMENU)IDC_INFO_OUTPUT, GetModuleHandle(NULL), NULL);
    SendMessage(hOutputValue, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // ========== 底部按钮区域 ==========
    int btnWidth = 80;
    int btnHeight = 28;
    int btnSpacing = 10;
    int btnY = dlgHeight - margin - btnHeight - BORDER_WIDTH;
    
    // 取消按钮（右侧，自绘）
    HWND hCancel = CreateWindowW(L"BUTTON", L"取消",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        dlgWidth - margin - btnWidth - BORDER_WIDTH, btnY, btnWidth, btnHeight,
        hDlg, (HMENU)IDC_CANCEL_BUTTON, GetModuleHandle(NULL), NULL);
    SendMessage(hCancel, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // 创建按钮（自绘）
    HWND hCreate = CreateWindowW(L"BUTTON", L"创建",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
        dlgWidth - margin - btnWidth * 2 - btnSpacing - BORDER_WIDTH, btnY, btnWidth, btnHeight,
        hDlg, (HMENU)IDC_CREATE_BUTTON, GetModuleHandle(NULL), NULL);
    SendMessage(hCreate, WM_SETFONT, (WPARAM)hFont, TRUE);
    
    // 设置焦点到项目名称输入框
    SetFocus(hName);
    SendMessage(hName, EM_SETSEL, 0, -1);
}

// 处理命令
void NewProjectDialog::OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam) {
    int wmId = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);
    
    switch (wmId) {
    case IDC_PROJECT_LIST:
        if (wmEvent == LBN_SELCHANGE) {
            // 更新详细信息
            HWND hList = GetDlgItem(hDlg, IDC_PROJECT_LIST);
            int sel = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR) {
                ProjectType type = (ProjectType)SendMessage(hList, LB_GETITEMDATA, sel, 0);
                
                // 更新类型
                std::wstring typeStr;
                switch (type) {
                case ProjectType::Console:
                    typeStr = L"Console 控制台程序";
                    break;
                case ProjectType::WindowsApp:
                    typeStr = L"WindowsApp 窗口程序";
                    break;
                case ProjectType::DynamicLibrary:
                    typeStr = L"DynamicLibrary 动态链接库";
                    break;
                }
                SetDlgItemTextW(hDlg, IDC_INFO_TYPE, typeStr.c_str());
                
                // 更新说明
                SetDlgItemTextW(hDlg, IDC_INFO_DESC, GetProjectTypeFullDescription(type));
                
                // 更新输出类型
                SetDlgItemTextW(hDlg, IDC_INFO_OUTPUT, GetProjectOutputType(type));
                
                // 更新默认项目名称
                SetDlgItemTextW(hDlg, IDC_PROJECT_NAME, GetDefaultProjectName(type));
                // 选中全部文本，方便用户直接输入
                HWND hName = GetDlgItem(hDlg, IDC_PROJECT_NAME);
                SendMessage(hName, EM_SETSEL, 0, -1);
                SetFocus(hName);
            }
        }
        break;
        
    case IDC_BROWSE_BUTTON:
        BrowseFolder(hDlg);
        break;
        
    case IDC_CREATE_BUTTON:
        if (ValidateInput(hDlg, s_result)) {
            EndDialog(hDlg, IDOK);
        }
        break;
        
    case IDC_CANCEL_BUTTON:
        EndDialog(hDlg, IDCANCEL);
        break;
    }
}

// 绘制项目类型项
void NewProjectDialog::OnDrawItem(HWND hDlg, LPDRAWITEMSTRUCT lpDIS) {
    if (lpDIS->CtlID != IDC_PROJECT_LIST) return;
    if (lpDIS->itemID == (UINT)-1) return;
    
    HDC hdc = lpDIS->hDC;
    RECT rc = lpDIS->rcItem;
    
    // 获取项目类型信息
    int idx = lpDIS->itemID;
    if (idx >= 0 && idx < _countof(g_projectTypes)) {
        const ProjectTypeInfo& info = g_projectTypes[idx];
        
        // 背景色 - 使用主题色
        COLORREF bgColor, textColor;
        if (lpDIS->itemState & ODS_SELECTED) {
            bgColor = g_CurrentTheme.explorerSelection;
            textColor = g_CurrentTheme.textBright;
        } else {
            bgColor = g_CurrentTheme.explorerBg;
            textColor = g_CurrentTheme.text;
        }
        
        // 填充背景
        HBRUSH hBrush = CreateSolidBrush(bgColor);
        FillRect(hdc, &rc, hBrush);
        DeleteObject(hBrush);
        
        // 绘制图标区域
        RECT rcIcon = rc;
        rcIcon.left += 8;
        rcIcon.right = rcIcon.left + 44;
        rcIcon.top += 8;
        rcIcon.bottom -= 8;
        
        // 图标背景 - 使用主题强调色（状态栏背景通常是蓝色）
        COLORREF iconBgColor = g_CurrentTheme.statusBarBg;
        // 如果状态栏背景太暗，使用一个默认的蓝色
        int r = GetRValue(iconBgColor);
        int g = GetGValue(iconBgColor);
        int b = GetBValue(iconBgColor);
        if (r + g + b < 100) {
            iconBgColor = RGB(0, 122, 204);  // VS Code 蓝色
        }
        HBRUSH hIconBg = CreateSolidBrush(iconBgColor);
        FillRect(hdc, &rcIcon, hIconBg);
        DeleteObject(hIconBg);
        
        // 绘制图标字符
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255, 255, 255));
        HFONT hIconFont = CreateFontW(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Consolas");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hIconFont);
        DrawTextW(hdc, info.iconChar, -1, &rcIcon, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        SelectObject(hdc, hOldFont);
        DeleteObject(hIconFont);
        
        // 绘制名称
        RECT rcText = rc;
        rcText.left = rcIcon.right + 12;
        rcText.top += 12;
        rcText.bottom = rcText.top + 22;
        
        SetTextColor(hdc, textColor);
        HFONT hNameFont = CreateFontW(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
        hOldFont = (HFONT)SelectObject(hdc, hNameFont);
        DrawTextW(hdc, info.name, -1, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
        SelectObject(hdc, hOldFont);
        DeleteObject(hNameFont);
        
        // 绘制简短描述
        rcText.top = rcText.bottom + 2;
        rcText.bottom = rc.bottom - 8;
        rcText.right -= 5;
        COLORREF descColor = (lpDIS->itemState & ODS_SELECTED) ? 
            g_CurrentTheme.textBright : g_CurrentTheme.textDim;
        SetTextColor(hdc, descColor);
        HFONT hDescFont = CreateFontW(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Microsoft YaHei UI");
        hOldFont = (HFONT)SelectObject(hdc, hDescFont);
        
        // 截取简短描述 - 左侧加宽后可显示更多字符
        std::wstring shortDesc = info.description;
        if (shortDesc.length() > 32) {
            shortDesc = shortDesc.substr(0, 29) + L"...";
        }
        DrawTextW(hdc, shortDesc.c_str(), -1, &rcText, DT_LEFT | DT_TOP | DT_SINGLELINE);
        SelectObject(hdc, hOldFont);
        DeleteObject(hDescFont);
        
        // 焦点矩形
        if (lpDIS->itemState & ODS_FOCUS) {
            DrawFocusRect(hdc, &rc);
        }
    }
}

// 浏览文件夹
void NewProjectDialog::BrowseFolder(HWND hDlg) {
    BROWSEINFOW bi = { 0 };
    bi.hwndOwner = hDlg;
    bi.lpszTitle = L"选择项目保存位置";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    
    LPITEMIDLIST pidl = SHBrowseForFolderW(&bi);
    if (pidl) {
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDListW(pidl, path)) {
            SetDlgItemTextW(hDlg, IDC_PROJECT_LOCATION, path);
        }
        CoTaskMemFree(pidl);
    }
}

// 验证输入
bool NewProjectDialog::ValidateInput(HWND hDlg, NewProjectResult& result) {
    // 获取项目名称
    wchar_t name[256] = { 0 };
    GetDlgItemTextW(hDlg, IDC_PROJECT_NAME, name, 256);
    
    if (wcslen(name) == 0) {
        MessageBoxW(hDlg, L"请输入项目名称", L"提示", MB_OK | MB_ICONWARNING);
        SetFocus(GetDlgItem(hDlg, IDC_PROJECT_NAME));
        return false;
    }
    
    // 检查项目名称是否包含非法字符
    const wchar_t* invalidChars = L"\\/:*?\"<>|";
    for (const wchar_t* p = invalidChars; *p; ++p) {
        if (wcschr(name, *p)) {
            MessageBoxW(hDlg, L"项目名称不能包含以下字符: \\ / : * ? \" < > |", 
                L"提示", MB_OK | MB_ICONWARNING);
            SetFocus(GetDlgItem(hDlg, IDC_PROJECT_NAME));
            return false;
        }
    }
    
    // 获取保存位置
    wchar_t location[MAX_PATH] = { 0 };
    GetDlgItemTextW(hDlg, IDC_PROJECT_LOCATION, location, MAX_PATH);
    
    if (wcslen(location) == 0) {
        MessageBoxW(hDlg, L"请选择项目保存位置", L"提示", MB_OK | MB_ICONWARNING);
        SetFocus(GetDlgItem(hDlg, IDC_PROJECT_LOCATION));
        return false;
    }
    
    // 获取选中的项目类型
    HWND hList = GetDlgItem(hDlg, IDC_PROJECT_LIST);
    int sel = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
    if (sel == LB_ERR) {
        MessageBoxW(hDlg, L"请选择项目类型", L"提示", MB_OK | MB_ICONWARNING);
        return false;
    }
    
    // 构建项目路径
    // 创建目录结构: location\projectName\projectName.epp
    std::wstring projectDir = location;
    projectDir += L"\\";
    projectDir += name;
    
    // 创建项目目录
    if (GetFileAttributesW(location) == INVALID_FILE_ATTRIBUTES) {
        // 父目录不存在，尝试创建
        if (!CreateDirectoryW(location, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
            MessageBoxW(hDlg, L"无法创建项目目录，请检查路径是否正确", 
                L"错误", MB_OK | MB_ICONERROR);
            return false;
        }
    }
    
    // 检查项目目录是否已存在
    if (GetFileAttributesW(projectDir.c_str()) != INVALID_FILE_ATTRIBUTES) {
        int ret = MessageBoxW(hDlg, 
            L"该项目目录已存在，是否覆盖？\n（现有文件可能被删除）", 
            L"确认", MB_YESNO | MB_ICONQUESTION);
        if (ret != IDYES) {
            return false;
        }
    } else {
        // 创建项目目录
        if (!CreateDirectoryW(projectDir.c_str(), NULL)) {
            MessageBoxW(hDlg, L"无法创建项目目录", L"错误", MB_OK | MB_ICONERROR);
            return false;
        }
    }
    
    // 填充结果
    result.type = (ProjectType)SendMessage(hList, LB_GETITEMDATA, sel, 0);
    result.projectName = name;
    result.projectDir = projectDir;
    result.projectPath = projectDir + L"\\" + name + L".epp";
    
    return true;
}

// 获取选中的项目类型
ProjectType NewProjectDialog::GetSelectedProjectType(HWND hDlg) {
    HWND hList = GetDlgItem(hDlg, IDC_PROJECT_LIST);
    int sel = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
    if (sel != LB_ERR) {
        return (ProjectType)SendMessage(hList, LB_GETITEMDATA, sel, 0);
    }
    return ProjectType::Console;
}
