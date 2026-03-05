/**
 * @file timer.cpp
 * @brief 定时器组件实现（无可见窗口，使用消息专用窗口）
 * @date 2026-03-05
 */
#define _CRT_SECURE_NO_WARNINGS
#include "../include_krnln_header.h"
#include "timer.h"
#include <stdlib.h>
#include <string.h>

#define TIMER_ID_INTERNAL 1

static LRESULT CALLBACK Timer_HostWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static const WCHAR g_szTimerHostClass[] = L"KrnlnTimerHost";
static BOOL g_bTimerClassRegistered = FALSE;

static void Timer_EnsureClass()
{
    if (g_bTimerClassRegistered) return;
    WNDCLASSEXW wc = {0};
    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = Timer_HostWndProc;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.lpszClassName = g_szTimerHostClass;
    RegisterClassExW(&wc);
    g_bTimerClassRegistered = TRUE;
}

HUNIT WINAPI Timer_Create(PUNIT_CREATE_PARAMS pParams)
{
    if (!pParams) return NULL;
    PTIMER_DATA p = (PTIMER_DATA)Unit_Alloc(sizeof(TIMER_DATA) - sizeof(WIN_UNIT_BASE));
    if (!p) return NULL;

    p->base.dwFormID = pParams->dwFormID;
    p->base.dwUnitID = pParams->dwUnitID;
    p->base.hParent  = pParams->hParentWnd;
    p->nInterval = 1000;
    p->bEnabled  = FALSE;

    if (pParams->pAllData && pParams->nAllDataSize > 0)
    {
        Unit_ReadPropInt(pParams->pAllData, pParams->nAllDataSize, PROP_TIMER_INTERVAL, &p->nInterval);
        if (p->nInterval <= 0) p->nInterval = 1000;
        BOOL bValue;
        if (Unit_ReadPropBool(pParams->pAllData, pParams->nAllDataSize, PROP_TIMER_ENABLED, &bValue))
            p->bEnabled = bValue;
    }

    /* 创建消息专用宿主窗口，不显示任何UI */
    Timer_EnsureClass();
    HWND hWnd = CreateWindowExW(0, g_szTimerHostClass, L"",
        0, 0, 0, 0, 0,
        HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);

    if (!hWnd) { Unit_Free((PWIN_UNIT_BASE)p); return NULL; }

    p->base.hWnd = hWnd;
    Unit_SetHWnd((PWIN_UNIT_BASE)p, hWnd);
    SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)p);

    /* 如果默认启用，立即启动定时器 */
    if (p->bEnabled)
        SetTimer(hWnd, TIMER_ID_INTERNAL, (UINT)p->nInterval, NULL);

    return (HUNIT)p;
}

BOOL WINAPI Timer_Destroy(HUNIT hUnit)
{
    PTIMER_DATA p = (PTIMER_DATA)hUnit;
    if (!p) return FALSE;
    if (p->base.hWnd) {
        KillTimer(p->base.hWnd, TIMER_ID_INTERNAL);
        if (IsWindow(p->base.hWnd)) DestroyWindow(p->base.hWnd);
    }
    Unit_Free((PWIN_UNIT_BASE)p);
    return TRUE;
}

static LRESULT CALLBACK Timer_HostWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_TIMER && wParam == TIMER_ID_INTERNAL)
    {
        PTIMER_DATA p = (PTIMER_DATA)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
        if (p) Unit_FireEvent(&p->base, EVENT_TIMER_TICK, 0);
        return 0;
    }
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

/*=== 命令接口 ===*/

BOOL WINAPI Timer_Start(HUNIT hUnit)
{
    PTIMER_DATA p = (PTIMER_DATA)hUnit;
    if (!p || !p->base.hWnd) return FALSE;
    if (p->bEnabled) return TRUE;
    p->bEnabled = TRUE;
    SetTimer(p->base.hWnd, TIMER_ID_INTERNAL, (UINT)(p->nInterval > 0 ? p->nInterval : 1000), NULL);
    return TRUE;
}

BOOL WINAPI Timer_Stop(HUNIT hUnit)
{
    PTIMER_DATA p = (PTIMER_DATA)hUnit;
    if (!p || !p->base.hWnd) return FALSE;
    p->bEnabled = FALSE;
    KillTimer(p->base.hWnd, TIMER_ID_INTERNAL);
    return TRUE;
}

BOOL WINAPI Timer_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue)
{
    PTIMER_DATA p = (PTIMER_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_TIMER_ENABLED:  pValue->m_bool = p->bEnabled;  return TRUE;
    case PROP_TIMER_INTERVAL: pValue->m_int  = p->nInterval; return TRUE;
    default: return FALSE;
    }
}

BOOL WINAPI Timer_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip)
{
    PTIMER_DATA p = (PTIMER_DATA)hUnit;
    if (!p || !pValue) return FALSE;
    switch (nIndex)
    {
    case PROP_TIMER_ENABLED:
        if (pValue->m_bool) Timer_Start(hUnit); else Timer_Stop(hUnit); return FALSE;
    case PROP_TIMER_INTERVAL:
        p->nInterval = pValue->m_int > 0 ? pValue->m_int : 1000;
        /* 如果正在运行，重新设置间隔 */
        if (p->bEnabled && p->base.hWnd) {
            KillTimer(p->base.hWnd, TIMER_ID_INTERNAL);
            SetTimer(p->base.hWnd, TIMER_ID_INTERNAL, (UINT)p->nInterval, NULL);
        }
        return FALSE;
    default: return FALSE;
    }
}

BOOL WINAPI Timer_PropertyUpdateUI(HUNIT hUnit, INT nIndex) { return TRUE; }

static HUNIT WINAPI Timer_Interface_Create(
    LPBYTE pAllData,INT nAllDataSize,DWORD dwStyle,HWND hParentWnd,
    UINT uID,HMENU hMenu,INT x,INT y,INT cx,INT cy,
    DWORD dwFormID,DWORD dwUnitID,HWND hDesignWnd,BOOL blInDesignMode)
{
    UNIT_CREATE_PARAMS q={0};
    q.pAllData=pAllData;q.nAllDataSize=nAllDataSize;q.dwStyle=dwStyle;q.hParentWnd=hParentWnd;
    q.uID=uID;q.hMenu=hMenu;q.x=x;q.y=y;q.cx=cx;q.cy=cy;
    q.dwFormID=dwFormID;q.dwUnitID=dwUnitID;q.hDesignWnd=hDesignWnd;q.blInDesignMode=blInDesignMode;
    return Timer_Create(&q);
}

PFN_INTERFACE WINAPI Timer_GetInterface(INT nInterfaceNO)
{
    switch (nInterfaceNO)
    {
    case ITF_CREATE_UNIT:             return (PFN_INTERFACE)Timer_Interface_Create;
    case ITF_GET_PROPERTY_DATA:       return (PFN_INTERFACE)Timer_GetProperty;
    case ITF_NOTIFY_PROPERTY_CHANGED: return (PFN_INTERFACE)Timer_SetProperty;
    case ITF_PROPERTY_UPDATE_UI:      return (PFN_INTERFACE)Timer_PropertyUpdateUI;
    default: return NULL;
    }
}
