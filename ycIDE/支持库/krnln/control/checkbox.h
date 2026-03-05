/**
 * @file checkbox.h
 * @brief 选择框组件定义
 * @date 2026-03-05
 */
#ifndef __KRNLN_CHECKBOX_H__
#define __KRNLN_CHECKBOX_H__
#include "control_base.h"

// 属性索引
#define PROP_CHECKBOX_LEFT          0
#define PROP_CHECKBOX_TOP           1
#define PROP_CHECKBOX_WIDTH         2
#define PROP_CHECKBOX_HEIGHT        3
#define PROP_CHECKBOX_TAG           4
#define PROP_CHECKBOX_VISIBLE       5
#define PROP_CHECKBOX_DISABLED      6
#define PROP_CHECKBOX_CURSOR        7
#define PROP_CHECKBOX_CAPTION       8   // 标题
#define PROP_CHECKBOX_CHECKED       9   // 选中状态 (0=未选,1=选中,2=不定)
#define PROP_CHECKBOX_FONT          10  // 字体
#define PROP_CHECKBOX_BACKCOLOR     11  // 背景颜色
#define PROP_CHECKBOX_FORECOLOR     12  // 前景颜色
#define PROP_CHECKBOX_TRISTATE      13  // 是否三态

// 选中状态
#define CHECKBOX_UNCHECKED          0
#define CHECKBOX_CHECKED            1
#define CHECKBOX_INDETERMINATE      2

// 事件
#define EVENT_CHECKBOX_CLICK        0   // 被单击（状态改变）

#pragma pack(push, 1)
typedef struct _CHECKBOX_DATA {
    WIN_UNIT_BASE   base;
    LPWSTR          pszCaption;
    HFONT           hFont;
    COLORREF        clrBack;
    COLORREF        clrFore;
    INT             nChecked;       // 0/1/2
    BOOL            bTriState;
} CHECKBOX_DATA, *PCHECKBOX_DATA;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI CheckBox_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI CheckBox_Destroy(HUNIT hUnit);
BOOL  WINAPI CheckBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI CheckBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI CheckBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI CheckBox_GetInterface(INT nInterfaceNO);

#ifdef __cplusplus
}
#endif
#endif // __KRNLN_CHECKBOX_H__
