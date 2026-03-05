/**
 * @file label.cpp
 * @brief 标签组件实现
 * @date 2026-03-05
 */

#define _CRT_SECURE_NO_WARNINGS

#include "../include_krnln_header.h"
#include "label.h"
#include <stdlib.h>
#include <string.h>

// 前向声明
static LRESULT CALLBACK Label_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ========== 创建标签 ==========
HUNIT WINAPI Label_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;

    PLABEL_DATA pLabel = (PLABEL_DATA)Unit_Alloc(sizeof(LABEL_DATA) - sizeof(WIN_UNIT_BASE));
    if (!pLabel) return NULL;

    pLabel->base.dwFormID    = pParams->dwFormID;
    pLabel->base.dwUnitID    = pParams->dwUnitID;
    pLabel->base.hParent     = pParams->hParentWnd;

    // 默认值
    pLabel->clrBack          = GetSysColor(COLOR_BTNFACE);
    pLabel->clrFore          = GetSysColor(COLOR_BTNTEXT);
    pLabel->nAlign           = LABEL_ALIGN_LEFT;
    pLabel->nVAlign          = LABEL_VALIGN_TOP;
    pLabel->nBorderStyle     = LABEL_BORDER_NONE;
    pLabel->bAutoSize        = FALSE;
    pLabel->bWordWrap        = FALSE;
    pLabel->bTransparent     = FALSE;

    // 读取属性数据
    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        LPSTR pszCaption = NULL;
        if (Unit_ReadPropText(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_CAPTION, &pszCaption))
        {
            pLabel->pszCaption = Unit_AnsiToWide(pszCaption, NULL);
            free(pszCaption);
        }

        INT nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_ALIGN, &nValue))
            pLabel->nAlign = nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_VALIGN, &nValue))
            pLabel->nVAlign = nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_BORDERSTYLE, &nValue))
            pLabel->nBorderStyle = nValue;

        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_AUTOSIZE, &bValue))
            pLabel->bAutoSize = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_WORDWRAP, &bValue))
            pLabel->bWordWrap = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_TRANSPARENT, &bValue))
            pLabel->bTransparent = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_VISIBLE, &bValue))
        {
            if (bValue) pLabel->base.dwState |= UNIT_STATE_VISIBLE;
        }
        else
        {
            pLabel->base.dwState |= UNIT_STATE_VISIBLE;
        }
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_DISABLED, &bValue))
        {
            if (bValue) pLabel->base.dwState |= UNIT_STATE_DISABLED;
        }

        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_BACKCOLOR, &clr))
            pLabel->clrBack = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_FORECOLOR, &clr))
            pLabel->clrFore = clr;

        LOGFONTW lf;
        if (Unit_ReadPropFont(pParams->pAllData, pParams->nAllDataSize, PROP_LABEL_FONT, &lf))
            pLabel->hFont = Unit_CreateFont(&lf);
    }
    else
    {
        pLabel->pszCaption = _wcsdup(L"标签");
        pLabel->base.dwState |= UNIT_STATE_VISIBLE;
    }

    // 构建 Windows 样式
    // SS_LEFT 默认支持自动换行，SS_LEFTNOWORDWRAP 禁止换行
    DWORD dwStyle = WS_CHILD | SS_NOTIFY;
    if (pLabel->bWordWrap)
        dwStyle |= SS_LEFT;
    else
        dwStyle |= SS_LEFTNOWORDWRAP;
    if (pLabel->base.dwState & UNIT_STATE_VISIBLE) dwStyle |= WS_VISIBLE;
    if (pLabel->base.dwState & UNIT_STATE_DISABLED) dwStyle |= WS_DISABLED;

    switch (pLabel->nBorderStyle)
    {
    case LABEL_BORDER_SINGLE:  dwStyle |= SS_SUNKEN;  break;
    case LABEL_BORDER_FIXED3D: dwStyle |= SS_ETCHEDFRAME; break;
    default: break;
    }

    int cx = pParams->cx > 0 ? pParams->cx : 60;
    int cy = pParams->cy > 0 ? pParams->cy : 18;

    HWND hWnd = CreateWindowExW(
        0, L"STATIC",
        pLabel->pszCaption ? pLabel->pszCaption : L"",
        dwStyle,
        pParams->x, pParams->y, cx, cy,
        pParams->hParentWnd,
        (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL), NULL);

    if (!hWnd)
    {
        Unit_Free((PWIN_UNIT_BASE)pLabel);
        return NULL;
    }

    pLabel->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)pLabel, hWnd);

    pLabel->base.pfnOldProc = (WNDPROC)SetWindowLongPtrW(
        hWnd, GWLP_WNDPROC, (LONG_PTR)Label_SubclassProc);
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)pLabel);

    HFONT hFont = pLabel->hFont ? pLabel->hFont : (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);

    return (HUNIT)pLabel;
}

// ========== 销毁标签 ==========
BOOL WINAPI Label_Destroy(HUNIT hUnit)
{
    PLABEL_DATA pLabel = (PLABEL_DATA)hUnit;
    if (!pLabel) return FALSE;

    if (pLabel->base.hWnd && pLabel->base.pfnOldProc)
        SetWindowLongPtrW(pLabel->base.hWnd, GWLP_WNDPROC, (LONG_PTR)pLabel->base.pfnOldProc);

    if (pLabel->base.hWnd && IsWindow(pLabel->base.hWnd))
        DestroyWindow(pLabel->base.hWnd);

    if (pLabel->pszCaption) free(pLabel->pszCaption);
    if (pLabel->hFont)      DeleteObject(pLabel->hFont);

    Unit_Free((PWIN_UNIT_BASE)pLabel);
    return TRUE;
}

// ========== 子类化窗口过程 ==========
static LRESULT CALLBACK Label_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PLABEL_DATA pLabel = (PLABEL_DATA)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (!pLabel || !pLabel->base.pfnOldProc)
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    switch (uMsg)
    {
    case WM_LBUTTONUP:
        Unit_FireEvent(&pLabel->base, EVENT_LABEL_CLICK, 0);
        break;
    case WM_LBUTTONDBLCLK:
        Unit_FireEvent(&pLabel->base, EVENT_LABEL_DBLCLICK, 0);
        break;
    case WM_MOUSEMOVE:
        Unit_FireEvent(&pLabel->base, EVENT_MOUSEMOVE, 3,
            (INT)LOWORD(lParam), (INT)HIWORD(lParam), (INT)wParam);
        break;
    case WM_CTLCOLORSTATIC:
        // 透明或自定义颜色由父窗口处理，此处返回空刷子
        break;
    case WM_DESTROY:
        Unit_FireEvent(&pLabel->base, EVENT_DESTROY, 0);
        break;
    }

    return CallWindowProcW(pLabel->base.pfnOldProc, hWnd, uMsg, wParam, lParam);
}

// ========== 标签命令 ==========
void WINAPI Label_SetCaption(PLABEL_DATA pLabel, LPCWSTR pszCaption)
{
    if (!pLabel) return;
    if (pLabel->pszCaption) free(pLabel->pszCaption);
    pLabel->pszCaption = pszCaption ? _wcsdup(pszCaption) : NULL;
    if (pLabel->base.hWnd)
        SetWindowTextW(pLabel->base.hWnd, pszCaption ? pszCaption : L"");
}

LPCWSTR WINAPI Label_GetCaption(PLABEL_DATA pLabel)
{
    return pLabel ? pLabel->pszCaption : NULL;
}

// ========== 属性操作 ==========
BOOL WINAPI Label_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PLABEL_DATA pLabel = (PLABEL_DATA)hUnit;
    if (!pLabel || !pValue) return FALSE;

    RECT rc = {0};
    if (pLabel->base.hWnd)
    {
        GetWindowRect(pLabel->base.hWnd, &rc);
        MapWindowPoints(HWND_DESKTOP, pLabel->base.hParent, (LPPOINT)&rc, 2);
    }

    switch (nIndex)
    {
    case PROP_LABEL_LEFT:        pValue->m_int  = rc.left;  return TRUE;
    case PROP_LABEL_TOP:         pValue->m_int  = rc.top;   return TRUE;
    case PROP_LABEL_WIDTH:       pValue->m_int  = rc.right  - rc.left; return TRUE;
    case PROP_LABEL_HEIGHT:      pValue->m_int  = rc.bottom - rc.top;  return TRUE;
    case PROP_LABEL_VISIBLE:     pValue->m_bool = (pLabel->base.dwState & UNIT_STATE_VISIBLE) != 0; return TRUE;
    case PROP_LABEL_DISABLED:    pValue->m_bool = (pLabel->base.dwState & UNIT_STATE_DISABLED) != 0; return TRUE;
    case PROP_LABEL_CAPTION:     pValue->m_szText = pLabel->pszCaption ? Unit_WideToAnsi(pLabel->pszCaption, NULL) : NULL; return TRUE;
    case PROP_LABEL_ALIGN:       pValue->m_int  = pLabel->nAlign;       return TRUE;
    case PROP_LABEL_VALIGN:      pValue->m_int  = pLabel->nVAlign;      return TRUE;
    case PROP_LABEL_BORDERSTYLE: pValue->m_int  = pLabel->nBorderStyle; return TRUE;
    case PROP_LABEL_AUTOSIZE:    pValue->m_bool = pLabel->bAutoSize;    return TRUE;
    case PROP_LABEL_WORDWRAP:    pValue->m_bool = pLabel->bWordWrap;    return TRUE;
    case PROP_LABEL_TRANSPARENT: pValue->m_bool = pLabel->bTransparent; return TRUE;
    case PROP_LABEL_BACKCOLOR:   pValue->m_clr  = pLabel->clrBack;     return TRUE;
    case PROP_LABEL_FORECOLOR:   pValue->m_clr  = pLabel->clrFore;     return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI Label_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PLABEL_DATA pLabel = (PLABEL_DATA)hUnit;
    if (!pLabel || !pValue) return FALSE;

    switch (nIndex)
    {
    case PROP_LABEL_LEFT: case PROP_LABEL_TOP: case PROP_LABEL_WIDTH: case PROP_LABEL_HEIGHT:
        if (pLabel->base.hWnd)
        {
            RECT rc;
            GetWindowRect(pLabel->base.hWnd, &rc);
            MapWindowPoints(HWND_DESKTOP, pLabel->base.hParent, (LPPOINT)&rc, 2);
            if (nIndex == PROP_LABEL_LEFT)         rc.left   = pValue->m_int;
            else if (nIndex == PROP_LABEL_TOP)     rc.top    = pValue->m_int;
            else if (nIndex == PROP_LABEL_WIDTH)   rc.right  = rc.left + pValue->m_int;
            else                                   rc.bottom = rc.top  + pValue->m_int;
            SetWindowPos(pLabel->base.hWnd, NULL, rc.left, rc.top,
                rc.right-rc.left, rc.bottom-rc.top, SWP_NOZORDER);
        }
        return FALSE;

    case PROP_LABEL_VISIBLE:
        if (pValue->m_bool) pLabel->base.dwState |= UNIT_STATE_VISIBLE;
        else                pLabel->base.dwState &= ~UNIT_STATE_VISIBLE;
        if (pLabel->base.hWnd)
            ShowWindow(pLabel->base.hWnd, pValue->m_bool ? SW_SHOW : SW_HIDE);
        return FALSE;

    case PROP_LABEL_DISABLED:
        if (pValue->m_bool) pLabel->base.dwState |= UNIT_STATE_DISABLED;
        else                pLabel->base.dwState &= ~UNIT_STATE_DISABLED;
        if (pLabel->base.hWnd)
            EnableWindow(pLabel->base.hWnd, !pValue->m_bool);
        return FALSE;

    case PROP_LABEL_CAPTION:
        {
            LPWSTR pszWide = pValue->m_szText ? Unit_AnsiToWide(pValue->m_szText, NULL) : NULL;
            Label_SetCaption(pLabel, pszWide);
            if (pszWide) Unit_FreeString(pszWide);
        }
        return FALSE;

    case PROP_LABEL_BACKCOLOR:
        pLabel->clrBack = pValue->m_clr;
        if (pLabel->base.hWnd) InvalidateRect(pLabel->base.hWnd, NULL, TRUE);
        return FALSE;

    case PROP_LABEL_FORECOLOR:
        pLabel->clrFore = pValue->m_clr;
        if (pLabel->base.hWnd) InvalidateRect(pLabel->base.hWnd, NULL, TRUE);
        return FALSE;

    case PROP_LABEL_WORDWRAP:
    case PROP_LABEL_BORDERSTYLE:
    case PROP_LABEL_AUTOSIZE:
        // 需要重建控件才能生效
        if (ppszTip) *ppszTip = "修改此属性需要重新创建组件";
        return TRUE;

    default:
        return FALSE;
    }
}

BOOL WINAPI Label_PropertyUpdateUI(HUNIT hUnit, INT nIndex)
{
    return TRUE;
}

// ========== 接口获取 ==========
static HUNIT WINAPI Label_Interface_Create(
    LPBYTE pAllData, INT nAllDataSize, DWORD dwStyle, HWND hParentWnd,
    UINT uID, HMENU hMenu, INT x, INT y, INT cx, INT cy,
    DWORD dwFormID, DWORD dwUnitID, HWND hDesignWnd, BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS p = {0};
    p.pAllData = pAllData; p.nAllDataSize = nAllDataSize;
    p.dwStyle = dwStyle;   p.hParentWnd = hParentWnd;
    p.uID = uID;           p.hMenu = hMenu;
    p.x = x; p.y = y; p.cx = cx; p.cy = cy;
    p.dwFormID = dwFormID; p.dwUnitID = dwUnitID;
    p.hDesignWnd = hDesignWnd; p.blInDesignMode = blInDesignMode;
    return Label_Create(&p);
}

PFN_INTERFACE WINAPI Label_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:            return (PFN_INTERFACE)Label_Interface_Create;
    case ITF_GET_PROPERTY_DATA:      return (PFN_INTERFACE)Label_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED:return (PFN_INTERFACE)Label_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:     return (PFN_INTERFACE)Label_PropertyUpdateUI;
    default: return NULL;
    }
}
