/**
 * @file progressbar.cpp
 * @brief 进度条组件实现
 * @date 2026-03-05
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../include_krnln_header.h"
#include "progressbar.h"
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

HUNIT WINAPI ProgressBar_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    PPROGRESSBAR_DATA p = (PPROGRESSBAR_DATA)Unit_Alloc(sizeof(PROGRESSBAR_DATA) - sizeof(WIN_UNIT_BASE));
    if (!p) return NULL;

    p->base.dwFormID = pParams->dwFormID;
    p->base.dwUnitID = pParams->dwUnitID;
    p->base.hParent  = pParams->hParentWnd;
    p->nMin = 0; p->nMax = 100; p->nValue = 0; p->nStep = 10;
    p->clrBar  = CLR_DEFAULT;
    p->clrBack = CLR_DEFAULT;
    p->bVertical = FALSE;
    p->bSmooth   = TRUE;

    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_VISIBLE, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_VISIBLE; }
        else p->base.dwState |= UNIT_STATE_VISIBLE;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_DISABLED, &bValue))
        { if (bValue) p->base.dwState |= UNIT_STATE_DISABLED; }
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_MIN,   &p->nMin);
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_MAX,   &p->nMax);
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_VALUE, &p->nValue);
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_STEP,  &p->nStep);
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_VERTICAL, &bValue)) p->bVertical = bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_SMOOTH, &bValue))   p->bSmooth   = bValue;
        COLORREF clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_BARCOLOR,  &clr)) p->clrBar  = clr;
        if (Unit_ReadPropColor(pParams->pAllData, pParams->nAllDataSize, PROP_PROGRESSBAR_BACKCOLOR, &clr)) p->clrBack = clr;
    }
    else
    {
        p->base.dwState |= UNIT_STATE_VISIBLE;
    }

    /* 初始化 Common Controls */
    INITCOMMONCONTROLSEX icc = {sizeof(icc), ICC_PROGRESS_CLASS};
    InitCommonControlsEx(&icc);

    DWORD dwStyle = WS_CHILD;
    if (p->bVertical) dwStyle |= PBS_VERTICAL;
    if (p->bSmooth)   dwStyle |= PBS_SMOOTH;
    if (p->base.dwState & UNIT_STATE_VISIBLE)  dwStyle |= WS_VISIBLE;
    if (p->base.dwState & UNIT_STATE_DISABLED) dwStyle |= WS_DISABLED;

    HWND hWnd = CreateWindowExW(0, PROGRESS_CLASSW, L"",
        dwStyle, pParams->x, pParams->y,
        pParams->cx > 0 ? pParams->cx : 200,
        pParams->cy > 0 ? pParams->cy : 20,
        pParams->hParentWnd, (HMENU)(UINT_PTR)pParams->uID,
        GetModuleHandle(NULL), NULL);

    if (!hWnd) { Unit_Free((PWIN_UNIT_BASE)p); return NULL; }

    p->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)p, hWnd);
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)p);

    SendMessage(hWnd, PBM_SETRANGE32, (WPARAM)p->nMin, (LPARAM)p->nMax);
    SendMessage(hWnd, PBM_SETSTEP,    (WPARAM)p->nStep, 0);
    SendMessage(hWnd, PBM_SETPOS,     (WPARAM)p->nValue, 0);
    if (p->clrBar  != CLR_DEFAULT) SendMessage(hWnd, PBM_SETBARCOLOR, 0, (LPARAM)p->clrBar);
    if (p->clrBack != CLR_DEFAULT) SendMessage(hWnd, PBM_SETBKCOLOR,  0, (LPARAM)p->clrBack);

    return (HUNIT)p;
}

BOOL WINAPI ProgressBar_Destroy(HUNIT hUnit)
{
    PPROGRESSBAR_DATA p = (PPROGRESSBAR_DATA)hUnit;
    if (!p) return FALSE;
    if (p->base.hWnd && IsWindow(p->base.hWnd)) DestroyWindow(p->base.hWnd);
    Unit_Free((PWIN_UNIT_BASE)p);
    return TRUE;
}

BOOL WINAPI ProgressBar_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PPROGRESSBAR_DATA p = (PPROGRESSBAR_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    RECT rc = {0};
    if (p->base.hWnd) { GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2); }
    switch (nIndex)
    {
    case PROP_PROGRESSBAR_LEFT:     pValue->m_int  = rc.left;  return TRUE;
    case PROP_PROGRESSBAR_TOP:      pValue->m_int  = rc.top;   return TRUE;
    case PROP_PROGRESSBAR_WIDTH:    pValue->m_int  = rc.right  - rc.left; return TRUE;
    case PROP_PROGRESSBAR_HEIGHT:   pValue->m_int  = rc.bottom - rc.top;  return TRUE;
    case PROP_PROGRESSBAR_VISIBLE:  pValue->m_bool = (p->base.dwState & UNIT_STATE_VISIBLE)  != 0; return TRUE;
    case PROP_PROGRESSBAR_DISABLED: pValue->m_bool = (p->base.dwState & UNIT_STATE_DISABLED) != 0; return TRUE;
    case PROP_PROGRESSBAR_MIN:      pValue->m_int  = p->nMin;    return TRUE;
    case PROP_PROGRESSBAR_MAX:      pValue->m_int  = p->nMax;    return TRUE;
    case PROP_PROGRESSBAR_VALUE:    pValue->m_int  = p->nValue;  return TRUE;
    case PROP_PROGRESSBAR_STEP:     pValue->m_int  = p->nStep;   return TRUE;
    case PROP_PROGRESSBAR_VERTICAL: pValue->m_bool = p->bVertical; return TRUE;
    case PROP_PROGRESSBAR_SMOOTH:   pValue->m_bool = p->bSmooth;   return TRUE;
    case PROP_PROGRESSBAR_BARCOLOR: pValue->m_clr  = p->clrBar;    return TRUE;
    case PROP_PROGRESSBAR_BACKCOLOR:pValue->m_clr  = p->clrBack;   return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI ProgressBar_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PPROGRESSBAR_DATA p = (PPROGRESSBAR_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_PROGRESSBAR_LEFT: case PROP_PROGRESSBAR_TOP: case PROP_PROGRESSBAR_WIDTH: case PROP_PROGRESSBAR_HEIGHT:
        if (p->base.hWnd) {
            RECT rc; GetWindowRect(p->base.hWnd, &rc); MapWindowPoints(HWND_DESKTOP, p->base.hParent, (LPPOINT)&rc, 2);
            if (nIndex==PROP_PROGRESSBAR_LEFT) rc.left=pValue->m_int;
            else if (nIndex==PROP_PROGRESSBAR_TOP) rc.top=pValue->m_int;
            else if (nIndex==PROP_PROGRESSBAR_WIDTH) rc.right=rc.left+pValue->m_int;
            else rc.bottom=rc.top+pValue->m_int;
            SetWindowPos(p->base.hWnd,NULL,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top,SWP_NOZORDER);
        } return FALSE;
    case PROP_PROGRESSBAR_VISIBLE:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_VISIBLE; else p->base.dwState&=~UNIT_STATE_VISIBLE;
        if(p->base.hWnd) ShowWindow(p->base.hWnd, pValue->m_bool?SW_SHOW:SW_HIDE); return FALSE;
    case PROP_PROGRESSBAR_DISABLED:
        if(pValue->m_bool) p->base.dwState|=UNIT_STATE_DISABLED; else p->base.dwState&=~UNIT_STATE_DISABLED;
        if(p->base.hWnd) EnableWindow(p->base.hWnd, !pValue->m_bool); return FALSE;
    case PROP_PROGRESSBAR_MIN:
        p->nMin = pValue->m_int;
        if(p->base.hWnd) SendMessage(p->base.hWnd, PBM_SETRANGE32, (WPARAM)p->nMin, (LPARAM)p->nMax); return FALSE;
    case PROP_PROGRESSBAR_MAX:
        p->nMax = pValue->m_int;
        if(p->base.hWnd) SendMessage(p->base.hWnd, PBM_SETRANGE32, (WPARAM)p->nMin, (LPARAM)p->nMax); return FALSE;
    case PROP_PROGRESSBAR_VALUE:
        p->nValue = pValue->m_int;
        if(p->base.hWnd) SendMessage(p->base.hWnd, PBM_SETPOS, (WPARAM)p->nValue, 0); return FALSE;
    case PROP_PROGRESSBAR_STEP:
        p->nStep = pValue->m_int;
        if(p->base.hWnd) SendMessage(p->base.hWnd, PBM_SETSTEP, (WPARAM)p->nStep, 0); return FALSE;
    case PROP_PROGRESSBAR_BARCOLOR:
        p->clrBar = pValue->m_clr;
        if(p->base.hWnd) SendMessage(p->base.hWnd, PBM_SETBARCOLOR, 0, (LPARAM)p->clrBar); return FALSE;
    case PROP_PROGRESSBAR_BACKCOLOR:
        p->clrBack = pValue->m_clr;
        if(p->base.hWnd) SendMessage(p->base.hWnd, PBM_SETBKCOLOR, 0, (LPARAM)p->clrBack); return FALSE;
    default: return FALSE;
    }
}

BOOL WINAPI ProgressBar_PropertyUpdateUI(HUNIT hUnit, INT nIndex) { return TRUE; }

static HUNIT WINAPI ProgressBar_Interface_Create(
    LPBYTE pAllData,INT nAllDataSize,DWORD dwStyle,HWND hParentWnd,
    UINT uID,HMENU hMenu,INT x,INT y,INT cx,INT cy,
    DWORD dwFormID,DWORD dwUnitID,HWND hDesignWnd,BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q={0};
    q.pAllData=pAllData;q.nAllDataSize=nAllDataSize;q.dwStyle=dwStyle;q.hParentWnd=hParentWnd;
    q.uID=uID;q.hMenu=hMenu;q.x=x;q.y=y;q.cx=cx;q.cy=cy;
    q.dwFormID=dwFormID;q.dwUnitID=dwUnitID;q.hDesignWnd=hDesignWnd;q.blInDesignMode=blInDesignMode;
    return ProgressBar_Create(&q);
}

PFN_INTERFACE WINAPI ProgressBar_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)ProgressBar_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)ProgressBar_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)ProgressBar_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)ProgressBar_PropertyUpdateUI;
    default: return NULL;
    }
}
