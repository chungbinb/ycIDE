/**
 * @file sliderbar.cpp
 * @brief 滑块/轨道条组件实现
 * @date 2026-03-05
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../include_krnln_header.h"
#include "sliderbar.h"
#include <commctrl.h>
#include <stdlib.h>
#pragma comment(lib, "comctl32.lib")

static LRESULT CALLBACK SliderBar_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK SliderBar_ParentSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef struct tagTB_PARENT_HOOK {
    HWND    hTrackbar;
    BOOL    bHorizontal;
    WNDPROC pfnOldParent;
    struct tagTB_PARENT_HOOK* pNext;
} TB_PARENT_HOOK;
static TB_PARENT_HOOK* g_pTBHooks = NULL;

static void TB_RegisterParentHook(HWND hParent, HWND hTrackbar, BOOL bHorizontal)
{
    TB_PARENT_HOOK* pHook = (TB_PARENT_HOOK*)malloc(sizeof(TB_PARENT_HOOK));
    if (!pHook) return;
    pHook->hTrackbar = hTrackbar; pHook->bHorizontal = bHorizontal; pHook->pNext = g_pTBHooks;
    WNDPROC pfnCur = (WNDPROC)GetWindowLongPtrW(hParent, GWLP_WNDPROC);
    if (pfnCur != SliderBar_ParentSubclass)
        pHook->pfnOldParent = (WNDPROC)SetWindowLongPtrW(hParent, GWLP_WNDPROC, (LONG_PTR)SliderBar_ParentSubclass);
    else
        pHook->pfnOldParent = (WNDPROC)GetPropW(hParent, L"TB_OldProc");
    SetPropW(hParent, L"TB_OldProc", (HANDLE)pHook->pfnOldParent);
    g_pTBHooks = pHook;
}

static LRESULT CALLBACK SliderBar_ParentSubclass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WNDPROC pfnOld = (WNDPROC)GetPropW(hWnd, L"TB_OldProc");
    if (uMsg == WM_HSCROLL || uMsg == WM_VSCROLL)
    {
        HWND hCtrl = (HWND)lParam;
        if (hCtrl) {
            for (TB_PARENT_HOOK* pHook = g_pTBHooks; pHook; pHook = pHook->pNext) {
                if (pHook->hTrackbar == hCtrl) {
                    PSLIDERBAR_DATA pData = (PSLIDERBAR_DATA)GetWindowLongPtrW(hCtrl, GWLP_USERDATA);
                    if (pData) {
                        pData->nValue = (INT)SendMessage(hCtrl, TBM_GETPOS, 0, 0);
                        Unit_FireEvent(&pData->base, EVENT_SLIDERBAR_CHANGE, 1, pData->nValue);
                    }
                    break;
                }
            }
        }
    }
    return pfnOld ? CallWindowProcW(pfnOld, hWnd, uMsg, wParam, lParam) : DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

HUNIT WINAPI SliderBar_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    PSLIDERBAR_DATA p = (PSLIDERBAR_DATA)Unit_Alloc(sizeof(SLIDERBAR_DATA) - sizeof(WIN_UNIT_BASE));
    if (!p) return NULL;

    p->base.dwFormID = pParams->dwFormID;
    p->base.dwUnitID = pParams->dwUnitID;
    p->base.hParent  = pParams->hParentWnd;
    p->nMin = 0; p->nMax = 100; p->nValue = 0; p->nTickFreq = 10;
    p->bVertical = FALSE; p->bShowTick = TRUE;

    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_SLIDERBAR_VISIBLE, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_VISIBLE; }
        else p->base.dwState |= UNIT_STATE_VISIBLE;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_SLIDERBAR_DISABLED, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_DISABLED; }
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_SLIDERBAR_MIN,      &p->nMin);
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_SLIDERBAR_MAX,      &p->nMax);
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_SLIDERBAR_VALUE,    &p->nValue);
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_SLIDERBAR_TICKFREQ, &p->nTickFreq);
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_SLIDERBAR_VERTICAL, &bValue)) p->bVertical = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_SLIDERBAR_SHOWTICK, &bValue)) p->bShowTick = bValue;
    }
    else
    {
        p->base.dwState |= UNIT_STATE_VISIBLE;
    }

    INITCOMMONCONTROLSEX icc = {sizeof(icc), ICC_BAR_CLASSES};
    InitCommonControlsEx(&icc);

    DWORD dwStyle = WS_CHILD | WS_TABSTOP;
    if (p->bVertical)  dwStyle |= TBS_VERT;  else dwStyle |= TBS_HORZ;
    if (!p->bShowTick) dwStyle |= TBS_NOTICKS;
    if (p->base.dwState & UNIT_STATE_VISIBLE)  dwStyle |= WS_VISIBLE;
    if (p->base.dwState & UNIT_STATE_DISABLED) dwStyle |= WS_DISABLED;

    HWND hWnd = CreateWindowExW(0, TRACKBAR_CLASSW, L"",
        dwStyle, pParams->x, pParams->y,
        pParams->cx > 0 ? pParams->cx : (p->bVertical ? 30  : 200),
        pParams->cy > 0 ? pParams->cy : (p->bVertical ? 200 : 30),
        pParams->hParentWnd, (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL), NULL);

    if (!hWnd) { Unit_Free((PWIN_UNIT_BASE)p); return NULL; }

    p->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)p, hWnd);
    p->base.pfnOldProc = (WNDPROC)SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)SliderBar_SubclassProc);
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)p);

    SendMessage(hWnd, TBM_SETRANGEMIN, FALSE, (LPARAM)p->nMin);
    SendMessage(hWnd, TBM_SETRANGEMAX, FALSE, (LPARAM)p->nMax);
    SendMessage(hWnd, TBM_SETPOS,      TRUE,  (LPARAM)p->nValue);
    if (p->bShowTick) SendMessage(hWnd, TBM_SETTICFREQ, (WPARAM)p->nTickFreq, 0);

    TB_RegisterParentHook(pParams->hParentWnd, hWnd, p->bVertical ? FALSE : TRUE);

    return (HUNIT)p;
}

BOOL WINAPI SliderBar_Destroy(HUNIT hUnit)
{
    PSLIDERBAR_DATA p = (PSLIDERBAR_DATA)hUnit;
    if (!p) return FALSE;
    if (p->base.hWnd && p->base.pfnOldProc)
        SetWindowLongPtrW(p->base.hWnd, GWLP_WNDPROC, (LONG_PTR)p->base.pfnOldProc);
    if (p->base.hWnd && IsWindow(p->base.hWnd)) DestroyWindow(p->base.hWnd);
    Unit_Free((PWIN_UNIT_BASE)p);
    return TRUE;
}

static LRESULT CALLBACK SliderBar_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PSLIDERBAR_DATA p = (PSLIDERBAR_DATA)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
    if (!p || !p->base.pfnOldProc) return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    switch (uMsg)
    {
    case WM_SETFOCUS:  p->base.dwState|=UNIT_STATE_FOCUSED;  Unit_FireEvent(&p->base, EVENT_GOTFOCUS,0);  break;
    case WM_KILLFOCUS: p->base.dwState&=~UNIT_STATE_FOCUSED; Unit_FireEvent(&p->base, EVENT_LOSTFOCUS,0); break;
    case WM_DESTROY:   Unit_FireEvent(&p->base, EVENT_DESTROY, 0); break;
    }
    return CallWindowProcW(p->base.pfnOldProc, hWnd, uMsg, wParam, lParam);
}

BOOL WINAPI SliderBar_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PSLIDERBAR_DATA p = (PSLIDERBAR_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    RECT rc = {0};
    if (p->base.hWnd) { GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2); }
    switch (nIndex)
    {
    case PROP_SLIDERBAR_LEFT:     pValue->m_int  = rc.left;  return TRUE;
    case PROP_SLIDERBAR_TOP:      pValue->m_int  = rc.top;   return TRUE;
    case PROP_SLIDERBAR_WIDTH:    pValue->m_int  = rc.right  - rc.left; return TRUE;
    case PROP_SLIDERBAR_HEIGHT:   pValue->m_int  = rc.bottom - rc.top;  return TRUE;
    case PROP_SLIDERBAR_VISIBLE:  pValue->m_bool = (p->base.dwState & UNIT_STATE_VISIBLE)  != 0; return TRUE;
    case PROP_SLIDERBAR_DISABLED: pValue->m_bool = (p->base.dwState & UNIT_STATE_DISABLED) != 0; return TRUE;
    case PROP_SLIDERBAR_MIN:      pValue->m_int  = p->nMin;      return TRUE;
    case PROP_SLIDERBAR_MAX:      pValue->m_int  = p->nMax;      return TRUE;
    case PROP_SLIDERBAR_VALUE:    pValue->m_int  = p->nValue;    return TRUE;
    case PROP_SLIDERBAR_TICKFREQ: pValue->m_int  = p->nTickFreq; return TRUE;
    case PROP_SLIDERBAR_VERTICAL: pValue->m_bool = p->bVertical; return TRUE;
    case PROP_SLIDERBAR_SHOWTICK: pValue->m_bool = p->bShowTick; return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI SliderBar_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PSLIDERBAR_DATA p = (PSLIDERBAR_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_SLIDERBAR_LEFT: case PROP_SLIDERBAR_TOP: case PROP_SLIDERBAR_WIDTH: case PROP_SLIDERBAR_HEIGHT:
        if (p->base.hWnd) {
            RECT rc; GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2);
            if (nIndex==PROP_SLIDERBAR_LEFT) rc.left=pValue->m_int;
            else if (nIndex==PROP_SLIDERBAR_TOP) rc.top=pValue->m_int;
            else if (nIndex==PROP_SLIDERBAR_WIDTH) rc.right=rc.left+pValue->m_int;
            else rc.bottom=rc.top+pValue->m_int;
            SetWindowPos(p->base.hWnd,NULL,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOZORDER);
        } return FALSE;
    case PROP_SLIDERBAR_VISIBLE:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_VISIBLE; else p->base.dwState&=~UNIT_STATE_VISIBLE;
        if(p->base.hWnd) ShowWindow(p->base.hWnd, pValue->m_bool?SW_SHOW:SW_HIDE); return FALSE;
    case PROP_SLIDERBAR_DISABLED:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_DISABLED; else p->base.dwState&=~UNIT_STATE_DISABLED;
        if(p->base.hWnd) EnableWindow(p->base.hWnd, !pValue->m_bool); return FALSE;
    case PROP_SLIDERBAR_MIN:
        p->nMin = pValue->m_int;
        if(p->base.hWnd) { SendMessage(p->base.hWnd,TBM_SETRANGEMIN,TRUE,(LPARAM)p->nMin); } return FALSE;
    case PROP_SLIDERBAR_MAX:
        p->nMax = pValue->m_int;
        if(p->base.hWnd) { SendMessage(p->base.hWnd,TBM_SETRANGEMAX,TRUE,(LPARAM)p->nMax); } return FALSE;
    case PROP_SLIDERBAR_VALUE:
        p->nValue = pValue->m_int;
        if(p->base.hWnd) SendMessage(p->base.hWnd, TBM_SETPOS, TRUE, (LPARAM)p->nValue); return FALSE;
    case PROP_SLIDERBAR_TICKFREQ:
        p->nTickFreq = pValue->m_int;
        if(p->base.hWnd && p->bShowTick) SendMessage(p->base.hWnd, TBM_SETTICFREQ, (WPARAM)p->nTickFreq, 0); return FALSE;
    default: return FALSE;
    }
}

BOOL WINAPI SliderBar_PropertyUpdateUI(HUNIT hUnit, INT nIndex) { return TRUE; }

static HUNIT WINAPI SliderBar_Interface_Create(
    LPBYTE pAllData,INT nAllDataSize,DWORD dwStyle,HWND hParentWnd,
    UINT uID,HMENU hMenu,INT x,INT y,INT cx,INT cy,
    DWORD dwFormID,DWORD dwUnitID,HWND hDesignWnd,BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q={0};
    q.pAllData=pAllData;q.nAllDataSize=nAllDataSize;q.dwStyle=dwStyle;q.hParentWnd=hParentWnd;
    q.uID=uID;q.hMenu=hMenu;q.x=x;q.y=y;q.cx=cx;q.cy=cy;
    q.dwFormID=dwFormID;q.dwUnitID=dwUnitID;q.hDesignWnd=hDesignWnd;q.blInDesignMode=blInDesignMode;
    return SliderBar_Create(&q);
}

PFN_INTERFACE WINAPI SliderBar_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)SliderBar_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)SliderBar_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)SliderBar_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)SliderBar_PropertyUpdateUI;
    default: return NULL;
    }
}
