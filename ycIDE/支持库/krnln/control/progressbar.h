/**
 * @file progressbar.h
 * @brief 进度条组件定义
 * @date 2026-03-05
 */
#ifndef __KRNLN_PROGRESSBAR_H__
#define __KRNLN_PROGRESSBAR_H__
#include "control_base.h"
#include <commctrl.h>

#define PROP_PROGRESSBAR_LEFT       0
#define PROP_PROGRESSBAR_TOP        1
#define PROP_PROGRESSBAR_WIDTH      2
#define PROP_PROGRESSBAR_HEIGHT     3
#define PROP_PROGRESSBAR_TAG        4
#define PROP_PROGRESSBAR_VISIBLE    5
#define PROP_PROGRESSBAR_DISABLED   6
#define PROP_PROGRESSBAR_CURSOR     7
#define PROP_PROGRESSBAR_MIN        8   // 最小值
#define PROP_PROGRESSBAR_MAX        9   // 最大值
#define PROP_PROGRESSBAR_VALUE      10  // 当前值
#define PROP_PROGRESSBAR_STEP       11  // 步进值
#define PROP_PROGRESSBAR_VERTICAL   12  // 是否纵向
#define PROP_PROGRESSBAR_SMOOTH     13  // 是否平滑
#define PROP_PROGRESSBAR_SHOWTEXT   14  // 显示百分比文本
#define PROP_PROGRESSBAR_BACKCOLOR  15  // 背景颜色
#define PROP_PROGRESSBAR_BARCOLOR   16  // 进度条颜色
#define PROP_PROGRESSBAR_FORECOLOR  PROP_PROGRESSBAR_BARCOLOR  // 兼容别名

#pragma pack(push, 1)
typedef struct _PROGRESSBAR_DATA {
    WIN_UNIT_BASE   base;
    INT             nMin;
    INT             nMax;
    INT             nValue;
    INT             nStep;
    BOOL            bVertical;
    BOOL            bSmooth;
    BOOL            bShowText;
    COLORREF        clrBack;
    COLORREF        clrBar;
} PROGRESSBAR_DATA, *PPROGRESSBAR_DATA;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI ProgressBar_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI ProgressBar_Destroy(HUNIT hUnit);
BOOL  WINAPI ProgressBar_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI ProgressBar_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI ProgressBar_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI ProgressBar_GetInterface(INT nInterfaceNO);

void WINAPI ProgressBar_StepIt(PPROGRESSBAR_DATA p);

#ifdef __cplusplus
}
#endif
#endif // __KRNLN_PROGRESSBAR_H__
