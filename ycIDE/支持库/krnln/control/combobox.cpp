/**
 * @file combobox.cpp
 * @brief 组合框/下拉框组件实现
 * @date 2026-03-05
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../include_krnln_header.h"
#include "combobox.h"
#include <stdlib.h>
#include <string.h>

static LRESULT CALLBACK ComboBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK ComboBox_ParentSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/* 父窗口挂钩 */
typedef struct tagCB_PARENT_HOOK {
    HWND   hCombo;
    UINT   uID;
    WNDPROC pfnOldParent;
    struct tagCB_PARENT_HOOK* pNext;
} CB_PARENT_HOOK;
static CB_PARENT_HOOK* g_pCBHooks = NULL;

static void CB_RegisterParentHook(HWND hParent, HWND hCombo, UINT uID)
{
    CB_PARENT_HOOK* pHook = (CB_PARENT_HOOK*)malloc(sizeof(CB_PARENT_HOOK));
    if (!pHook) return;
    pHook->hCombo = hCombo; pHook->uID = uID; pHook->pNext = g_pCBHooks;
    WNDPROC pfnCur = (WNDPROC)GetWindowLongPtrW(hParent, GWLP_WNDPROC);
    if (pfnCur != ComboBox_ParentSubclass)
        pHook->pfnOldParent = (WNDPROC)SetWindowLongPtrW(hParent, GWLP_WNDPROC, (LONG_PTR)ComboBox_ParentSubclass);
    else
        pHook->pfnOldParent = (WNDPROC)GetPropW(hParent, L"CB_OldProc");
    SetPropW(hParent, L"CB_OldProc", (HANDLE)pHook->pfnOldParent);
    g_pCBHooks = pHook;
}

static LRESULT CALLBACK ComboBox_ParentSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WNDPROC pfnOld = (WNDPROC)GetPropW(hWnd, L"CB_OldProc");
    if (uMsg == WM_COMMAND)
    {
        UINT uID = LOWORD(wParam); UINT uCode = HIWORD(wParam);
        for (CB_PARENT_HOOK* pHook = g_pCBHooks; pHook; pHook = pHook->pNext)
        {
            if (pHook->uID == uID)
            {
                PCOMBOBOX_DATA pData = (PCOMBOBOX_DATA)GetWindowLongPtrW(pHook->hCombo, GWLP_USERDATA);
                if (pData) {
                    if (uCode == CBN_SELCHANGE) {
                        pData->nSelIndex = (INT)SendMessage(pHook->hCombo, CB_GETCURSEL, 0, 0);
                        Unit_FireEvent(&pData->base, EVENT_COMBOBOX_SELCHANGE, 0);
                    } else if (uCode == CBN_EDITCHANGE) {
                        Unit_FireEvent(&pData->base, EVENT_COMBOBOX_EDITCHANGE, 0);
                    }
                }
                break;
            }
        }
    }
    return pfnOld ? CallWindowProcW(pfnOld, hWnd, uMsg, wParam, lParam) : DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

HUNIT WINAPI ComboBox_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)Unit_Alloc(sizeof(COMBOBOX_DATA) - sizeof(WIN_UNIT_BASE));
    if (!p) return NULL;

    p->base.dwFormID = pParams->dwFormID;
    p->base.dwUnitID = pParams->dwUnitID;
    p->base.hParent  = pParams->hParentWnd;
    p->clrBack = RGB(255,255,255);
    p->clrFore = GetSysColor(COLOR_WINDOWTEXT);
    p->nSelIndex = -1;
    p->nStyle = COMBOBOX_STYLE_DROPDOWNLIST;

    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_COMBOBOX_VISIBLE, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_VISIBLE; }
        else p->base.dwState |= UNIT_STATE_VISIBLE;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_COMBOBOX_DISABLED, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_DISABLED; }
        INT nStyleVal;
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_COMBOBOX_STYLE, &nStyleVal);
        p->nStyle = nStyleVal;
        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_COMBOBOX_BACKCOLOR, &clr)) p->clrBack = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_COMBOBOX_FORECOLOR, &clr)) p->clrFore = clr;
        LOGFONTW lf;
        if (Unit_ReadPropFont(pParams->pAllData, pParams->nAllDataSize, PROP_COMBOBOX_FONT, &lf))
            p->hFont = Unit_CreateFont(&lf);
    }
    else
    {
        p->base.dwState |= UNIT_STATE_VISIBLE;
    }

    DWORD dwCBStyle;
    switch (p->nStyle) {
    case COMBOBOX_STYLE_SIMPLE:       dwCBStyle = CBS_SIMPLE;       break;
    case COMBOBOX_STYLE_DROPDOWN:     dwCBStyle = CBS_DROPDOWN;     break;
    case COMBOBOX_STYLE_DROPDOWNLIST: default: dwCBStyle = CBS_DROPDOWNLIST; break;
    }

    DWORD dwStyle = WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_HASSTRINGS | dwCBStyle;
    if (p->base.dwState & UNIT_STATE_VISIBLE)  dwStyle |= WS_VISIBLE;
    if (p->base.dwState & UNIT_STATE_DISABLED) dwStyle |= WS_DISABLED;

    HWND hWnd = CreateWindowExW(0, L"COMBOBOX", L"",
        dwStyle, pParams->x, pParams->y,
        pParams->cx > 0 ? pParams->cx : 120,
        pParams->cy > 0 ? pParams->cy : 200,
        pParams->hParentWnd, (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL), NULL);

    if (!hWnd) { Unit_Free((PWIN_UNIT_BASE)p); return NULL; }

    p->base.hWnd = hWnd;
    p->uMyID = pParams->uID;
    Unit_SetHWnd((PWIN_UNIT_BASE)p, hWnd);
    p->base.pfnOldProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)ComboBox_SubclassProc);
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)p);

    CB_RegisterParentHook(pParams->hParentWnd, hWnd, pParams->uID);

    SendMessage(hWnd, WM_SETFONT,
        (WPARAM)(p->hFont ? p->hFont : GetStockObject(DEFAULT_GUI_FONT)), TRUE);

    return (HUNIT)p;
}

BOOL WINAPI ComboBox_Destroy(HUNIT hUnit)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p) return FALSE;
    if (p->base.hWnd && p->base.pfnOldProc)
        SetWindowLongPtrW(p->base.hWnd, GWLP_WNDPROC, (LONG_PTR)p->base.pfnOldProc);
    if (p->base.hWnd && IsWindow(p->base.hWnd)) DestroyWindow(p->base.hWnd);
    if (p->hFont) DeleteObject(p->hFont);
    Unit_Free((PWIN_UNIT_BASE)p);
    return TRUE;
}

static LRESULT CALLBACK ComboBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (!p || !p->base.pfnOldProc) return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    switch (uMsg)
    {
    case WM_SETFOCUS:  p->base.dwState|=UNIT_STATE_FOCUSED;  Unit_FireEvent(&p->base, EVENT_GOTFOCUS,0);  break;
    case WM_KILLFOCUS: p->base.dwState&=~UNIT_STATE_FOCUSED; Unit_FireEvent(&p->base, EVENT_LOSTFOCUS,0); break;
    case WM_DESTROY:   Unit_FireEvent(&p->base, EVENT_DESTROY, 0); break;
    }
    return CallWindowProcW(p->base.pfnOldProc, hWnd, uMsg, wParam, lParam);
}

/*=== 命令接口 ===*/

INT WINAPI ComboBox_AddItem(HUNIT hUnit, LPCSTR pszText)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return -1;
    LPWSTR pwsz = Unit_AnsiToWide(pszText, NULL);
    INT n = (INT)SendMessageW(p->base.hWnd, CB_ADDSTRING, 0, (LPARAM)(pwsz ? pwsz : L""));
    if (pwsz) free(pwsz);
    return (n < 0) ? -1 : n;
}

INT WINAPI ComboBox_InsertItem(HUNIT hUnit, INT nIndex, LPCSTR pszText)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return -1;
    LPWSTR pwsz = Unit_AnsiToWide(pszText, NULL);
    INT n = (INT)SendMessageW(p->base.hWnd, CB_INSERTSTRING, (WPARAM)nIndex, (LPARAM)(pwsz ? pwsz : L""));
    if (pwsz) free(pwsz);
    return (n < 0) ? -1 : n;
}

BOOL WINAPI ComboBox_DeleteItem(HUNIT hUnit, INT nIndex)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return FALSE;
    return SendMessage(p->base.hWnd, CB_DELETESTRING, (WPARAM)nIndex, 0) != CB_ERR;
}

BOOL WINAPI ComboBox_ClearAll(HUNIT hUnit)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return FALSE;
    SendMessage(p->base.hWnd, CB_RESETCONTENT, 0, 0);
    p->nSelIndex = -1;
    return TRUE;
}

LPSTR WINAPI ComboBox_GetItemText(HUNIT hUnit, INT nIndex)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return NULL;
    INT nLen = (INT)SendMessage(p->base.hWnd, CB_GETLBTEXTLEN, (WPARAM)nIndex, 0);
    if (nLen <= 0) return NULL;
    LPWSTR pwsz = (LPWSTR)malloc((nLen+2)*sizeof(WCHAR));
    if (!pwsz) return NULL;
    SendMessageW(p->base.hWnd, CB_GETLBTEXT, (WPARAM)nIndex, (LPARAM)pwsz);
    LPSTR pszRet = Unit_WideToAnsi(pwsz, NULL);
    free(pwsz);
    return pszRet;
}

LPSTR WINAPI ComboBox_GetText(HUNIT hUnit)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return NULL;
    INT nLen = GetWindowTextLengthW(p->base.hWnd);
    if (nLen <= 0) return NULL;
    LPWSTR pwsz = (LPWSTR)malloc((nLen+2)*sizeof(WCHAR));
    if (!pwsz) return NULL;
    GetWindowTextW(p->base.hWnd, pwsz, nLen+1);
    LPSTR pszRet = Unit_WideToAnsi(pwsz, NULL);
    free(pwsz);
    return pszRet;
}

BOOL WINAPI ComboBox_SetText(HUNIT hUnit, LPCSTR pszText)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return FALSE;
    LPWSTR pwsz = Unit_AnsiToWide(pszText, NULL);
    SetWindowTextW(p->base.hWnd, pwsz ? pwsz : L"");
    if (pwsz) free(pwsz);
    return TRUE;
}

INT WINAPI ComboBox_GetCount(HUNIT hUnit)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return 0;
    return (INT)SendMessage(p->base.hWnd, CB_GETCOUNT, 0, 0);
}

BOOL WINAPI ComboBox_SetSelection(HUNIT hUnit, INT nIndex)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return FALSE;
    SendMessage(p->base.hWnd, CB_SETCURSEL, (WPARAM)nIndex, 0);
    p->nSelIndex = nIndex;
    return TRUE;
}

INT WINAPI ComboBox_GetSelection(HUNIT hUnit)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return -1;
    return (INT)SendMessage(p->base.hWnd, CB_GETCURSEL, 0, 0);
}

BOOL WINAPI ComboBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    RECT rc = {0};
    if (p->base.hWnd) { GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2); }
    switch (nIndex)
    {
    case PROP_COMBOBOX_LEFT:     pValue->m_int  = rc.left;  return TRUE;
    case PROP_COMBOBOX_TOP:      pValue->m_int  = rc.top;   return TRUE;
    case PROP_COMBOBOX_WIDTH:    pValue->m_int  = rc.right  - rc.left; return TRUE;
    case PROP_COMBOBOX_HEIGHT:   pValue->m_int  = rc.bottom - rc.top;  return TRUE;
    case PROP_COMBOBOX_VISIBLE:  pValue->m_bool = (p->base.dwState & UNIT_STATE_VISIBLE)  != 0; return TRUE;
    case PROP_COMBOBOX_DISABLED: pValue->m_bool = (p->base.dwState & UNIT_STATE_DISABLED) != 0; return TRUE;
    case PROP_COMBOBOX_STYLE:    pValue->m_int  = p->nStyle; return TRUE;
    case PROP_COMBOBOX_SELINDEX: pValue->m_int  = ComboBox_GetSelection(hUnit); return TRUE;
    case PROP_COMBOBOX_BACKCOLOR:pValue->m_clr  = p->clrBack; return TRUE;
    case PROP_COMBOBOX_FORECOLOR:pValue->m_clr  = p->clrFore; return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI ComboBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PCOMBOBOX_DATA p = (PCOMBOBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_COMBOBOX_LEFT: case PROP_COMBOBOX_TOP: case PROP_COMBOBOX_WIDTH: case PROP_COMBOBOX_HEIGHT:
        if (p->base.hWnd) {
            RECT rc; GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2);
            if (nIndex==PROP_COMBOBOX_LEFT) rc.left=pValue->m_int;
            else if (nIndex==PROP_COMBOBOX_TOP) rc.top=pValue->m_int;
            else if (nIndex==PROP_COMBOBOX_WIDTH) rc.right=rc.left+pValue->m_int;
            else rc.bottom=rc.top+pValue->m_int;
            SetWindowPos(p->base.hWnd,NULL,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOZORDER);
        } return FALSE;
    case PROP_COMBOBOX_VISIBLE:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_VISIBLE; else p->base.dwState&=~UNIT_STATE_VISIBLE;
        if(p->base.hWnd) ShowWindow(p->base.hWnd, pValue->m_bool?SW_SHOW:SW_HIDE); return FALSE;
    case PROP_COMBOBOX_DISABLED:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_DISABLED; else p->base.dwState&=~UNIT_STATE_DISABLED;
        if(p->base.hWnd) EnableWindow(p->base.hWnd, !pValue->m_bool); return FALSE;
    case PROP_COMBOBOX_SELINDEX: ComboBox_SetSelection(hUnit, pValue->m_int); return FALSE;
    case PROP_COMBOBOX_BACKCOLOR: p->clrBack=pValue->m_clr; if(p->base.hWnd) InvalidateRect(p->base.hWnd,NULL,TRUE); return FALSE;
    case PROP_COMBOBOX_FORECOLOR: p->clrFore=pValue->m_clr; if(p->base.hWnd) InvalidateRect(p->base.hWnd,NULL,TRUE); return FALSE;
    default: return FALSE;
    }
}

BOOL WINAPI ComboBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex) { return TRUE; }

static HUNIT WINAPI ComboBox_Interface_Create(
    LPBYTE pAllData,INT nAllDataSize,DWORD dwStyle,HWND hParentWnd,
    UINT uID,HMENU hMenu,INT x,INT y,INT cx,INT cy,
    DWORD dwFormID,DWORD dwUnitID,HWND hDesignWnd,BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q={0};
    q.pAllData=pAllData;q.nAllDataSize=nAllDataSize;q.dwStyle=dwStyle;q.hParentWnd=hParentWnd;
    q.uID=uID;q.hMenu=hMenu;q.x=x;q.y=y;q.cx=cx;q.cy=cy;
    q.dwFormID=dwFormID;q.dwUnitID=dwUnitID;q.hDesignWnd=hDesignWnd;q.blInDesignMode=blInDesignMode;
    return ComboBox_Create(&q);
}

PFN_INTERFACE WINAPI ComboBox_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)ComboBox_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)ComboBox_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)ComboBox_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)ComboBox_PropertyUpdateUI;
    default: return NULL;
    }
}
