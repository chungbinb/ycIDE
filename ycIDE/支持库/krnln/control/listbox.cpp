/**
 * @file listbox.cpp
 * @brief 列表框组件实现
 * @date 2026-03-05
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../include_krnln_header.h"
#include "listbox.h"
#include <stdlib.h>
#include <string.h>

static LRESULT CALLBACK ListBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK ListBox_ParentSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/** 子类化父窗口以捕获WM_COMMAND通知 */
typedef struct tagLB_PARENT_HOOK {
    HWND   hListBox;
    UINT   uID;
    WNDPROC pfnOldParent;
    struct tagLB_PARENT_HOOK* pNext;
} LB_PARENT_HOOK;

static LB_PARENT_HOOK* g_pHooks = NULL;

static void RegisterParentHook(HWND hParent, HWND hListBox, UINT uID, WNDPROC* ppOldProc)
{
    LB_PARENT_HOOK* pHook = (LB_PARENT_HOOK*)malloc(sizeof(LB_PARENT_HOOK));
    if (!pHook) return;
    pHook->hListBox = hListBox;
    pHook->uID = uID;
    pHook->pNext = g_pHooks;
    /* 如果父窗口已被挂钩，用原proc的值 */
    WNDPROC pfnCur = (WNDPROC)GetWindowLongPtrW(hParent, GWLP_WNDPROC);
    if (pfnCur != ListBox_ParentSubclass)
        pHook->pfnOldParent = (WNDPROC)SetWindowLongPtrW(hParent, GWLP_WNDPROC, (LONG_PTR)ListBox_ParentSubclass);
    else
        pHook->pfnOldParent = (WNDPROC)GetPropW(hParent, L"LB_OldProc");
    SetPropW(hParent, L"LB_OldProc", (HANDLE)pHook->pfnOldParent);
    g_pHooks = pHook;
    if (ppOldProc) *ppOldProc = NULL;
}

static LB_PARENT_HOOK* FindHookByListBox(HWND hListBox)
{
    for (LB_PARENT_HOOK* p = g_pHooks; p; p=p->pNext)
        if (p->hListBox == hListBox) return p;
    return NULL;
}

static LRESULT CALLBACK ListBox_ParentSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WNDPROC pfnOld = (WNDPROC)GetPropW(hWnd, L"LB_OldProc");
    if (uMsg == WM_COMMAND)
    {
        UINT uID = LOWORD(wParam);
        UINT uCode = HIWORD(wParam);
        if (uCode == LBN_SELCHANGE)
        {
            for (LB_PARENT_HOOK* pHook = g_pHooks; pHook; pHook = pHook->pNext)
            {
                if (pHook->uID == uID)
                {
                    PLISTBOX_DATA pData = (PLISTBOX_DATA)GetWindowLongPtrW(pHook->hListBox, GWLP_USERDATA);
                    if (pData) {
                        pData->nSelIndex = (INT)SendMessage(pHook->hListBox, LB_GETCURSEL, 0, 0);
                        Unit_FireEvent(&pData->base, EVENT_LISTBOX_SELCHANGE, 0);
                    }
                    break;
                }
            }
        }
    }
    return pfnOld ? CallWindowProcW(pfnOld, hWnd, uMsg, wParam, lParam) : DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

HUNIT WINAPI ListBox_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    PLISTBOX_DATA p = (PLISTBOX_DATA)Unit_Alloc(sizeof(LISTBOX_DATA) - sizeof(WIN_UNIT_BASE));
    if (!p) return NULL;

    p->base.dwFormID = pParams->dwFormID;
    p->base.dwUnitID = pParams->dwUnitID;
    p->base.hParent  = pParams->hParentWnd;
    p->clrBack = RGB(255,255,255);
    p->clrFore = GetSysColor(COLOR_WINDOWTEXT);
    p->nSelIndex = -1;
    p->bMultiSel = FALSE;

    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_LISTBOX_VISIBLE, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_VISIBLE; }
        else p->base.dwState |= UNIT_STATE_VISIBLE;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_LISTBOX_DISABLED, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_DISABLED; }
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_LISTBOX_MULTISEL, &bValue))
            p->bMultiSel = bValue;
        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_LISTBOX_BACKCOLOR, &clr)) p->clrBack = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_LISTBOX_FORECOLOR, &clr)) p->clrFore = clr;
        LOGFONTW lf;
        if (Unit_ReadPropFont(pParams->pAllData, pParams->nAllDataSize, PROP_LISTBOX_FONT, &lf))
            p->hFont = Unit_CreateFont(&lf);
    }
    else
    {
        p->base.dwState |= UNIT_STATE_VISIBLE;
    }

    DWORD dwStyle = WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS;
    if (p->bMultiSel) dwStyle |= LBS_MULTIPLESEL;
    if (p->base.dwState & UNIT_STATE_VISIBLE)  dwStyle |= WS_VISIBLE;
    if (p->base.dwState & UNIT_STATE_DISABLED) dwStyle |= WS_DISABLED;

    HWND hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", L"",
        dwStyle, pParams->x, pParams->y,
        pParams->cx > 0 ? pParams->cx : 120,
        pParams->cy > 0 ? pParams->cy : 100,
        pParams->hParentWnd, (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL), NULL);

    if (!hWnd) { Unit_Free((PWIN_UNIT_BASE)p); return NULL; }

    p->base.hWnd = hWnd;
    p->uMyID = pParams->uID;
    Unit_SetHWnd((PWIN_UNIT_BASE)p, hWnd);
    p->base.pfnOldProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)ListBox_SubclassProc);
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)p);

    RegisterParentHook(pParams->hParentWnd, hWnd, pParams->uID, NULL);

    SendMessage(hWnd, WM_SETFONT,
        (WPARAM)(p->hFont ? p->hFont : GetStockObject(DEFAULT_GUI_FONT)), TRUE);

    return (HUNIT)p;
}

BOOL WINAPI ListBox_Destroy(HUNIT hUnit)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p) return FALSE;
    if (p->base.hWnd && p->base.pfnOldProc)
        SetWindowLongPtrW(p->base.hWnd, GWLP_WNDPROC, (LONG_PTR)p->base.pfnOldProc);
    if (p->base.hWnd && IsWindow(p->base.hWnd)) DestroyWindow(p->base.hWnd);
    if (p->hFont) DeleteObject(p->hFont);
    Unit_Free((PWIN_UNIT_BASE)p);
    return TRUE;
}

static LRESULT CALLBACK ListBox_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (!p || !p->base.pfnOldProc) return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    switch (uMsg)
    {
    case WM_SETFOCUS:  p->base.dwState|=UNIT_STATE_FOCUSED;  Unit_FireEvent(&p->base, EVENT_GOTFOCUS,0);  break;
    case WM_KILLFOCUS: p->base.dwState&=~UNIT_STATE_FOCUSED; Unit_FireEvent(&p->base, EVENT_LOSTFOCUS,0); break;
    case WM_LBUTTONDBLCLK: Unit_FireEvent(&p->base, EVENT_LISTBOX_DBLCLICK, 0); break;
    case WM_DESTROY: Unit_FireEvent(&p->base, EVENT_DESTROY, 0); break;
    }
    return CallWindowProcW(p->base.pfnOldProc, hWnd, uMsg, wParam, lParam);
}

/*=== 命令接口 ===*/

INT WINAPI ListBox_AddItem(HUNIT hUnit, LPCSTR pszText)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return -1;
    LPWSTR pwsz = Unit_AnsiToWide(pszText, NULL);
    INT nIdx = (INT)SendMessageW(p->base.hWnd, LB_ADDSTRING, 0, (LPARAM)(pwsz ? pwsz : L""));
    if (pwsz) free(pwsz);
    return (nIdx >= 0) ? nIdx : -1;
}

INT WINAPI ListBox_InsertItem(HUNIT hUnit, INT nIndex, LPCSTR pszText)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return -1;
    LPWSTR pwsz = Unit_AnsiToWide(pszText, NULL);
    INT nIdx = (INT)SendMessageW(p->base.hWnd, LB_INSERTSTRING, (WPARAM)nIndex, (LPARAM)(pwsz ? pwsz : L""));
    if (pwsz) free(pwsz);
    return (nIdx >= 0) ? nIdx : -1;
}

BOOL WINAPI ListBox_DeleteItem(HUNIT hUnit, INT nIndex)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return FALSE;
    return SendMessage(p->base.hWnd, LB_DELETESTRING, (WPARAM)nIndex, 0) != LB_ERR;
}

BOOL WINAPI ListBox_ClearAll(HUNIT hUnit)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return FALSE;
    SendMessage(p->base.hWnd, LB_RESETCONTENT, 0, 0);
    p->nSelIndex = -1;
    return TRUE;
}

LPSTR WINAPI ListBox_GetItemText(HUNIT hUnit, INT nIndex)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return NULL;
    INT nLen = (INT)SendMessage(p->base.hWnd, LB_GETTEXTLEN, (WPARAM)nIndex, 0);
    if (nLen <= 0) return NULL;
    LPWSTR pwsz = (LPWSTR)malloc((nLen+2)*sizeof(WCHAR));
    if (!pwsz) return NULL;
    SendMessageW(p->base.hWnd, LB_GETTEXT, (WPARAM)nIndex, (LPARAM)pwsz);
    LPSTR pszRet = Unit_WideToAnsi(pwsz, NULL);
    free(pwsz);
    return pszRet;
}

INT WINAPI ListBox_FindItem(HUNIT hUnit, LPCSTR pszText)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return -1;
    LPWSTR pwsz = Unit_AnsiToWide(pszText, NULL);
    INT nIdx = (INT)SendMessageW(p->base.hWnd, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)(pwsz ? pwsz : L""));
    if (pwsz) free(pwsz);
    return (nIdx == LB_ERR) ? -1 : nIdx;
}

INT WINAPI ListBox_GetCount(HUNIT hUnit)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return 0;
    return (INT)SendMessage(p->base.hWnd, LB_GETCOUNT, 0, 0);
}

BOOL WINAPI ListBox_SetSelection(HUNIT hUnit, INT nIndex)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return FALSE;
    SendMessage(p->base.hWnd, LB_SETCURSEL, (WPARAM)nIndex, 0);
    p->nSelIndex = nIndex;
    return TRUE;
}

INT WINAPI ListBox_GetSelection(HUNIT hUnit)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !p->base.hWnd) return -1;
    return (INT)SendMessage(p->base.hWnd, LB_GETCURSEL, 0, 0);
}

BOOL WINAPI ListBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    RECT rc = {0};
    if (p->base.hWnd) { GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2); }
    switch (nIndex)
    {
    case PROP_LISTBOX_LEFT:     pValue->m_int  = rc.left;  return TRUE;
    case PROP_LISTBOX_TOP:      pValue->m_int  = rc.top;   return TRUE;
    case PROP_LISTBOX_WIDTH:    pValue->m_int  = rc.right  - rc.left; return TRUE;
    case PROP_LISTBOX_HEIGHT:   pValue->m_int  = rc.bottom - rc.top;  return TRUE;
    case PROP_LISTBOX_VISIBLE:  pValue->m_bool = (p->base.dwState & UNIT_STATE_VISIBLE)  != 0; return TRUE;
    case PROP_LISTBOX_DISABLED: pValue->m_bool = (p->base.dwState & UNIT_STATE_DISABLED) != 0; return TRUE;
    case PROP_LISTBOX_SELINDEX: pValue->m_int  = ListBox_GetSelection(hUnit);  return TRUE;
    case PROP_LISTBOX_MULTISEL: pValue->m_bool = p->bMultiSel; return TRUE;
    case PROP_LISTBOX_BACKCOLOR:pValue->m_clr  = p->clrBack; return TRUE;
    case PROP_LISTBOX_FORECOLOR:pValue->m_clr  = p->clrFore; return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI ListBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PLISTBOX_DATA p = (PLISTBOX_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_LISTBOX_LEFT: case PROP_LISTBOX_TOP: case PROP_LISTBOX_WIDTH: case PROP_LISTBOX_HEIGHT:
        if (p->base.hWnd) {
            RECT rc; GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2);
            if (nIndex==PROP_LISTBOX_LEFT) rc.left=pValue->m_int;
            else if (nIndex==PROP_LISTBOX_TOP) rc.top=pValue->m_int;
            else if (nIndex==PROP_LISTBOX_WIDTH) rc.right=rc.left+pValue->m_int;
            else rc.bottom=rc.top+pValue->m_int;
            SetWindowPos(p->base.hWnd,NULL,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOZORDER);
        } return FALSE;
    case PROP_LISTBOX_VISIBLE:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_VISIBLE; else p->base.dwState&=~UNIT_STATE_VISIBLE;
        if(p->base.hWnd) ShowWindow(p->base.hWnd, pValue->m_bool?SW_SHOW:SW_HIDE); return FALSE;
    case PROP_LISTBOX_DISABLED:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_DISABLED; else p->base.dwState&=~UNIT_STATE_DISABLED;
        if(p->base.hWnd) EnableWindow(p->base.hWnd, !pValue->m_bool); return FALSE;
    case PROP_LISTBOX_SELINDEX: ListBox_SetSelection(hUnit, pValue->m_int); return FALSE;
    case PROP_LISTBOX_BACKCOLOR: p->clrBack=pValue->m_clr; if(p->base.hWnd) InvalidateRect(p->base.hWnd,NULL,TRUE); return FALSE;
    case PROP_LISTBOX_FORECOLOR: p->clrFore=pValue->m_clr; if(p->base.hWnd) InvalidateRect(p->base.hWnd,NULL,TRUE); return FALSE;
    default: return FALSE;
    }
}

BOOL WINAPI ListBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex) { return TRUE; }

static HUNIT WINAPI ListBox_Interface_Create(
    LPBYTE pAllData,INT nAllDataSize,DWORD dwStyle,HWND hParentWnd,
    UINT uID,HMENU hMenu,INT x,INT y,INT cx,INT cy,
    DWORD dwFormID,DWORD dwUnitID,HWND hDesignWnd,BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q={0};
    q.pAllData=pAllData;q.nAllDataSize=nAllDataSize;q.dwStyle=dwStyle;q.hParentWnd=hParentWnd;
    q.uID=uID;q.hMenu=hMenu;q.x=x;q.y=y;q.cx=cx;q.cy=cy;
    q.dwFormID=dwFormID;q.dwUnitID=dwUnitID;q.hDesignWnd=hDesignWnd;q.blInDesignMode=blInDesignMode;
    return ListBox_Create(&q);
}

PFN_INTERFACE WINAPI ListBox_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)ListBox_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)ListBox_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)ListBox_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)ListBox_PropertyUpdateUI;
    default: return NULL;
    }
}
