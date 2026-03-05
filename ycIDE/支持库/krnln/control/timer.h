/**
 * @file timer.h
 * @brief 时钟组件定义（功能型，无可视窗口）
 * @date 2026-03-05
 */
#ifndef __KRNLN_TIMER_H__
#define __KRNLN_TIMER_H__
#include "control_base.h"

#define PROP_TIMER_LEFT             0   // 保留（不使用）
#define PROP_TIMER_TOP              1
#define PROP_TIMER_WIDTH            2
#define PROP_TIMER_HEIGHT           3
#define PROP_TIMER_TAG              4
#define PROP_TIMER_VISIBLE          5   // 保留
#define PROP_TIMER_DISABLED         6   // 保留
#define PROP_TIMER_CURSOR           7
#define PROP_TIMER_INTERVAL         8   // 时钟周期（毫秒），0=停止
#define PROP_TIMER_ENABLED          9   // 是否启用

// 事件
#define EVENT_TIMER_TICK            0   // 时钟事件

// 时钟组件使用一个隐藏的宿主窗口来接收 WM_TIMER 消息
#pragma pack(push, 1)
typedef struct _TIMER_DATA {
    WIN_UNIT_BASE   base;       // base.hWnd 为宿主窗口句柄
    UINT            uTimerID;   // Win32 timer ID
    INT             nInterval;  // 周期毫秒
    BOOL            bEnabled;
} TIMER_DATA, *PTIMER_DATA;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI Timer_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI Timer_Destroy(HUNIT hUnit);
BOOL  WINAPI Timer_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI Timer_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI Timer_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI Timer_GetInterface(INT nInterfaceNO);

void WINAPI Timer_SetInterval(PTIMER_DATA p, INT nInterval);
void WINAPI Timer_SetEnabled(PTIMER_DATA p, BOOL bEnabled);

#ifdef __cplusplus
}
#endif
#endif // __KRNLN_TIMER_H__
