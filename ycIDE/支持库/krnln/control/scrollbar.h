/**
 * @file scrollbar.h
 * @brief 横向/纵向滚动条组件定义
 * @date 2026-03-05
 */
#ifndef __KRNLN_SCROLLBAR_H__
#define __KRNLN_SCROLLBAR_H__
#include "control_base.h"

// 公用属性索引（横/纵向滚动条共享）
#define PROP_SCROLLBAR_LEFT         0
#define PROP_SCROLLBAR_TOP          1
#define PROP_SCROLLBAR_WIDTH        2
#define PROP_SCROLLBAR_HEIGHT       3
#define PROP_SCROLLBAR_TAG          4
#define PROP_SCROLLBAR_VISIBLE      5
#define PROP_SCROLLBAR_DISABLED     6
#define PROP_SCROLLBAR_CURSOR       7
#define PROP_SCROLLBAR_MIN          8   // 最小值
#define PROP_SCROLLBAR_MAX          9   // 最大值
#define PROP_SCROLLBAR_VALUE        10  // 当前值
#define PROP_SCROLLBAR_PAGESIZE     11  // 页大小（射击范围）
#define PROP_SCROLLBAR_SMALLCHANGE  PROP_SCROLLBAR_PAGESIZE  // 兼容别名
#define PROP_SCROLLBAR_LARGECHANGE  PROP_SCROLLBAR_PAGESIZE  // 兼容别名

// 事件
#define EVENT_SCROLLBAR_CHANGE      0   // 位置改变

#pragma pack(push, 1)
typedef struct _SCROLLBAR_DATA {
    WIN_UNIT_BASE   base;
    INT             nMin;
    INT             nMax;
    INT             nValue;
    INT             nPageSize;      // 页大小（射击范围）
    BOOL            bHorizontal;    // TRUE=横向, FALSE=纵向
    UINT            uMyID;          // 控件ID
} SCROLLBAR_DATA, *PSCROLLBAR_DATA;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI HScrollBar_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI HScrollBar_Destroy(HUNIT hUnit);
BOOL  WINAPI HScrollBar_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI HScrollBar_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI HScrollBar_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI HScrollBar_GetInterface(INT nInterfaceNO);

HUNIT WINAPI VScrollBar_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI VScrollBar_Destroy(HUNIT hUnit);
BOOL  WINAPI VScrollBar_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI VScrollBar_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI VScrollBar_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI VScrollBar_GetInterface(INT nInterfaceNO);

BOOL  WINAPI ScrollBar_Destroy(HUNIT hUnit);
BOOL  WINAPI ScrollBar_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI ScrollBar_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);

#ifdef __cplusplus
}
#endif
#endif // __KRNLN_SCROLLBAR_H__
