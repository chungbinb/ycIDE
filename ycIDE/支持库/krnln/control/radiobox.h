/**
 * @file radiobox.h
 * @brief 单选框组件定义
 * @date 2026-03-05
 */
#ifndef __KRNLN_RADIOBOX_H__
#define __KRNLN_RADIOBOX_H__
#include "control_base.h"

#define PROP_RADIOBOX_LEFT          0
#define PROP_RADIOBOX_TOP           1
#define PROP_RADIOBOX_WIDTH         2
#define PROP_RADIOBOX_HEIGHT        3
#define PROP_RADIOBOX_TAG           4
#define PROP_RADIOBOX_VISIBLE       5
#define PROP_RADIOBOX_DISABLED      6
#define PROP_RADIOBOX_CURSOR        7
#define PROP_RADIOBOX_CAPTION       8
#define PROP_RADIOBOX_CHECKED       9
#define PROP_RADIOBOX_FONT          10
#define PROP_RADIOBOX_BACKCOLOR     11
#define PROP_RADIOBOX_FORECOLOR     12

#define EVENT_RADIOBOX_CLICK        0

#pragma pack(push, 1)
typedef struct _RADIOBOX_DATA {
    WIN_UNIT_BASE   base;
    LPWSTR          pszCaption;
    HFONT           hFont;
    COLORREF        clrBack;
    COLORREF        clrFore;
    BOOL            bChecked;
} RADIOBOX_DATA, *PRADIOBOX_DATA;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI RadioBox_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI RadioBox_Destroy(HUNIT hUnit);
BOOL  WINAPI RadioBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI RadioBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI RadioBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI RadioBox_GetInterface(INT nInterfaceNO);

#ifdef __cplusplus
}
#endif
#endif // __KRNLN_RADIOBOX_H__
