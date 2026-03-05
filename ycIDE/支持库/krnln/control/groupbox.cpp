/**
 * @file groupbox.cpp
 * @brief 分组框组件实现
 * @date 2026-03-05
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../include_krnln_header.h"
#include "groupbox.h"
#include <stdlib.h>
#include <string.h>

HUNIT WINAPI GroupBox_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    PGROUPBOX_DATA p = (PGROUPBOX_DATA)Unit_Alloc(sizeof(GROUPBOX_DATA) - sizeof(WIN_UNIT_BASE));
    if (!p) return NULL;

    p->base.dwFormID = pParams->dwFormID;
    p->base.dwUnitID = pParams->dwUnitID;
    p->base.hParent  = pParams->hParentWnd;
    p->clrBack = GetSysColor(COLOR_BTNFACE);
    p->clrFore = GetSysColor(COLOR_BTNTEXT);

    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        LPSTR pszCaption = NULL;
        if (Unit_ReadPropText(pParams->pAllData, pParams->nAllDataSize, PROP_GROUPBOX_CAPTION, &pszCaption))
        { p->pszCaption = Unit_AnsiToWide(pszCaption, NULL); free(pszCaption); }
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_GROUPBOX_VISIBLE, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_VISIBLE; }
        else p->base.dwState |= UNIT_STATE_VISIBLE;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_GROUPBOX_DISABLED, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_DISABLED; }
        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_GROUPBOX_BACKCOLOR, &clr)) p->clrBack = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_GROUPBOX_FORECOLOR, &clr)) p->clrFore = clr;
        LOGFONTW lf;
        if (Unit_ReadPropFont(pParams->pAllData, pParams->nAllDataSize, PROP_GROUPBOX_FONT, &lf))
            p->hFont = Unit_CreateFont(&lf);
    }
    else
    {
        p->pszCaption = _wcsdup(L"分组框");
        p->base.dwState |= UNIT_STATE_VISIBLE;
    }

    DWORD dwStyle = WS_CHILD | BS_GROUPBOX;
    if (p->base.dwState & UNIT_STATE_VISIBLE)  dwStyle |= WS_VISIBLE;
    if (p->base.dwState & UNIT_STATE_DISABLED) dwStyle |= WS_DISABLED;

    HWND hWnd = CreateWindowExW(0, L"BUTTON",
        p->pszCaption ? p->pszCaption : L"",
        dwStyle, pParams->x, pParams->y,
        pParams->cx > 0 ? pParams->cx : 120,
        pParams->cy > 0 ? pParams->cy : 80,
        pParams->hParentWnd, (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL), NULL);

    if (!hWnd) { Unit_Free((PWIN_UNIT_BASE)p); return NULL; }

    p->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)p, hWnd);
    /* 分组框无需事件，不需要子类化 */

    SendMessage(hWnd, WM_SETFONT,
        (WPARAM)(p->hFont ? p->hFont : GetStockObject(DEFAULT_GUI_FONT)), TRUE);

    return (HUNIT)p;
}

BOOL WINAPI GroupBox_Destroy(HUNIT hUnit)
{
    PGROUPBOX_DATA p = (PGROUPBOX_DATA)hUnit;
    if (!p) return FALSE;
    if (p->base.hWnd && IsWindow(p->base.hWnd)) DestroyWindow(p->base.hWnd);
    if (p->pszCaption) free(p->pszCaption);
    if (p->hFont)      DeleteObject(p->hFont);
    Unit_Free((PWIN_UNIT_BASE)p);
    return TRUE;
}

BOOL WINAPI GroupBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PGROUPBOX_DATA p = (PGROUPBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    RECT rc = {0};
    if (p->base.hWnd) { GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2); }
    switch (nIndex)
    {
    case PROP_GROUPBOX_LEFT:     pValue->m_int  = rc.left;  return TRUE;
    case PROP_GROUPBOX_TOP:      pValue->m_int  = rc.top;   return TRUE;
    case PROP_GROUPBOX_WIDTH:    pValue->m_int  = rc.right  - rc.left; return TRUE;
    case PROP_GROUPBOX_HEIGHT:   pValue->m_int  = rc.bottom - rc.top;  return TRUE;
    case PROP_GROUPBOX_VISIBLE:  pValue->m_bool = (p->base.dwState & UNIT_STATE_VISIBLE)  != 0; return TRUE;
    case PROP_GROUPBOX_DISABLED: pValue->m_bool = (p->base.dwState & UNIT_STATE_DISABLED) != 0; return TRUE;
    case PROP_GROUPBOX_CAPTION:  pValue->m_szText = p->pszCaption ? Unit_WideToAnsi(p->pszCaption, NULL) : NULL; return TRUE;
    case PROP_GROUPBOX_BACKCOLOR:pValue->m_clr  = p->clrBack; return TRUE;
    case PROP_GROUPBOX_FORECOLOR:pValue->m_clr  = p->clrFore; return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI GroupBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PGROUPBOX_DATA p = (PGROUPBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_GROUPBOX_LEFT: case PROP_GROUPBOX_TOP: case PROP_GROUPBOX_WIDTH: case PROP_GROUPBOX_HEIGHT:
        if (p->base.hWnd) {
            RECT rc; GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2);
            if (nIndex==PROP_GROUPBOX_LEFT) rc.left=pValue->m_int;
            else if (nIndex==PROP_GROUPBOX_TOP) rc.top=pValue->m_int;
            else if (nIndex==PROP_GROUPBOX_WIDTH) rc.right=rc.left+pValue->m_int;
            else rc.bottom=rc.top+pValue->m_int;
            SetWindowPos(p->base.hWnd,NULL,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOZORDER);
        } return FALSE;
    case PROP_GROUPBOX_VISIBLE:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_VISIBLE; else p->base.dwState&=~UNIT_STATE_VISIBLE;
        if(p->base.hWnd) ShowWindow(p->base.hWnd, pValue->m_bool?SW_SHOW:SW_HIDE); return FALSE;
    case PROP_GROUPBOX_DISABLED:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_DISABLED; else p->base.dwState&=~UNIT_STATE_DISABLED;
        if(p->base.hWnd) EnableWindow(p->base.hWnd, !pValue->m_bool); return FALSE;
    case PROP_GROUPBOX_CAPTION:
        if(p->pszCaption) free(p->pszCaption);
        p->pszCaption = pValue->m_szText ? Unit_AnsiToWide(pValue->m_szText,NULL) : NULL;
        if(p->base.hWnd) SetWindowTextW(p->base.hWnd, p->pszCaption?p->pszCaption:L""); return FALSE;
    case PROP_GROUPBOX_BACKCOLOR: p->clrBack=pValue->m_clr; if(p->base.hWnd) InvalidateRect(p->base.hWnd,NULL,TRUE); return FALSE;
    case PROP_GROUPBOX_FORECOLOR: p->clrFore=pValue->m_clr; if(p->base.hWnd) InvalidateRect(p->base.hWnd,NULL,TRUE); return FALSE;
    default: return FALSE;
    }
}

BOOL WINAPI GroupBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex) { return TRUE; }

static HUNIT WINAPI GroupBox_Interface_Create(
    LPBYTE pAllData,INT nAllDataSize,DWORD dwStyle,HWND hParentWnd,
    UINT uID,HMENU hMenu,INT x,INT y,INT cx,INT cy,
    DWORD dwFormID,DWORD dwUnitID,HWND hDesignWnd,BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q={0};
    q.pAllData=pAllData;q.nAllDataSize=nAllDataSize;q.dwStyle=dwStyle;q.hParentWnd=hParentWnd;
    q.uID=uID;q.hMenu=hMenu;q.x=x;q.y=y;q.cx=cx;q.cy=cy;
    q.dwFormID=dwFormID;q.dwUnitID=dwUnitID;q.hDesignWnd=hDesignWnd;q.blInDesignMode=blInDesignMode;
    return GroupBox_Create(&q);
}

PFN_INTERFACE WINAPI GroupBox_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)GroupBox_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)GroupBox_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)GroupBox_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)GroupBox_PropertyUpdateUI;
    default: return NULL;
    }
}
