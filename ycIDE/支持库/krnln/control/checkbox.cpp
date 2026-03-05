/**
 * @file checkbox.cpp
 * @brief 选择框组件实现
 * @date 2026-03-05
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../include_krnln_header.h"
#include "checkbox.h"
#include <stdlib.h>
#include <string.h>

static LRESULT CALLBACK CheckBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ========== 创建 ==========
HUNIT WINAPI CheckBox_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    PCHECKBOX_DATA p = (PCHECKBOX_DATA)Unit_Alloc(sizeof(CHECKBOX_DATA) - sizeof(WIN_UNIT_BASE));
    if (!p) return NULL;

    p->base.dwFormID = pParams->dwFormID;
    p->base.dwUnitID = pParams->dwUnitID;
    p->base.hParent  = pParams->hParentWnd;
    p->clrBack = GetSysColor(COLOR_BTNFACE);
    p->clrFore = GetSysColor(COLOR_BTNTEXT);
    p->nChecked = CHECKBOX_UNCHECKED;
    p->bTriState = FALSE;

    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        LPSTR pszCaption = NULL;
        if (Unit_ReadPropText(pParams->pAllData, pParams->nAllDataSize, PROP_CHECKBOX_CAPTION, &pszCaption))
        {
            p->pszCaption = Unit_AnsiToWide(pszCaption, NULL);
            free(pszCaption);
        }
        INT nValue;
        if (Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_CHECKBOX_CHECKED, &nValue))
            p->nChecked = nValue;
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_CHECKBOX_TRISTATE, &bValue))
            p->bTriState = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_CHECKBOX_VISIBLE, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_VISIBLE; }
        else p->base.dwState |= UNIT_STATE_VISIBLE;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_CHECKBOX_DISABLED, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_DISABLED; }
        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_CHECKBOX_BACKCOLOR, &clr)) p->clrBack = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_CHECKBOX_FORECOLOR, &clr)) p->clrFore = clr;
        LOGFONTW lf;
        if (Unit_ReadPropFont(pParams->pAllData, pParams->nAllDataSize, PROP_CHECKBOX_FONT, &lf))
            p->hFont = Unit_CreateFont(&lf);
    }
    else
    {
        p->pszCaption = _wcsdup(L"选择框");
        p->base.dwState |= UNIT_STATE_VISIBLE;
    }

    DWORD dwStyle = WS_CHILD | WS_TABSTOP | BS_NOTIFY;
    dwStyle |= p->bTriState ? BS_AUTO3STATE : BS_AUTOCHECKBOX;
    if (p->base.dwState & UNIT_STATE_VISIBLE) dwStyle |= WS_VISIBLE;
    if (p->base.dwState & UNIT_STATE_DISABLED) dwStyle |= WS_DISABLED;

    HWND hWnd = CreateWindowExW(0, L"BUTTON",
        p->pszCaption ? p->pszCaption : L"",
        dwStyle,
        pParams->x, pParams->y,
        pParams->cx > 0 ? pParams->cx : 80,
        pParams->cy > 0 ? pParams->cy : 20,
        pParams->hParentWnd,
        (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL), NULL);

    if (!hWnd) { Unit_Free((PWIN_UNIT_BASE)p); return NULL; }

    p->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)p, hWnd);
    p->base.pfnOldProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)CheckBox_SubclassProc);
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)p);

    SendMessage(hWnd, BM_SETCHECK, p->nChecked, 0);
    SendMessage(hWnd, WM_SETFONT,
        (WPARAM)(p->hFont ? p->hFont : GetStockObject(DEFAULT_GUI_FONT)), TRUE);

    return (HUNIT)p;
}

// ========== 销毁 ==========
BOOL WINAPI CheckBox_Destroy(HUNIT hUnit)
{
    PCHECKBOX_DATA p = (PCHECKBOX_DATA)hUnit;
    if (!p) return FALSE;
    if (p->base.hWnd && p->base.pfnOldProc)
        SetWindowLongPtrW(p->base.hWnd, GWLP_WNDPROC, (LONG_PTR)p->base.pfnOldProc);
    if (p->base.hWnd && IsWindow(p->base.hWnd))
        DestroyWindow(p->base.hWnd);
    if (p->pszCaption) free(p->pszCaption);
    if (p->hFont)      DeleteObject(p->hFont);
    Unit_Free((PWIN_UNIT_BASE)p);
    return TRUE;
}

// ========== 子类化过程 ==========
static LRESULT CALLBACK CheckBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PCHECKBOX_DATA p = (PCHECKBOX_DATA)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (!p || !p->base.pfnOldProc)
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);

    switch (uMsg)
    {
    case WM_LBUTTONUP:
        // 同步选中状态
        p->nChecked = (INT)SendMessage(hWnd, BM_GETCHECK, 0, 0);
        Unit_FireEvent(&p->base, EVENT_CHECKBOX_CLICK, 1, p->nChecked);
        break;
    case WM_KEYUP:
        if (wParam == VK_SPACE)
        {
            p->nChecked = (INT)SendMessage(hWnd, BM_GETCHECK, 0, 0);
            Unit_FireEvent(&p->base, EVENT_CHECKBOX_CLICK, 1, p->nChecked);
        }
        break;
    case WM_SETFOCUS:
        p->base.dwState |= UNIT_STATE_FOCUSED;
        Unit_FireEvent(&p->base, EVENT_GOTFOCUS, 0);
        break;
    case WM_KILLFOCUS:
        p->base.dwState &= ~UNIT_STATE_FOCUSED;
        Unit_FireEvent(&p->base, EVENT_LOSTFOCUS, 0);
        break;
    case WM_DESTROY:
        Unit_FireEvent(&p->base, EVENT_DESTROY, 0);
        break;
    }
    return CallWindowProcW(p->base.pfnOldProc, hWnd, uMsg, wParam, lParam);
}

// ========== 属性操作 ==========
BOOL WINAPI CheckBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PCHECKBOX_DATA p = (PCHECKBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    RECT rc = {0};
    if (p->base.hWnd) { GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2); }
    switch (nIndex)
    {
    case PROP_CHECKBOX_LEFT:     pValue->m_int  = rc.left;  return TRUE;
    case PROP_CHECKBOX_TOP:      pValue->m_int  = rc.top;   return TRUE;
    case PROP_CHECKBOX_WIDTH:    pValue->m_int  = rc.right  - rc.left; return TRUE;
    case PROP_CHECKBOX_HEIGHT:   pValue->m_int  = rc.bottom - rc.top;  return TRUE;
    case PROP_CHECKBOX_VISIBLE:  pValue->m_bool = (p->base.dwState & UNIT_STATE_VISIBLE)  != 0; return TRUE;
    case PROP_CHECKBOX_DISABLED: pValue->m_bool = (p->base.dwState & UNIT_STATE_DISABLED) != 0; return TRUE;
    case PROP_CHECKBOX_CAPTION:  pValue->m_szText = p->pszCaption ? Unit_WideToAnsi(p->pszCaption, NULL) : NULL; return TRUE;
    case PROP_CHECKBOX_CHECKED:  pValue->m_int  = p->nChecked;  return TRUE;
    case PROP_CHECKBOX_BACKCOLOR:pValue->m_clr  = p->clrBack;  return TRUE;
    case PROP_CHECKBOX_FORECOLOR:pValue->m_clr  = p->clrFore;  return TRUE;
    case PROP_CHECKBOX_TRISTATE: pValue->m_bool = p->bTriState; return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI CheckBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PCHECKBOX_DATA p = (PCHECKBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_CHECKBOX_LEFT: case PROP_CHECKBOX_TOP: case PROP_CHECKBOX_WIDTH: case PROP_CHECKBOX_HEIGHT:
        if (p->base.hWnd)
        {
            RECT rc; GetWindowRect(p->base.hWnd, &rc);
            MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2);
            if (nIndex == PROP_CHECKBOX_LEFT)        rc.left   = pValue->m_int;
            else if (nIndex == PROP_CHECKBOX_TOP)    rc.top    = pValue->m_int;
            else if (nIndex == PROP_CHECKBOX_WIDTH)  rc.right  = rc.left + pValue->m_int;
            else                                     rc.bottom = rc.top  + pValue->m_int;
            SetWindowPos(p->base.hWnd, NULL, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, SWP_NOZORDER);
        }
        return FALSE;
    case PROP_CHECKBOX_VISIBLE:
        if (pValue->m_bool) p->base.dwState |= UNIT_STATE_VISIBLE;
        else p->base.dwState &= ~UNIT_STATE_VISIBLE;
        if (p->base.hWnd) ShowWindow(p->base.hWnd, pValue->m_bool ? SW_SHOW : SW_HIDE);
        return FALSE;
    case PROP_CHECKBOX_DISABLED:
        if (pValue->m_bool) p->base.dwState |= UNIT_STATE_DISABLED;
        else p->base.dwState &= ~UNIT_STATE_DISABLED;
        if (p->base.hWnd) EnableWindow(p->base.hWnd, !pValue->m_bool);
        return FALSE;
    case PROP_CHECKBOX_CAPTION:
        if (p->pszCaption) free(p->pszCaption);
        p->pszCaption = pValue->m_szText ? Unit_AnsiToWide(pValue->m_szText, NULL) : NULL;
        if (p->base.hWnd) SetWindowTextW(p->base.hWnd, p->pszCaption ? p->pszCaption : L"");
        return FALSE;
    case PROP_CHECKBOX_CHECKED:
        p->nChecked = pValue->m_int;
        if (p->base.hWnd) SendMessage(p->base.hWnd, BM_SETCHECK, p->nChecked, 0);
        return FALSE;
    case PROP_CHECKBOX_BACKCOLOR:
        p->clrBack = pValue->m_clr;
        if (p->base.hWnd) InvalidateRect(p->base.hWnd, NULL, TRUE);
        return FALSE;
    case PROP_CHECKBOX_FORECOLOR:
        p->clrFore = pValue->m_clr;
        if (p->base.hWnd) InvalidateRect(p->base.hWnd, NULL, TRUE);
        return FALSE;
    case PROP_CHECKBOX_TRISTATE:
        if (ppszTip) *ppszTip = "修改此属性需要重新创建组件";
        return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI CheckBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex) { return TRUE; }

static HUNIT WINAPI CheckBox_Interface_Create(
    LPBYTE pAllData, INT nAllDataSize, DWORD dwStyle, HWND hParentWnd,
    UINT uID, HMENU hMenu, INT x, INT y, INT cx, INT cy,
    DWORD dwFormID, DWORD dwUnitID, HWND hDesignWnd, BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q = {0};
    q.pAllData=pAllData; q.nAllDataSize=nAllDataSize; q.dwStyle=dwStyle; q.hParentWnd=hParentWnd;
    q.uID=uID; q.hMenu=hMenu; q.x=x; q.y=y; q.cx=cx; q.cy=cy;
    q.dwFormID=dwFormID; q.dwUnitID=dwUnitID; q.hDesignWnd=hDesignWnd; q.blInDesignMode=blInDesignMode;
    return CheckBox_Create(&q);
}

PFN_INTERFACE WINAPI CheckBox_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)CheckBox_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)CheckBox_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)CheckBox_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)CheckBox_PropertyUpdateUI;
    default: return NULL;
    }
}
