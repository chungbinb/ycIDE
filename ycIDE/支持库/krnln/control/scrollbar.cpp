/**
 * @file scrollbar.cpp
 * @brief 水平/垂直滚动条组件实现
 * @date 2026-03-05
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../include_krnln_header.h"
#include "scrollbar.h"
#include <stdlib.h>

/* 滚动条WM_HSCROLL/WM_VSCROLL由父窗口接收，需要子类化父窗口 */
typedef struct tagSB_PARENT_HOOK {
    HWND    hScrollBar;
    UINT    uID;
    BOOL    bHorizontal;
    WNDPROC pfnOldParent;
    struct tagSB_PARENT_HOOK* pNext;
} SB_PARENT_HOOK;

static SB_PARENT_HOOK* g_pSBHooks = NULL;

static LRESULT CALLBACK ScrollBar_ParentSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static void SB_RegisterParentHook(HWND hParent, HWND hScrollBar, UINT uID, BOOL bHorizontal)
{
    SB_PARENT_HOOK* pHook = (SB_PARENT_HOOK*)malloc(sizeof(SB_PARENT_HOOK));
    if (!pHook) return;
    pHook->hScrollBar = hScrollBar; pHook->uID = uID; pHook->bHorizontal = bHorizontal; pHook->pNext = g_pSBHooks;
    WNDPROC pfnCur = (WNDPROC)GetWindowLongPtrW(hParent, GWLP_WNDPROC);
    if (pfnCur != ScrollBar_ParentSubclass)
        pHook->pfnOldParent = (WNDPROC)SetWindowLongPtrW(hParent, GWLP_WNDPROC, (LONG_PTR)ScrollBar_ParentSubclass);
    else
        pHook->pfnOldParent = (WNDPROC)GetPropW(hParent, L"SB_OldProc");
    SetPropW(hParent, L"SB_OldProc", (HANDLE)pHook->pfnOldParent);
    g_pSBHooks = pHook;
}

static LRESULT CALLBACK ScrollBar_ParentSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WNDPROC pfnOld = (WNDPROC)GetPropW(hWnd, L"SB_OldProc");
    if (uMsg == WM_HSCROLL || uMsg == WM_VSCROLL)
    {
        HWND hCtrl = (HWND)lParam;
        if (hCtrl)
        {
            for (SB_PARENT_HOOK* pHook = g_pSBHooks; pHook; pHook = pHook->pNext)
            {
                if (pHook->hScrollBar == hCtrl)
                {
                    PSCROLLBAR_DATA pData = (PSCROLLBAR_DATA)GetWindowLongPtrW(hCtrl, GWLP_USERDATA);
                    if (pData) {
                        SCROLLINFO si = {0}; si.cbSize = sizeof(si); si.fMask = SIF_ALL;
                        GetScrollInfo(hCtrl, SB_CTL, &si);
                        INT nNewPos = si.nPos;
                        INT nCode = LOWORD(wParam);
                        switch (nCode) {
                        case SB_LINELEFT:    nNewPos = si.nPos - 1; break;
                        case SB_LINERIGHT:   nNewPos = si.nPos + 1; break;
                        case SB_PAGELEFT:    nNewPos = si.nPos - si.nPage; break;
                        case SB_PAGERIGHT:   nNewPos = si.nPos + si.nPage; break;
                        case SB_THUMBTRACK:
                        case SB_THUMBPOSITION: nNewPos = si.nTrackPos; break;
                        case SB_LEFT:        nNewPos = si.nMin; break;
                        case SB_RIGHT:       nNewPos = si.nMax; break;
                        }
                        if (nNewPos < si.nMin) nNewPos = si.nMin;
                        if (nNewPos > si.nMax - (INT)si.nPage + 1) nNewPos = si.nMax - (INT)si.nPage + 1;
                        if (nNewPos < si.nMin) nNewPos = si.nMin;
                        si.fMask = SIF_POS; si.nPos = nNewPos;
                        SetScrollInfo(hCtrl, SB_CTL, &si, TRUE);
                        pData->nValue = nNewPos;
                        Unit_FireEvent(&pData->base, EVENT_SCROLLBAR_CHANGE, 1, nNewPos);
                    }
                    break;
                }
            }
        }
    }
    return pfnOld ? CallWindowProcW(pfnOld, hWnd, uMsg, wParam, lParam) : DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

/* 通用创建函数 */
static HUNIT ScrollBar_CreateImpl(PUNIT_CREATE_PARAMS pParams, BOOL bHorizontal)
{
    if (!pParams) return NULL;
    PSCROLLBAR_DATA p = (PSCROLLBAR_DATA)Unit_Alloc(sizeof(SCROLLBAR_DATA) - sizeof(WIN_UNIT_BASE));
    if (!p) return NULL;

    p->base.dwFormID = pParams->dwFormID;
    p->base.dwUnitID = pParams->dwUnitID;
    p->base.hParent  = pParams->hParentWnd;
    p->bHorizontal   = bHorizontal;
    p->nMin = 0; p->nMax = 100; p->nValue = 0; p->nPageSize = 10;

    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_SCROLLBAR_VISIBLE, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_VISIBLE; }
        else p->base.dwState |= UNIT_STATE_VISIBLE;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_SCROLLBAR_DISABLED, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_DISABLED; }
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_SCROLLBAR_MIN,      &p->nMin);
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_SCROLLBAR_MAX,      &p->nMax);
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_SCROLLBAR_VALUE,    &p->nValue);
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_SCROLLBAR_PAGESIZE, &p->nPageSize);
    }
    else
    {
        p->base.dwState |= UNIT_STATE_VISIBLE;
    }

    DWORD dwStyle = WS_CHILD | (bHorizontal ? SBS_HORZ : SBS_VERT);
    if (p->base.dwState & UNIT_STATE_VISIBLE)  dwStyle |= WS_VISIBLE;
    if (p->base.dwState & UNIT_STATE_DISABLED) dwStyle |= WS_DISABLED;

    HWND hWnd = CreateWindowExW(0, L"SCROLLBAR", L"",
        dwStyle, pParams->x, pParams->y,
        pParams->cx > 0 ? pParams->cx : (bHorizontal ? 120 : 16),
        pParams->cy > 0 ? pParams->cy : (bHorizontal ? 16  : 120),
        pParams->hParentWnd, (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL), NULL);

    if (!hWnd) { Unit_Free((PWIN_UNIT_BASE)p); return NULL; }

    p->base.hWnd = hWnd;
    p->uMyID = pParams->uID;
    Unit_SetHWnd((PWIN_UNIT_BASE)p, hWnd);
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)p);

    SCROLLINFO si = {0};
    si.cbSize = sizeof(si); si.fMask = SIF_ALL;
    si.nMin = p->nMin; si.nMax = p->nMax;
    si.nPos = p->nValue; si.nPage = (UINT)p->nPageSize;
    SetScrollInfo(hWnd, SB_CTL, &si, TRUE);

    SB_RegisterParentHook(pParams->hParentWnd, hWnd, pParams->uID, bHorizontal);

    return (HUNIT)p;
}

HUNIT WINAPI HScrollBar_Create(PUNIT_CREATE_PARAMS pParams) { return ScrollBar_CreateImpl(pParams, TRUE);  }
HUNIT WINAPI VScrollBar_Create(PUNIT_CREATE_PARAMS pParams) { return ScrollBar_CreateImpl(pParams, FALSE); }

BOOL WINAPI ScrollBar_Destroy(HUNIT hUnit)
{
    PSCROLLBAR_DATA p = (PSCROLLBAR_DATA)hUnit;
    if (!p) return FALSE;
    if (p->base.hWnd && IsWindow(p->base.hWnd)) DestroyWindow(p->base.hWnd);
    Unit_Free((PWIN_UNIT_BASE)p);
    return TRUE;
}

BOOL WINAPI ScrollBar_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PSCROLLBAR_DATA p = (PSCROLLBAR_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    RECT rc = {0};
    if (p->base.hWnd) { GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2); }
    switch (nIndex)
    {
    case PROP_SCROLLBAR_LEFT:     pValue->m_int  = rc.left;  return TRUE;
    case PROP_SCROLLBAR_TOP:      pValue->m_int  = rc.top;   return TRUE;
    case PROP_SCROLLBAR_WIDTH:    pValue->m_int  = rc.right  - rc.left; return TRUE;
    case PROP_SCROLLBAR_HEIGHT:   pValue->m_int  = rc.bottom - rc.top;  return TRUE;
    case PROP_SCROLLBAR_VISIBLE:  pValue->m_bool = (p->base.dwState & UNIT_STATE_VISIBLE)  != 0; return TRUE;
    case PROP_SCROLLBAR_DISABLED: pValue->m_bool = (p->base.dwState & UNIT_STATE_DISABLED) != 0; return TRUE;
    case PROP_SCROLLBAR_MIN:      pValue->m_int  = p->nMin;      return TRUE;
    case PROP_SCROLLBAR_MAX:      pValue->m_int  = p->nMax;      return TRUE;
    case PROP_SCROLLBAR_VALUE:    pValue->m_int  = p->nValue;    return TRUE;
    case PROP_SCROLLBAR_PAGESIZE: pValue->m_int  = p->nPageSize; return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI ScrollBar_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PSCROLLBAR_DATA p = (PSCROLLBAR_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_SCROLLBAR_LEFT: case PROP_SCROLLBAR_TOP: case PROP_SCROLLBAR_WIDTH: case PROP_SCROLLBAR_HEIGHT:
        if (p->base.hWnd) {
            RECT rc; GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2);
            if (nIndex==PROP_SCROLLBAR_LEFT) rc.left=pValue->m_int;
            else if (nIndex==PROP_SCROLLBAR_TOP) rc.top=pValue->m_int;
            else if (nIndex==PROP_SCROLLBAR_WIDTH) rc.right=rc.left+pValue->m_int;
            else rc.bottom=rc.top+pValue->m_int;
            SetWindowPos(p->base.hWnd,NULL,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOZORDER);
        } return FALSE;
    case PROP_SCROLLBAR_VISIBLE:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_VISIBLE; else p->base.dwState&=~UNIT_STATE_VISIBLE;
        if(p->base.hWnd) ShowWindow(p->base.hWnd, pValue->m_bool?SW_SHOW:SW_HIDE); return FALSE;
    case PROP_SCROLLBAR_DISABLED:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_DISABLED; else p->base.dwState&=~UNIT_STATE_DISABLED;
        if(p->base.hWnd) EnableWindow(p->base.hWnd, !pValue->m_bool); return FALSE;
    case PROP_SCROLLBAR_MIN: case PROP_SCROLLBAR_MAX: case PROP_SCROLLBAR_VALUE: case PROP_SCROLLBAR_PAGESIZE:
    {
        if (nIndex == PROP_SCROLLBAR_MIN)      p->nMin      = pValue->m_int;
        else if (nIndex == PROP_SCROLLBAR_MAX) p->nMax      = pValue->m_int;
        else if (nIndex == PROP_SCROLLBAR_VALUE)   p->nValue    = pValue->m_int;
        else                                   p->nPageSize = pValue->m_int;
        if (p->base.hWnd) {
            SCROLLINFO si={0}; si.cbSize=sizeof(si); si.fMask=SIF_ALL;
            si.nMin=p->nMin; si.nMax=p->nMax; si.nPos=p->nValue; si.nPage=(UINT)p->nPageSize;
            SetScrollInfo(p->base.hWnd, SB_CTL, &si, TRUE);
        }
        return FALSE;
    }
    default: return FALSE;
    }
}

BOOL WINAPI ScrollBar_PropertyUpdateUI(HUNIT hUnit, INT nIndex) { return TRUE; }

/* ---- 水平滚动条 GetInterface ---- */
static HUNIT WINAPI HScrollBar_Interface_Create(
    LPBYTE pAllData,INT nAllDataSize,DWORD dwStyle,HWND hParentWnd,
    UINT uID,HMENU hMenu,INT x,INT y,INT cx,INT cy,
    DWORD dwFormID,DWORD dwUnitID,HWND hDesignWnd,BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q={0};
    q.pAllData=pAllData;q.nAllDataSize=nAllDataSize;q.dwStyle=dwStyle;q.hParentWnd=hParentWnd;
    q.uID=uID;q.hMenu=hMenu;q.x=x;q.y=y;q.cx=cx;q.cy=cy;
    q.dwFormID=dwFormID;q.dwUnitID=dwUnitID;q.hDesignWnd=hDesignWnd;q.blInDesignMode=blInDesignMode;
    return HScrollBar_Create(&q);
}
PFN_INTERFACE WINAPI HScrollBar_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)HScrollBar_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)ScrollBar_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)ScrollBar_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)ScrollBar_PropertyUpdateUI;
    default: return NULL;
    }
}

/* ---- 垂直滚动条 GetInterface ---- */
static HUNIT WINAPI VScrollBar_Interface_Create(
    LPBYTE pAllData,INT nAllDataSize,DWORD dwStyle,HWND hParentWnd,
    UINT uID,HMENU hMenu,INT x,INT y,INT cx,INT cy,
    DWORD dwFormID,DWORD dwUnitID,HWND hDesignWnd,BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q={0};
    q.pAllData=pAllData;q.nAllDataSize=nAllDataSize;q.dwStyle=dwStyle;q.hParentWnd=hParentWnd;
    q.uID=uID;q.hMenu=hMenu;q.x=x;q.y=y;q.cx=cx;q.cy=cy;
    q.dwFormID=dwFormID;q.dwUnitID=dwUnitID;q.hDesignWnd=hDesignWnd;q.blInDesignMode=blInDesignMode;
    return VScrollBar_Create(&q);
}
PFN_INTERFACE WINAPI VScrollBar_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)VScrollBar_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)ScrollBar_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)ScrollBar_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)ScrollBar_PropertyUpdateUI;
    default: return NULL;
    }
}
