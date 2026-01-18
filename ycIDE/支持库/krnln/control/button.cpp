/**
 * @file button.cpp
 * @brief 按钮组件实现
 * @date 2026-01-18
 */

#include "../include_krnln_header.h"
#include "button.h"
#include <stdlib.h>
#include <string.h>
#include <commctrl.h>

// 前向声明
static LRESULT CALLBACK Button_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ========== 创建按钮 ==========
HUNIT WINAPI Button_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    
    // 分配按钮数据
    PBUTTON_DATA pButton = (PBUTTON_DATA)Unit_Alloc(sizeof(BUTTON_DATA) - sizeof(WIN_UNIT_BASE));
    if (!pButton) return NULL;
    
    // 初始化基础数据
    pButton->base.dwFormID = pParams->dwFormID;
    pButton->base.dwUnitID = pParams->dwUnitID;
    pButton->base.hParent = pParams->hParentWnd;
    
    // 设置默认值
    pButton->clrBack = GetSysColor(COLOR_BTNFACE);
    pButton->clrFore = GetSysColor(COLOR_BTNTEXT);
    pButton->nAlign = BUTTON_ALIGN_CENTER;
    pButton->bFlat = FALSE;
    pButton->bDefault = FALSE;
    pButton->bHover = FALSE;
    pButton->bPressed = FALSE;
    
    // 从属性数据读取属性
    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        LPSTR pszCaption = NULL;
        if (Unit_ReadPropText(pParams->pAllData, pParams->nAllDataSize, PROP_BUTTON_CAPTION, &pszCaption))
        {
            pButton->pszCaption = Unit_AnsiToWide(pszCaption, NULL);
            free(pszCaption);
        }
        
        INT nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_BUTTON_ALIGN, &nValue))
            pButton->nAlign = nValue;
        
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_BUTTON_FLAT, &bValue))
            pButton->bFlat = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_BUTTON_DEFAULT, &bValue))
            pButton->bDefault = bValue;
        
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_BUTTON_VISIBLE, &bValue))
        {
            if (bValue)
                pButton->base.dwState |= UNIT_STATE_VISIBLE;
        }
        else
        {
            pButton->base.dwState |= UNIT_STATE_VISIBLE;
        }
        
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_BUTTON_DISABLED, &bValue))
        {
            if (bValue)
                pButton->base.dwState |= UNIT_STATE_DISABLED;
        }
        
        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_BUTTON_BACKCOLOR, &clr))
            pButton->clrBack = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_BUTTON_FORECOLOR, &clr))
            pButton->clrFore = clr;
        
        LOGFONTW lf;
        if (Unit_ReadPropFont(pParams->pAllData, pParams->nAllDataSize, PROP_BUTTON_FONT, &lf))
            pButton->hFont = Unit_CreateFont(&lf);
    }
    else
    {
        pButton->pszCaption = _wcsdup(L"按钮");
        pButton->base.dwState |= UNIT_STATE_VISIBLE;
    }
    
    // 计算按钮样式
    DWORD dwStyle = WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON | BS_NOTIFY;
    
    if (pButton->base.dwState & UNIT_STATE_VISIBLE)
        dwStyle |= WS_VISIBLE;
    if (pButton->base.dwState & UNIT_STATE_DISABLED)
        dwStyle |= WS_DISABLED;
    if (pButton->bDefault)
        dwStyle |= BS_DEFPUSHBUTTON;
    if (pButton->bFlat)
        dwStyle |= BS_FLAT;
    
    // 创建Windows按钮
    int x = pParams->x;
    int y = pParams->y;
    int cx = pParams->cx > 0 ? pParams->cx : 75;
    int cy = pParams->cy > 0 ? pParams->cy : 25;
    
    HWND hWnd = CreateWindowExW(
        0,
        L"BUTTON",
        pButton->pszCaption ? pButton->pszCaption : L"",
        dwStyle,
        x, y, cx, cy,
        pParams->hParentWnd,
        (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL),
        NULL
    );
    
    if (!hWnd)
    {
        Unit_Free((PWIN_UNIT_BASE)pButton);
        return NULL;
    }
    
    pButton->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)pButton, hWnd);
    
    // 子类化按钮
    pButton->base.pfnOldProc = (WNDPROC)SetWindowLongPtrW(
        hWnd, GWLP_WNDPROC, (LONG_PTR)Button_SubclassProc);
    
    // 存储按钮数据到窗口
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)pButton);
    
    // 设置字体
    if (pButton->hFont)
    {
        SendMessage(hWnd, WM_SETFONT, (WPARAM)pButton->hFont, TRUE);
    }
    else
    {
        // 使用默认字体
        HFONT hDefaultFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        SendMessage(hWnd, WM_SETFONT, (WPARAM)hDefaultFont, TRUE);
    }
    
    return (HUNIT)pButton;
}

// ========== 销毁按钮 ==========
BOOL WINAPI Button_Destroy(HUNIT hUnit)
{
    PBUTTON_DATA pButton = (PBUTTON_DATA)hUnit;
    if (!pButton) return FALSE;
    
    // 取消子类化
    if (pButton->base.hWnd && pButton->base.pfnOldProc)
    {
        SetWindowLongPtrW(pButton->base.hWnd, GWLP_WNDPROC, (LONG_PTR)pButton->base.pfnOldProc);
    }
    
    // 销毁Windows窗口
    if (pButton->base.hWnd && IsWindow(pButton->base.hWnd))
    {
        DestroyWindow(pButton->base.hWnd);
    }
    
    // 释放资源
    if (pButton->pszCaption)
    {
        free(pButton->pszCaption);
    }
    if (pButton->hFont)
    {
        DeleteObject(pButton->hFont);
    }
    if (pButton->hPicture)
    {
        DeleteObject(pButton->hPicture);
    }
    
    Unit_Free((PWIN_UNIT_BASE)pButton);
    return TRUE;
}

// ========== 子类化过程 ==========
static LRESULT CALLBACK Button_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PBUTTON_DATA pButton = (PBUTTON_DATA)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (!pButton || !pButton->base.pfnOldProc)
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    
    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
        pButton->bPressed = TRUE;
        Unit_FireEvent(&pButton->base, EVENT_MOUSEDOWN, 3,
            LOWORD(lParam), HIWORD(lParam), (INT)wParam);
        break;
        
    case WM_LBUTTONUP:
        {
            BOOL bWasPressed = pButton->bPressed;
            pButton->bPressed = FALSE;
            
            Unit_FireEvent(&pButton->base, EVENT_MOUSEUP, 3,
                LOWORD(lParam), HIWORD(lParam), (INT)wParam);
            
            // 检查是否在按钮范围内释放，触发点击事件
            if (bWasPressed)
            {
                RECT rc;
                GetClientRect(hWnd, &rc);
                POINT pt = { LOWORD(lParam), HIWORD(lParam) };
                if (PtInRect(&rc, pt))
                {
                    Unit_FireEvent(&pButton->base, EVENT_BUTTON_CLICK, 0);
                }
            }
        }
        break;
        
    case WM_LBUTTONDBLCLK:
        Unit_FireEvent(&pButton->base, EVENT_BUTTON_DBLCLICK, 0);
        break;
        
    case WM_MOUSEMOVE:
        Unit_FireEvent(&pButton->base, EVENT_MOUSEMOVE, 3,
            LOWORD(lParam), HIWORD(lParam), (INT)wParam);
        
        // 跟踪鼠标离开
        if (!pButton->bHover)
        {
            pButton->bHover = TRUE;
            Unit_FireEvent(&pButton->base, EVENT_MOUSEENTER, 0);
            
            TRACKMOUSEEVENT tme = {0};
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
        }
        break;
        
    case WM_MOUSELEAVE:
        pButton->bHover = FALSE;
        pButton->bPressed = FALSE;
        Unit_FireEvent(&pButton->base, EVENT_MOUSELEAVE, 0);
        break;
        
    case WM_KEYDOWN:
        Unit_FireEvent(&pButton->base, EVENT_KEYDOWN, 1, (INT)wParam);
        
        // 空格键和回车键触发点击
        if (wParam == VK_SPACE || wParam == VK_RETURN)
        {
            pButton->bPressed = TRUE;
        }
        break;
        
    case WM_KEYUP:
        Unit_FireEvent(&pButton->base, EVENT_KEYUP, 1, (INT)wParam);
        
        if ((wParam == VK_SPACE || wParam == VK_RETURN) && pButton->bPressed)
        {
            pButton->bPressed = FALSE;
            Unit_FireEvent(&pButton->base, EVENT_BUTTON_CLICK, 0);
        }
        break;
        
    case WM_SETFOCUS:
        pButton->base.dwState |= UNIT_STATE_FOCUSED;
        Unit_FireEvent(&pButton->base, EVENT_GOTFOCUS, 0);
        break;
        
    case WM_KILLFOCUS:
        pButton->base.dwState &= ~UNIT_STATE_FOCUSED;
        pButton->bPressed = FALSE;
        Unit_FireEvent(&pButton->base, EVENT_LOSTFOCUS, 0);
        break;
        
    case WM_DESTROY:
        Unit_FireEvent(&pButton->base, EVENT_DESTROY, 0);
        break;
    }
    
    return CallWindowProcW(pButton->base.pfnOldProc, hWnd, uMsg, wParam, lParam);
}

// ========== 按钮命令实现 ==========

void WINAPI Button_SetCaption(PBUTTON_DATA pButton, LPCWSTR pszCaption)
{
    if (pButton)
    {
        if (pButton->pszCaption)
            free(pButton->pszCaption);
        pButton->pszCaption = pszCaption ? _wcsdup(pszCaption) : NULL;
        
        if (pButton->base.hWnd)
        {
            SetWindowTextW(pButton->base.hWnd, pszCaption ? pszCaption : L"");
        }
    }
}

LPCWSTR WINAPI Button_GetCaption(PBUTTON_DATA pButton)
{
    return pButton ? pButton->pszCaption : NULL;
}

void WINAPI Button_SetFont(PBUTTON_DATA pButton, const LOGFONTW* pLogFont)
{
    if (pButton && pLogFont)
    {
        if (pButton->hFont)
            DeleteObject(pButton->hFont);
        pButton->hFont = Unit_CreateFont(pLogFont);
        
        if (pButton->base.hWnd)
        {
            SendMessage(pButton->base.hWnd, WM_SETFONT, (WPARAM)pButton->hFont, TRUE);
        }
    }
}

void WINAPI Button_SetPicture(PBUTTON_DATA pButton, HBITMAP hBitmap)
{
    if (pButton)
    {
        if (pButton->hPicture)
            DeleteObject(pButton->hPicture);
        pButton->hPicture = hBitmap;
        
        if (pButton->base.hWnd)
        {
            // 设置按钮图片（需要BS_BITMAP样式）
            LONG_PTR dwStyle = GetWindowLongPtrW(pButton->base.hWnd, GWL_STYLE);
            if (hBitmap)
            {
                SetWindowLongPtrW(pButton->base.hWnd, GWL_STYLE, dwStyle | BS_BITMAP);
                SendMessage(pButton->base.hWnd, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
            }
            else
            {
                SetWindowLongPtrW(pButton->base.hWnd, GWL_STYLE, dwStyle & ~BS_BITMAP);
                SendMessage(pButton->base.hWnd, BM_SETIMAGE, IMAGE_BITMAP, 0);
            }
            InvalidateRect(pButton->base.hWnd, NULL, TRUE);
        }
    }
}

void WINAPI Button_Click(PBUTTON_DATA pButton)
{
    if (pButton)
    {
        // 触发点击事件
        Unit_FireEvent(&pButton->base, EVENT_BUTTON_CLICK, 0);
    }
}

// ========== 属性操作 ==========

BOOL WINAPI Button_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PBUTTON_DATA pButton = (PBUTTON_DATA)hUnit;
    if (!pButton || !pValue) return FALSE;
    
    RECT rc = {0};
    if (pButton->base.hWnd)
    {
        GetWindowRect(pButton->base.hWnd, &rc);
        MapWindowPoints(HWND_DESKTOP, pButton->base.hParent, (LPPOINT)&rc, 2);
    }
    
    switch (nIndex)
    {
    case PROP_BUTTON_LEFT:
        pValue->m_int = rc.left;
        return TRUE;
    case PROP_BUTTON_TOP:
        pValue->m_int = rc.top;
        return TRUE;
    case PROP_BUTTON_WIDTH:
        pValue->m_int = rc.right - rc.left;
        return TRUE;
    case PROP_BUTTON_HEIGHT:
        pValue->m_int = rc.bottom - rc.top;
        return TRUE;
    case PROP_BUTTON_VISIBLE:
        pValue->m_bool = (pButton->base.dwState & UNIT_STATE_VISIBLE) != 0;
        return TRUE;
    case PROP_BUTTON_DISABLED:
        pValue->m_bool = (pButton->base.dwState & UNIT_STATE_DISABLED) != 0;
        return TRUE;
    case PROP_BUTTON_CAPTION:
        pValue->m_szText = pButton->pszCaption ? Unit_WideToAnsi(pButton->pszCaption, NULL) : NULL;
        return TRUE;
    case PROP_BUTTON_ALIGN:
        pValue->m_int = pButton->nAlign;
        return TRUE;
    case PROP_BUTTON_FLAT:
        pValue->m_bool = pButton->bFlat;
        return TRUE;
    case PROP_BUTTON_DEFAULT:
        pValue->m_bool = pButton->bDefault;
        return TRUE;
    case PROP_BUTTON_BACKCOLOR:
        pValue->m_clr = pButton->clrBack;
        return TRUE;
    case PROP_BUTTON_FORECOLOR:
        pValue->m_clr = pButton->clrFore;
        return TRUE;
    default:
        return FALSE;
    }
}

BOOL WINAPI Button_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PBUTTON_DATA pButton = (PBUTTON_DATA)hUnit;
    if (!pButton || !pValue) return FALSE;
    
    switch (nIndex)
    {
    case PROP_BUTTON_LEFT:
    case PROP_BUTTON_TOP:
    case PROP_BUTTON_WIDTH:
    case PROP_BUTTON_HEIGHT:
        if (pButton->base.hWnd)
        {
            RECT rc;
            GetWindowRect(pButton->base.hWnd, &rc);
            MapWindowPoints(HWND_DESKTOP, pButton->base.hParent, (LPPOINT)&rc, 2);
            
            if (nIndex == PROP_BUTTON_LEFT)
                rc.left = pValue->m_int;
            else if (nIndex == PROP_BUTTON_TOP)
                rc.top = pValue->m_int;
            else if (nIndex == PROP_BUTTON_WIDTH)
                rc.right = rc.left + pValue->m_int;
            else
                rc.bottom = rc.top + pValue->m_int;
            
            SetWindowPos(pButton->base.hWnd, NULL,
                rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                SWP_NOZORDER);
        }
        return FALSE;  // 不需要重建
        
    case PROP_BUTTON_VISIBLE:
        if (pValue->m_bool)
        {
            pButton->base.dwState |= UNIT_STATE_VISIBLE;
            if (pButton->base.hWnd)
                ShowWindow(pButton->base.hWnd, SW_SHOW);
        }
        else
        {
            pButton->base.dwState &= ~UNIT_STATE_VISIBLE;
            if (pButton->base.hWnd)
                ShowWindow(pButton->base.hWnd, SW_HIDE);
        }
        return FALSE;
        
    case PROP_BUTTON_DISABLED:
        if (pValue->m_bool)
            pButton->base.dwState |= UNIT_STATE_DISABLED;
        else
            pButton->base.dwState &= ~UNIT_STATE_DISABLED;
        if (pButton->base.hWnd)
            EnableWindow(pButton->base.hWnd, !pValue->m_bool);
        return FALSE;
        
    case PROP_BUTTON_CAPTION:
        if (pValue->m_szText)
        {
            LPWSTR pszWide = Unit_AnsiToWide(pValue->m_szText, NULL);
            Button_SetCaption(pButton, pszWide);
            Unit_FreeString(pszWide);
        }
        else
        {
            Button_SetCaption(pButton, NULL);
        }
        return FALSE;
        
    case PROP_BUTTON_FLAT:
        pButton->bFlat = pValue->m_bool;
        if (ppszTip)
            *ppszTip = "修改此属性需要重新创建组件";
        return TRUE;  // 需要重建
        
    case PROP_BUTTON_DEFAULT:
        pButton->bDefault = pValue->m_bool;
        if (ppszTip)
            *ppszTip = "修改此属性需要重新创建组件";
        return TRUE;
        
    case PROP_BUTTON_BACKCOLOR:
        pButton->clrBack = pValue->m_clr;
        if (pButton->base.hWnd)
            InvalidateRect(pButton->base.hWnd, NULL, TRUE);
        return FALSE;
        
    case PROP_BUTTON_FORECOLOR:
        pButton->clrFore = pValue->m_clr;
        if (pButton->base.hWnd)
            InvalidateRect(pButton->base.hWnd, NULL, TRUE);
        return FALSE;
        
    default:
        return FALSE;
    }
}

BOOL WINAPI Button_PropertyUpdateUI(HUNIT hUnit, INT nIndex)
{
    return TRUE;  // 所有属性默认可用
}

// ========== 接口获取 ==========

static HUNIT WINAPI Button_Interface_Create(LPBYTE pAllData, INT nAllDataSize,
    DWORD dwStyle, HWND hParentWnd, UINT uID, HMENU hMenu, INT x, INT y, INT cx, INT cy,
    DWORD dwWinFormID, DWORD dwUnitID, HWND hDesignWnd, BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS params = {0};
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
    
    return Button_Create(&params);
}

PFN_INTERFACE WINAPI Button_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:
        return (PFN_INTERFACE)Button_Interface_Create;
    case ITF_GET_PROPERTY_DATA:
        return (PFN_INTERFACE)Button_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED:
        return (PFN_INTERFACE)Button_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:
        return (PFN_INTERFACE)Button_PropertyUpdateUI;
    default:
        return NULL;
    }
}
