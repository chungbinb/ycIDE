/**
 * @file sliderbar.h
 * @brief 滑块条组件定义
 * @date 2026-03-05
 */
#ifndef __KRNLN_SLIDERBAR_H__
#define __KRNLN_SLIDERBAR_H__
#include "control_base.h"
#include <commctrl.h>

#define PROP_SLIDERBAR_LEFT         0
#define PROP_SLIDERBAR_TOP          1
#define PROP_SLIDERBAR_WIDTH        2
#define PROP_SLIDERBAR_HEIGHT       3
#define PROP_SLIDERBAR_TAG          4
#define PROP_SLIDERBAR_VISIBLE      5
#define PROP_SLIDERBAR_DISABLED     6
#define PROP_SLIDERBAR_CURSOR       7
#define PROP_SLIDERBAR_MIN          8
#define PROP_SLIDERBAR_MAX          9
#define PROP_SLIDERBAR_VALUE        10
#define PROP_SLIDERBAR_TICKFREQ     11  // 刻度间距
#define PROP_SLIDERBAR_VERTICAL     12  // 是否纵向
#define PROP_SLIDERBAR_SHOWTICK     13  // 是否显示刻度

// 兼容别名
#define PROP_SLIDERBAR_SMALLCHANGE  PROP_SLIDERBAR_TICKFREQ
#define PROP_SLIDERBAR_LARGECHANGE  PROP_SLIDERBAR_TICKFREQ
#define PROP_SLIDERBAR_TICKSTYLE    PROP_SLIDERBAR_SHOWTICK

// 刻度位置
#define SLIDERBAR_TICK_NONE         0
#define SLIDERBAR_TICK_BOTTOM_RIGHT 1
#define SLIDERBAR_TICK_TOP_LEFT     2
#define SLIDERBAR_TICK_BOTH         3

// 事件
#define EVENT_SLIDERBAR_CHANGE      0

#pragma pack(push, 1)
typedef struct _SLIDERBAR_DATA {
    WIN_UNIT_BASE   base;
    INT             nMin;
    INT             nMax;
    INT             nValue;
    INT             nTickFreq;      // 刻度间距
    BOOL            bVertical;      // 是否纵向
    BOOL            bShowTick;      // 是否显示刻度
} SLIDERBAR_DATA, *PSLIDERBAR_DATA;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI SliderBar_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI SliderBar_Destroy(HUNIT hUnit);
BOOL  WINAPI SliderBar_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI SliderBar_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI SliderBar_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI SliderBar_GetInterface(INT nInterfaceNO);

#ifdef __cplusplus
}
#endif
#endif // __KRNLN_SLIDERBAR_H__
