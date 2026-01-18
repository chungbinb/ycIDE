/**
 * @file win_unit.cpp
 * @brief 窗口组件实现
 * @date 2026-01-18
 */

#include "../include_krnln_header.h"
#include "win_unit.h"
#include <stdlib.h>
#include <string.h>
#include <algorithm>

// 窗口类名
static const WCHAR g_szWindowClass[] = L"EPL_WINDOW";
static BOOL g_bWindowClassRegistered = FALSE;

// 前向声明
static LRESULT CALLBACK Window_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static void Window_RegisterClass();
static DWORD Window_GetStyle(PWINDOW_DATA pWindow);
static DWORD Window_GetExStyle(PWINDOW_DATA pWindow);

// ========== 注册窗口类 ==========
static void Window_RegisterClass()
{
    if (g_bWindowClassRegistered) return;
    
    WNDCLASSEXW wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wcex.lpfnWndProc = Window_WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(LPVOID);  // 存储窗口数据指针
    wcex.hInstance = GetModuleHandle(NULL);
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = g_szWindowClass;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if (RegisterClassExW(&wcex))
    {
        g_bWindowClassRegistered = TRUE;
    }
}

// ========== 计算窗口样式 ==========
static DWORD Window_GetStyle(PWINDOW_DATA pWindow)
{
    DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    
    switch (pWindow->nBorderStyle)
    {
    case WINDOW_BORDER_NONE:
        dwStyle |= WS_POPUP;
        break;
    case WINDOW_BORDER_FIXED_SINGLE:
        dwStyle |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION;
        break;
    case WINDOW_BORDER_SIZABLE:
        dwStyle |= WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION;
        break;
    case WINDOW_BORDER_FIXED_DIALOG:
        dwStyle |= WS_OVERLAPPED | WS_DLGFRAME | WS_CAPTION;
        break;
    case WINDOW_BORDER_FIXED_TOOL:
        dwStyle |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION;
        break;
    case WINDOW_BORDER_SIZABLE_TOOL:
        dwStyle |= WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION;
        break;
    default:
        dwStyle |= WS_OVERLAPPED | WS_THICKFRAME | WS_CAPTION;
        break;
    }
    
    if (pWindow->bControlBox)
        dwStyle |= WS_SYSMENU;
    if (pWindow->bMaxButton)
        dwStyle |= WS_MAXIMIZEBOX;
    if (pWindow->bMinButton)
        dwStyle |= WS_MINIMIZEBOX;
    
    if (pWindow->base.dwState & UNIT_STATE_VISIBLE)
        dwStyle |= WS_VISIBLE;
    
    return dwStyle;
}

static DWORD Window_GetExStyle(PWINDOW_DATA pWindow)
{
    DWORD dwExStyle = WS_EX_CONTROLPARENT;
    
    if (pWindow->nBorderStyle == WINDOW_BORDER_FIXED_TOOL ||
        pWindow->nBorderStyle == WINDOW_BORDER_SIZABLE_TOOL)
    {
        dwExStyle |= WS_EX_TOOLWINDOW;
    }
    
    if (pWindow->bTopMost)
        dwExStyle |= WS_EX_TOPMOST;
    
    if (pWindow->nOpacity < 255 || pWindow->clrTrans != CLR_INVALID)
        dwExStyle |= WS_EX_LAYERED;
    
    // 是否显示在任务栏
    if (!pWindow->bShowTaskbar)
        dwExStyle |= WS_EX_TOOLWINDOW;
    
    return dwExStyle;
}

// ========== 窗口过程 ==========
static LRESULT CALLBACK Window_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PWINDOW_DATA pWindow = (PWINDOW_DATA)GetWindowLongPtrW(hWnd, 0);
    
    if (uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT pCreate = (LPCREATESTRUCT)lParam;
        pWindow = (PWINDOW_DATA)pCreate->lpCreateParams;
        SetWindowLongPtrW(hWnd, 0, (LONG_PTR)pWindow);
        return TRUE;
    }
    
    if (!pWindow)
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    
    switch (uMsg)
    {
    case WM_CREATE:
        // 触发创建完毕事件
        Unit_FireEvent(&pWindow->base, EVENT_WINDOW_LOAD, 0);
        return 0;
        
    case WM_CLOSE:
        {
            // 触发可否被关闭事件
            EVENT_NOTIFY notify(pWindow->base.dwFormID, pWindow->base.dwUnitID, EVENT_WINDOW_CANCLOSE);
            notify.m_blHasRetVal = TRUE;
            notify.m_nRetVal = 1;  // 默认允许关闭
            
            // 如果返回0，则取消关闭
            if (notify.m_nRetVal == 0)
                return 0;
            
            // 如果是模态窗口，设置返回值
            if (pWindow->bIsModal)
            {
                pWindow->nModalResult = IDCANCEL;
            }
            
            pWindow->bIsClosing = TRUE;
        }
        break;
        
    case WM_DESTROY:
        // 触发将被销毁事件
        Unit_FireEvent(&pWindow->base, EVENT_WINDOW_UNLOAD, 0);
        
        // 如果是模态窗口，退出模态循环
        if (pWindow->bIsModal)
        {
            PostQuitMessage(pWindow->nModalResult);
        }
        
        // 清理资源
        SetWindowLongPtrW(hWnd, 0, 0);
        pWindow->base.hWnd = NULL;
        return 0;
        
    case WM_ACTIVATE:
        if (LOWORD(wParam) == WA_INACTIVE)
            Unit_FireEvent(&pWindow->base, EVENT_WINDOW_DEACTIVATE, 0);
        else
            Unit_FireEvent(&pWindow->base, EVENT_WINDOW_ACTIVATE, 0);
        break;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            // 绘制背景
            RECT rc;
            GetClientRect(hWnd, &rc);
            
            if (pWindow->hBackPic)
            {
                // 绘制底图
                HDC hdcMem = CreateCompatibleDC(hdc);
                HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, pWindow->hBackPic);
                
                BITMAP bm;
                GetObject(pWindow->hBackPic, sizeof(bm), &bm);
                
                switch (pWindow->nBackPicMode)
                {
                case BACKPIC_MODE_TOPLEFT:
                    BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
                    break;
                case BACKPIC_MODE_CENTER:
                    {
                        int x = (rc.right - bm.bmWidth) / 2;
                        int y = (rc.bottom - bm.bmHeight) / 2;
                        BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
                    }
                    break;
                case BACKPIC_MODE_STRETCH:
                    StretchBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
                    break;
                case BACKPIC_MODE_TILE:
                    for (int y = 0; y < rc.bottom; y += bm.bmHeight)
                    {
                        for (int x = 0; x < rc.right; x += bm.bmWidth)
                        {
                            BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
                        }
                    }
                    break;
                }
                
                SelectObject(hdcMem, hOldBmp);
                DeleteDC(hdcMem);
            }
            else
            {
                // 用背景颜色填充
                HBRUSH hBrush = CreateSolidBrush(pWindow->clrBack);
                FillRect(hdc, &rc, hBrush);
                DeleteObject(hBrush);
            }
            
            // 触发需要重画事件
            Unit_FireEvent(&pWindow->base, EVENT_WINDOW_PAINT, 1, (INT)(INT_PTR)hdc);
            
            EndPaint(hWnd, &ps);
        }
        return 0;
        
    case WM_SIZE:
        Unit_FireEvent(&pWindow->base, EVENT_WINDOW_RESIZE, 2, LOWORD(lParam), HIWORD(lParam));
        break;
        
    case WM_MOVE:
        Unit_FireEvent(&pWindow->base, EVENT_WINDOW_MOVE, 2, LOWORD(lParam), HIWORD(lParam));
        break;
        
    case WM_ERASEBKGND:
        // 我们自己绘制背景
        return 1;
        
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORSTATIC:
        // 为子控件设置默认背景
        {
            HDC hdcChild = (HDC)wParam;
            SetBkMode(hdcChild, TRANSPARENT);
            return (LRESULT)GetStockObject(NULL_BRUSH);
        }
        
    case WM_DROPFILES:
        {
            HDROP hDrop = (HDROP)wParam;
            UINT nCount = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
            
            if (nCount > 0)
            {
                // 触发拖放文件事件，参数为文件数量
                Unit_FireEvent(&pWindow->base, EVENT_WINDOW_DROPFILE, 1, (INT)nCount);
            }
            
            DragFinish(hDrop);
        }
        return 0;
        
    case WM_KEYDOWN:
        // 预处理按键事件
        Unit_FireEvent(&pWindow->base, EVENT_WINDOW_KEYDOWN, 1, (INT)wParam);
        break;
        
    case WM_CHAR:
        // 预处理字符输入事件
        Unit_FireEvent(&pWindow->base, EVENT_WINDOW_CHAR, 1, (INT)wParam);
        break;
        
    case WM_HSCROLL:
    case WM_VSCROLL:
        // 滚动条事件
        Unit_FireEvent(&pWindow->base, EVENT_WINDOW_SCROLL, 3, 
            uMsg == WM_HSCROLL ? 0 : 1,  // 方向：0=水平，1=垂直
            LOWORD(wParam),              // 操作类型
            HIWORD(wParam));             // 位置
        break;
        
    case WM_CONTEXTMENU:
        // 即将弹出菜单事件
        Unit_FireEvent(&pWindow->base, EVENT_WINDOW_CONTEXTMENU, 2,
            LOWORD(lParam), HIWORD(lParam));
        break;
    }
    
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

// ========== 创建窗口 ==========
HUNIT WINAPI Window_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    
    // 注册窗口类
    Window_RegisterClass();
    
    // 分配窗口数据
    PWINDOW_DATA pWindow = (PWINDOW_DATA)Unit_Alloc(sizeof(WINDOW_DATA) - sizeof(WIN_UNIT_BASE));
    if (!pWindow) return NULL;
    
    // 初始化基础数据
    pWindow->base.dwFormID = pParams->dwFormID;
    pWindow->base.dwUnitID = pParams->dwUnitID;
    pWindow->base.hParent = pParams->hParentWnd;
    
    // 设置默认值
    pWindow->clrBack = GetSysColor(COLOR_BTNFACE);
    pWindow->nBorderStyle = WINDOW_BORDER_SIZABLE;
    pWindow->nStartPos = WINDOW_STARTPOS_CENTER_SCREEN;
    pWindow->nBackPicMode = BACKPIC_MODE_TOPLEFT;
    pWindow->nOpacity = 255;
    pWindow->clrTrans = CLR_INVALID;
    pWindow->bMaxButton = TRUE;
    pWindow->bMinButton = TRUE;
    pWindow->bControlBox = TRUE;
    pWindow->bMoveable = TRUE;
    pWindow->bShowTaskbar = TRUE;
    pWindow->bTopMost = FALSE;
    pWindow->bSysMenu = TRUE;
    
    // 从属性数据读取属性
    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        LPSTR pszTitle = NULL;
        if (Unit_ReadPropText(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_TITLE, &pszTitle))
        {
            pWindow->pszTitle = Unit_AnsiToWide(pszTitle, NULL);
            free(pszTitle);
        }
        
        INT nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_BORDERSTYLE, &nValue))
            pWindow->nBorderStyle = nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_STARTPOS, &nValue))
            pWindow->nStartPos = nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_OPACITY, &nValue))
            pWindow->nOpacity = nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_BACKPICMODE, &nValue))
            pWindow->nBackPicMode = nValue;
        
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_MAXBUTTON, &bValue))
            pWindow->bMaxButton = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_MINBUTTON, &bValue))
            pWindow->bMinButton = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_CONTROLBOX, &bValue))
            pWindow->bControlBox = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_MOVEABLE, &bValue))
            pWindow->bMoveable = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_SHOWTASKBAR, &bValue))
            pWindow->bShowTaskbar = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_TOPMOST, &bValue))
            pWindow->bTopMost = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_SYSMENU, &bValue))
            pWindow->bSysMenu = bValue;
        
        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_BACKCOLOR, &clr))
            pWindow->clrBack = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_TRANSCOLOR, &clr))
            pWindow->clrTrans = clr;
        
        LOGFONTW lf;
        if (Unit_ReadPropFont(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_FONT, &lf))
            pWindow->hFont = Unit_CreateFont(&lf);
        
        // 固定属性
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_VISIBLE, &bValue))
        {
            if (bValue)
                pWindow->base.dwState |= UNIT_STATE_VISIBLE;
        }
        else
        {
            // 默认可见
            pWindow->base.dwState |= UNIT_STATE_VISIBLE;
        }
        
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_WINDOW_DISABLED, &bValue))
        {
            if (bValue)
                pWindow->base.dwState |= UNIT_STATE_DISABLED;
        }
    }
    else
    {
        // 没有属性数据，使用默认值
        pWindow->pszTitle = _wcsdup(L"窗口");
        pWindow->base.dwState |= UNIT_STATE_VISIBLE;
    }
    
    // 计算窗口样式
    DWORD dwStyle = Window_GetStyle(pWindow);
    DWORD dwExStyle = Window_GetExStyle(pWindow);
    
    // 计算窗口位置
    int x = pParams->x;
    int y = pParams->y;
    int cx = pParams->cx > 0 ? pParams->cx : 640;
    int cy = pParams->cy > 0 ? pParams->cy : 480;
    
    // 根据启动位置调整
    if (pWindow->nStartPos == WINDOW_STARTPOS_CENTER_SCREEN)
    {
        x = (GetSystemMetrics(SM_CXSCREEN) - cx) / 2;
        y = (GetSystemMetrics(SM_CYSCREEN) - cy) / 2;
    }
    else if (pWindow->nStartPos == WINDOW_STARTPOS_CENTER_OWNER && pParams->hParentWnd)
    {
        RECT rcParent;
        GetWindowRect(pParams->hParentWnd, &rcParent);
        x = rcParent.left + ((rcParent.right - rcParent.left) - cx) / 2;
        y = rcParent.top + ((rcParent.bottom - rcParent.top) - cy) / 2;
    }
    
    // 创建窗口
    HWND hWnd = CreateWindowExW(
        dwExStyle,
        g_szWindowClass,
        pWindow->pszTitle ? pWindow->pszTitle : L"",
        dwStyle,
        x, y, cx, cy,
        pParams->hParentWnd,
        pParams->hMenu,
        GetModuleHandle(NULL),
        pWindow
    );
    
    if (!hWnd)
    {
        Unit_Free((PWIN_UNIT_BASE)pWindow);
        return NULL;
    }
    
    pWindow->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)pWindow, hWnd);
    
    // 设置分层窗口属性
    if (pWindow->nOpacity < 255 || pWindow->clrTrans != CLR_INVALID)
    {
        DWORD dwFlags = 0;
        BYTE bAlpha = (BYTE)pWindow->nOpacity;
        COLORREF crKey = 0;
        
        if (pWindow->nOpacity < 255)
            dwFlags |= LWA_ALPHA;
        if (pWindow->clrTrans != CLR_INVALID)
        {
            dwFlags |= LWA_COLORKEY;
            crKey = pWindow->clrTrans;
        }
        
        SetLayeredWindowAttributes(hWnd, crKey, bAlpha, dwFlags);
    }
    
    // 设置字体
    if (pWindow->hFont)
    {
        SendMessage(hWnd, WM_SETFONT, (WPARAM)pWindow->hFont, TRUE);
    }
    
    // 设置图标
    if (pWindow->hIcon)
    {
        SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)pWindow->hIcon);
        SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)pWindow->hIcon);
    }
    
    // 启用文件拖放
    DragAcceptFiles(hWnd, TRUE);
    
    // 设计模式下不显示窗口
    if (pParams->blInDesignMode)
    {
        ShowWindow(hWnd, SW_HIDE);
    }
    
    return (HUNIT)pWindow;
}

// ========== 销毁窗口 ==========
BOOL WINAPI Window_Destroy(HUNIT hUnit)
{
    PWINDOW_DATA pWindow = (PWINDOW_DATA)hUnit;
    if (!pWindow) return FALSE;
    
    // 销毁Windows窗口
    if (pWindow->base.hWnd && IsWindow(pWindow->base.hWnd))
    {
        DestroyWindow(pWindow->base.hWnd);
    }
    
    // 释放资源
    if (pWindow->pszTitle)
    {
        free(pWindow->pszTitle);
    }
    if (pWindow->hFont)
    {
        DeleteObject(pWindow->hFont);
    }
    if (pWindow->hIcon)
    {
        DestroyIcon(pWindow->hIcon);
    }
    if (pWindow->hBackPic)
    {
        DeleteObject(pWindow->hBackPic);
    }
    
    Unit_Free((PWIN_UNIT_BASE)pWindow);
    return TRUE;
}

// ========== 窗口命令实现 ==========

void WINAPI Window_Show(PWINDOW_DATA pWindow)
{
    if (pWindow && pWindow->base.hWnd)
    {
        ShowWindow(pWindow->base.hWnd, SW_SHOW);
        UpdateWindow(pWindow->base.hWnd);
        pWindow->base.dwState |= UNIT_STATE_VISIBLE;
    }
}

void WINAPI Window_Hide(PWINDOW_DATA pWindow)
{
    if (pWindow && pWindow->base.hWnd)
    {
        ShowWindow(pWindow->base.hWnd, SW_HIDE);
        pWindow->base.dwState &= ~UNIT_STATE_VISIBLE;
    }
}

INT WINAPI Window_ShowModal(PWINDOW_DATA pWindow)
{
    if (!pWindow || !pWindow->base.hWnd) return -1;
    
    pWindow->bIsModal = TRUE;
    pWindow->nModalResult = 0;
    
    // 禁用父窗口
    HWND hParent = pWindow->base.hParent;
    if (hParent)
    {
        EnableWindow(hParent, FALSE);
    }
    
    // 显示窗口
    ShowWindow(pWindow->base.hWnd, SW_SHOW);
    UpdateWindow(pWindow->base.hWnd);
    
    // 进入消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!IsDialogMessage(pWindow->base.hWnd, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        
        // 检查窗口是否已关闭
        if (!IsWindow(pWindow->base.hWnd))
            break;
    }
    
    // 恢复父窗口
    if (hParent)
    {
        EnableWindow(hParent, TRUE);
        SetForegroundWindow(hParent);
    }
    
    pWindow->bIsModal = FALSE;
    return pWindow->nModalResult;
}

void WINAPI Window_Close(PWINDOW_DATA pWindow)
{
    if (pWindow && pWindow->base.hWnd)
    {
        SendMessage(pWindow->base.hWnd, WM_CLOSE, 0, 0);
    }
}

void WINAPI Window_Move(PWINDOW_DATA pWindow, INT x, INT y)
{
    if (pWindow && pWindow->base.hWnd)
    {
        SetWindowPos(pWindow->base.hWnd, NULL, x, y, 0, 0,
            SWP_NOSIZE | SWP_NOZORDER);
    }
}

void WINAPI Window_Resize(PWINDOW_DATA pWindow, INT cx, INT cy)
{
    if (pWindow && pWindow->base.hWnd)
    {
        SetWindowPos(pWindow->base.hWnd, NULL, 0, 0, cx, cy,
            SWP_NOMOVE | SWP_NOZORDER);
    }
}

void WINAPI Window_Center(PWINDOW_DATA pWindow, BOOL bRelativeToScreen)
{
    if (!pWindow || !pWindow->base.hWnd) return;
    
    RECT rcWindow;
    GetWindowRect(pWindow->base.hWnd, &rcWindow);
    int cx = rcWindow.right - rcWindow.left;
    int cy = rcWindow.bottom - rcWindow.top;
    
    int x, y;
    
    if (bRelativeToScreen)
    {
        x = (GetSystemMetrics(SM_CXSCREEN) - cx) / 2;
        y = (GetSystemMetrics(SM_CYSCREEN) - cy) / 2;
    }
    else if (pWindow->base.hParent)
    {
        RECT rcParent;
        GetWindowRect(pWindow->base.hParent, &rcParent);
        x = rcParent.left + ((rcParent.right - rcParent.left) - cx) / 2;
        y = rcParent.top + ((rcParent.bottom - rcParent.top) - cy) / 2;
    }
    else
    {
        x = (GetSystemMetrics(SM_CXSCREEN) - cx) / 2;
        y = (GetSystemMetrics(SM_CYSCREEN) - cy) / 2;
    }
    
    SetWindowPos(pWindow->base.hWnd, NULL, x, y, 0, 0,
        SWP_NOSIZE | SWP_NOZORDER);
}

void WINAPI Window_SetTopMost(PWINDOW_DATA pWindow, BOOL bTopMost)
{
    if (pWindow && pWindow->base.hWnd)
    {
        pWindow->bTopMost = bTopMost;
        SetWindowPos(pWindow->base.hWnd,
            bTopMost ? HWND_TOPMOST : HWND_NOTOPMOST,
            0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}

void WINAPI Window_Activate(PWINDOW_DATA pWindow)
{
    if (pWindow && pWindow->base.hWnd)
    {
        SetForegroundWindow(pWindow->base.hWnd);
    }
}

void WINAPI Window_SetTitle(PWINDOW_DATA pWindow, LPCWSTR pszTitle)
{
    if (pWindow)
    {
        if (pWindow->pszTitle)
            free(pWindow->pszTitle);
        pWindow->pszTitle = pszTitle ? _wcsdup(pszTitle) : NULL;
        
        if (pWindow->base.hWnd)
        {
            SetWindowTextW(pWindow->base.hWnd, pszTitle ? pszTitle : L"");
        }
    }
}

LPCWSTR WINAPI Window_GetTitle(PWINDOW_DATA pWindow)
{
    return pWindow ? pWindow->pszTitle : NULL;
}

void WINAPI Window_SetBackColor(PWINDOW_DATA pWindow, COLORREF clr)
{
    if (pWindow)
    {
        pWindow->clrBack = clr;
        if (pWindow->base.hWnd)
        {
            InvalidateRect(pWindow->base.hWnd, NULL, TRUE);
        }
    }
}

COLORREF WINAPI Window_GetBackColor(PWINDOW_DATA pWindow)
{
    return pWindow ? pWindow->clrBack : 0;
}

void WINAPI Window_SetOpacity(PWINDOW_DATA pWindow, INT nOpacity)
{
    if (pWindow && pWindow->base.hWnd)
    {
        pWindow->nOpacity = std::max(0, std::min(255, nOpacity));
        
        // 更新分层窗口
        DWORD dwExStyle = GetWindowLong(pWindow->base.hWnd, GWL_EXSTYLE);
        if (!(dwExStyle & WS_EX_LAYERED))
        {
            SetWindowLong(pWindow->base.hWnd, GWL_EXSTYLE, dwExStyle | WS_EX_LAYERED);
        }
        
        SetLayeredWindowAttributes(pWindow->base.hWnd, 0, (BYTE)pWindow->nOpacity, LWA_ALPHA);
    }
}

INT WINAPI Window_GetOpacity(PWINDOW_DATA pWindow)
{
    return pWindow ? pWindow->nOpacity : 255;
}

void WINAPI Window_Invalidate(PWINDOW_DATA pWindow, BOOL bErase)
{
    if (pWindow && pWindow->base.hWnd)
    {
        InvalidateRect(pWindow->base.hWnd, NULL, bErase);
    }
}

void WINAPI Window_Update(PWINDOW_DATA pWindow)
{
    if (pWindow && pWindow->base.hWnd)
    {
        UpdateWindow(pWindow->base.hWnd);
    }
}

void WINAPI Window_ClientToScreen(PWINDOW_DATA pWindow, LPPOINT ppt)
{
    if (pWindow && pWindow->base.hWnd && ppt)
    {
        ClientToScreen(pWindow->base.hWnd, ppt);
    }
}

void WINAPI Window_ScreenToClient(PWINDOW_DATA pWindow, LPPOINT ppt)
{
    if (pWindow && pWindow->base.hWnd && ppt)
    {
        ScreenToClient(pWindow->base.hWnd, ppt);
    }
}

// ========== 属性操作 ==========

BOOL WINAPI Window_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PWINDOW_DATA pWindow = (PWINDOW_DATA)hUnit;
    if (!pWindow || !pValue) return FALSE;
    
    RECT rc;
    if (pWindow->base.hWnd)
    {
        GetWindowRect(pWindow->base.hWnd, &rc);
    }
    else
    {
        SetRect(&rc, 0, 0, 640, 480);
    }
    
    switch (nIndex)
    {
    case PROP_WINDOW_LEFT:
        pValue->m_int = rc.left;
        return TRUE;
    case PROP_WINDOW_TOP:
        pValue->m_int = rc.top;
        return TRUE;
    case PROP_WINDOW_WIDTH:
        pValue->m_int = rc.right - rc.left;
        return TRUE;
    case PROP_WINDOW_HEIGHT:
        pValue->m_int = rc.bottom - rc.top;
        return TRUE;
    case PROP_WINDOW_VISIBLE:
        pValue->m_bool = (pWindow->base.dwState & UNIT_STATE_VISIBLE) != 0;
        return TRUE;
    case PROP_WINDOW_DISABLED:
        pValue->m_bool = (pWindow->base.dwState & UNIT_STATE_DISABLED) != 0;
        return TRUE;
    case PROP_WINDOW_TITLE:
        pValue->m_szText = pWindow->pszTitle ? Unit_WideToAnsi(pWindow->pszTitle, NULL) : NULL;
        return TRUE;
    case PROP_WINDOW_BORDERSTYLE:
        pValue->m_int = pWindow->nBorderStyle;
        return TRUE;
    case PROP_WINDOW_BACKCOLOR:
        pValue->m_clr = pWindow->clrBack;
        return TRUE;
    case PROP_WINDOW_TOPMOST:
        pValue->m_bool = pWindow->bTopMost;
        return TRUE;
    case PROP_WINDOW_OPACITY:
        pValue->m_int = pWindow->nOpacity;
        return TRUE;
    case PROP_WINDOW_MAXBUTTON:
        pValue->m_bool = pWindow->bMaxButton;
        return TRUE;
    case PROP_WINDOW_MINBUTTON:
        pValue->m_bool = pWindow->bMinButton;
        return TRUE;
    case PROP_WINDOW_CONTROLBOX:
        pValue->m_bool = pWindow->bControlBox;
        return TRUE;
    default:
        return FALSE;
    }
}

BOOL WINAPI Window_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PWINDOW_DATA pWindow = (PWINDOW_DATA)hUnit;
    if (!pWindow || !pValue) return FALSE;
    
    switch (nIndex)
    {
    case PROP_WINDOW_LEFT:
    case PROP_WINDOW_TOP:
    case PROP_WINDOW_WIDTH:
    case PROP_WINDOW_HEIGHT:
        {
            RECT rc;
            if (pWindow->base.hWnd)
            {
                GetWindowRect(pWindow->base.hWnd, &rc);
                if (nIndex == PROP_WINDOW_LEFT)
                    rc.left = pValue->m_int;
                else if (nIndex == PROP_WINDOW_TOP)
                    rc.top = pValue->m_int;
                else if (nIndex == PROP_WINDOW_WIDTH)
                    rc.right = rc.left + pValue->m_int;
                else
                    rc.bottom = rc.top + pValue->m_int;
                    
                SetWindowPos(pWindow->base.hWnd, NULL,
                    rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                    SWP_NOZORDER);
            }
        }
        return TRUE;
        
    case PROP_WINDOW_VISIBLE:
        if (pValue->m_bool)
        {
            Window_Show(pWindow);
        }
        else
        {
            Window_Hide(pWindow);
        }
        return TRUE;
        
    case PROP_WINDOW_DISABLED:
        if (pValue->m_bool)
            pWindow->base.dwState |= UNIT_STATE_DISABLED;
        else
            pWindow->base.dwState &= ~UNIT_STATE_DISABLED;
        if (pWindow->base.hWnd)
        {
            EnableWindow(pWindow->base.hWnd, !pValue->m_bool);
        }
        return TRUE;
        
    case PROP_WINDOW_TITLE:
        if (pValue->m_szText)
        {
            LPWSTR pszWide = Unit_AnsiToWide(pValue->m_szText, NULL);
            Window_SetTitle(pWindow, pszWide);
            Unit_FreeString(pszWide);
        }
        else
        {
            Window_SetTitle(pWindow, NULL);
        }
        return TRUE;
        
    case PROP_WINDOW_BACKCOLOR:
        Window_SetBackColor(pWindow, pValue->m_clr);
        return TRUE;
        
    case PROP_WINDOW_TOPMOST:
        Window_SetTopMost(pWindow, pValue->m_bool);
        return TRUE;
        
    case PROP_WINDOW_OPACITY:
        Window_SetOpacity(pWindow, pValue->m_int);
        return TRUE;
        
    // 以下属性需要重新创建窗口
    case PROP_WINDOW_BORDERSTYLE:
    case PROP_WINDOW_MAXBUTTON:
    case PROP_WINDOW_MINBUTTON:
    case PROP_WINDOW_CONTROLBOX:
        if (ppszTip)
            *ppszTip = "修改此属性需要重新创建窗口";
        return TRUE;  // 返回TRUE表示需要重建
        
    default:
        return FALSE;
    }
}

BOOL WINAPI Window_PropertyUpdateUI(HUNIT hUnit, INT nIndex)
{
    // 所有属性默认可用
    return TRUE;
}

HGLOBAL WINAPI Window_GetAllPropertyData(HUNIT hUnit)
{
    // TODO: 实现获取所有属性数据
    return NULL;
}

// ========== 接口获取 ==========

static HUNIT WINAPI Window_Interface_Create(LPBYTE pAllData, INT nAllDataSize,
    DWORD dwStyle, HWND hParentWnd, UINT uID, HMENU hMenu, INT x, INT y, INT cx, INT cy,
    DWORD dwWinFormID, DWORD dwUnitID, HWND hDesignWnd, BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS params;
    params.pAllData = pAllData;
    params.nAllDataSize = nAllDataSize;
    params.dwStyle = dwStyle;
    params.hParentWnd = hParentWnd;
    params.uID = uID;
    params.hMenu = hMenu;
    params.x = x;
    params.y = y;
    params.cx = cx;
    params.cy = cy;
    params.dwFormID = dwWinFormID;
    params.dwUnitID = dwUnitID;
    params.hDesignWnd = hDesignWnd;
    params.blInDesignMode = blInDesignMode;
    
    return Window_Create(&params);
}

PFN_INTERFACE WINAPI Window_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:
        return (PFN_INTERFACE)Window_Interface_Create;
    case ITF_GET_PROPERTY_DATA:
        return (PFN_INTERFACE)Window_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED:
        return (PFN_INTERFACE)Window_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:
        return (PFN_INTERFACE)Window_PropertyUpdateUI;
    case ITF_GET_ALL_PROPERTY_DATA:
        return (PFN_INTERFACE)Window_GetAllPropertyData;
    default:
        return NULL;
    }
}
