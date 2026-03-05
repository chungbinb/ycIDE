/**
 * @file radiobox.cpp
 * @brief 单选框组件实现
 * @date 2026-03-05
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../include_krnln_header.h"
#include "radiobox.h"
#include <stdlib.h>
#include <string.h>

static LRESULT CALLBACK RadioBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HUNIT WINAPI RadioBox_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    PRADIOBOX_DATA p = (PRADIOBOX_DATA)Unit_Alloc(sizeof(RADIOBOX_DATA) - sizeof(WIN_UNIT_BASE));
    if (!p) return NULL;

    p->base.dwFormID = pParams->dwFormID;
    p->base.dwUnitID = pParams->dwUnitID;
    p->base.hParent  = pParams->hParentWnd;
    p->clrBack = GetSysColor(COLOR_BTNFACE);
    p->clrFore = GetSysColor(COLOR_BTNTEXT);
    p->bChecked = FALSE;

    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        LPSTR pszCaption = NULL;
        if (Unit_ReadPropText(pParams->pAllData, pParams->nAllDataSize, PROP_RADIOBOX_CAPTION, &pszCaption))
        { p->pszCaption = Unit_AnsiToWide(pszCaption, NULL); free(pszCaption); }
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_RADIOBOX_CHECKED, &bValue))
            p->bChecked = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_RADIOBOX_VISIBLE, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_VISIBLE; }
        else p->base.dwState |= UNIT_STATE_VISIBLE;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_RADIOBOX_DISABLED, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_DISABLED; }
        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_RADIOBOX_BACKCOLOR, &clr)) p->clrBack = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_RADIOBOX_FORECOLOR, &clr)) p->clrFore = clr;
        LOGFONTW lf;
        if (Unit_ReadPropFont(pParams->pAllData, pParams->nAllDataSize, PROP_RADIOBOX_FONT, &lf))
            p->hFont = Unit_CreateFont(&lf);
    }
    else
    {
        p->pszCaption = _wcsdup(L"单选框");
        p->base.dwState |= UNIT_STATE_VISIBLE;
    }

    DWORD dwStyle = WS_CHILD | WS_TABSTOP | BS_AUTORADIOBUTTON | BS_NOTIFY;
    if (p->base.dwState & UNIT_STATE_VISIBLE)  dwStyle |= WS_VISIBLE;
    if (p->base.dwState & UNIT_STATE_DISABLED) dwStyle |= WS_DISABLED;

    HWND hWnd = CreateWindowExW(0, L"BUTTON",
        p->pszCaption ? p->pszCaption : L"",
        dwStyle, pParams->x, pParams->y,
        pParams->cx > 0 ? pParams->cx : 80,
        pParams->cy > 0 ? pParams->cy : 20,
        pParams->hParentWnd, (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL), NULL);

    if (!hWnd) { Unit_Free((PWIN_UNIT_BASE)p); return NULL; }

    p->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)p, hWnd);
    p->base.pfnOldProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)RadioBox_SubclassProc);
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)p);

    SendMessage(hWnd, BM_SETCHECK, p->bChecked ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessage(hWnd, WM_SETFONT,
        (WPARAM)(p->hFont ? p->hFont : GetStockObject(DEFAULT_GUI_FONT)), TRUE);

    return (HUNIT)p;
}

BOOL WINAPI RadioBox_Destroy(HUNIT hUnit)
{
    PRADIOBOX_DATA p = (PRADIOBOX_DATA)hUnit;
    if (!p) return FALSE;
    if (p->base.hWnd && p->base.pfnOldProc)
        SetWindowLongPtrW(p->base.hWnd, GWLP_WNDPROC, (LONG_PTR)p->base.pfnOldProc);
    if (p->base.hWnd && IsWindow(p->base.hWnd)) DestroyWindow(p->base.hWnd);
    if (p->pszCaption) free(p->pszCaption);
    if (p->hFont)      DeleteObject(p->hFont);
    Unit_Free((PWIN_UNIT_BASE)p);
    return TRUE;
}

static LRESULT CALLBACK RadioBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PRADIOBOX_DATA p = (PRADIOBOX_DATA)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (!p || !p->base.pfnOldProc) return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    switch (uMsg)
    {
    case WM_LBUTTONUP:
        p->bChecked = (SendMessage(hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED);
        Unit_FireEvent(&p->base, EVENT_RADIOBOX_CLICK, 0);
        break;
    case WM_KEYUP:
        if (wParam == VK_SPACE)
        { p->bChecked = (SendMessage(hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED); Unit_FireEvent(&p->base, EVENT_RADIOBOX_CLICK, 0); }
        break;
    case WM_SETFOCUS:  p->base.dwState |= UNIT_STATE_FOCUSED;  Unit_FireEvent(&p->base, EVENT_GOTFOCUS, 0);  break;
    case WM_KILLFOCUS: p->base.dwState &= ~UNIT_STATE_FOCUSED; Unit_FireEvent(&p->base, EVENT_LOSTFOCUS, 0); break;
    case WM_DESTROY:   Unit_FireEvent(&p->base, EVENT_DESTROY, 0); break;
    }
    return CallWindowProcW(p->base.pfnOldProc, hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI RadioBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PRADIOBOX_DATA p = (PRADIOBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    RECT rc = {0};
    if (p->base.hWnd) { GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2); }
    switch (nIndex)
    {
    case PROP_RADIOBOX_LEFT:     pValue->m_int  = rc.left;  return TRUE;
    case PROP_RADIOBOX_TOP:      pValue->m_int  = rc.top;   return TRUE;
    case PROP_RADIOBOX_WIDTH:    pValue->m_int  = rc.right  - rc.left; return TRUE;
    case PROP_RADIOBOX_HEIGHT:   pValue->m_int  = rc.bottom - rc.top;  return TRUE;
    case PROP_RADIOBOX_VISIBLE:  pValue->m_bool = (p->base.dwState & UNIT_STATE_VISIBLE)  != 0; return TRUE;
    case PROP_RADIOBOX_DISABLED: pValue->m_bool = (p->base.dwState & UNIT_STATE_DISABLED) != 0; return TRUE;
    case PROP_RADIOBOX_CAPTION:  pValue->m_szText = p->pszCaption ? Unit_WideToAnsi(p->pszCaption, NULL) : NULL; return TRUE;
    case PROP_RADIOBOX_CHECKED:  pValue->m_bool = p->bChecked;  return TRUE;
    case PROP_RADIOBOX_BACKCOLOR:pValue->m_clr  = p->clrBack;   return TRUE;
    case PROP_RADIOBOX_FORECOLOR:pValue->m_clr  = p->clrFore;   return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI RadioBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PRADIOBOX_DATA p = (PRADIOBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_RADIOBOX_LEFT: case PROP_RADIOBOX_TOP: case PROP_RADIOBOX_WIDTH: case PROP_RADIOBOX_HEIGHT:
        if (p->base.hWnd) {
            RECT rc; GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2);
            if (nIndex==PROP_RADIOBOX_LEFT) rc.left=pValue->m_int;
            else if (nIndex==PROP_RADIOBOX_TOP) rc.top=pValue->m_int;
            else if (nIndex==PROP_RADIOBOX_WIDTH) rc.right=rc.left+pValue->m_int;
            else rc.bottom=rc.top+pValue->m_int;
            SetWindowPos(p->base.hWnd,NULL,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOZORDER);
        } return FALSE;
    case PROP_RADIOBOX_VISIBLE:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_VISIBLE; else p->base.dwState&=~UNIT_STATE_VISIBLE;
        if(p->base.hWnd) ShowWindow(p->base.hWnd, pValue->m_bool?SW_SHOW:SW_HIDE); return FALSE;
    case PROP_RADIOBOX_DISABLED:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_DISABLED; else p->base.dwState&=~UNIT_STATE_DISABLED;
        if(p->base.hWnd) EnableWindow(p->base.hWnd, !pValue->m_bool); return FALSE;
    case PROP_RADIOBOX_CAPTION:
        if(p->pszCaption) free(p->pszCaption);
        p->pszCaption = pValue->m_szText ? Unit_AnsiToWide(pValue->m_szText,NULL) : NULL;
        if(p->base.hWnd) SetWindowTextW(p->base.hWnd, p->pszCaption?p->pszCaption:L""); return FALSE;
    case PROP_RADIOBOX_CHECKED:
        p->bChecked = pValue->m_bool;
        if(p->base.hWnd) SendMessage(p->base.hWnd, BM_SETCHECK, p->bChecked?BST_CHECKED:BST_UNCHECKED, 0); return FALSE;
    case PROP_RADIOBOX_BACKCOLOR: p->clrBack=pValue->m_clr; if(p->base.hWnd) InvalidateRect(p->base.hWnd,NULL,TRUE); return FALSE;
    case PROP_RADIOBOX_FORECOLOR: p->clrFore=pValue->m_clr; if(p->base.hWnd) InvalidateRect(p->base.hWnd,NULL,TRUE); return FALSE;
    default: return FALSE;
    }
}

BOOL WINAPI RadioBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex) { return TRUE; }

static HUNIT WINAPI RadioBox_Interface_Create(
    LPBYTE pAllData,INT nAllDataSize,DWORD dwStyle,HWND hParentWnd,
    UINT uID,HMENU hMenu,INT x,INT y,INT cx,INT cy,
    DWORD dwFormID,DWORD dwUnitID,HWND hDesignWnd,BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q={0};
    q.pAllData=pAllData;q.nAllDataSize=nAllDataSize;q.dwStyle=dwStyle;q.hParentWnd=hParentWnd;
    q.uID=uID;q.hMenu=hMenu;q.x=x;q.y=y;q.cx=cx;q.cy=cy;
    q.dwFormID=dwFormID;q.dwUnitID=dwUnitID;q.hDesignWnd=hDesignWnd;q.blInDesignMode=blInDesignMode;
    return RadioBox_Create(&q);
}

PFN_INTERFACE WINAPI RadioBox_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)RadioBox_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)RadioBox_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)RadioBox_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)RadioBox_PropertyUpdateUI;
    default: return NULL;
    }
}
