/**
 * @file groupbox.h
 * @brief 分组框组件定义
 * @date 2026-03-05
 */
#ifndef __KRNLN_GROUPBOX_H__
#define __KRNLN_GROUPBOX_H__
#include "control_base.h"

#define PROP_GROUPBOX_LEFT          0
#define PROP_GROUPBOX_TOP           1
#define PROP_GROUPBOX_WIDTH         2
#define PROP_GROUPBOX_HEIGHT        3
#define PROP_GROUPBOX_TAG           4
#define PROP_GROUPBOX_VISIBLE       5
#define PROP_GROUPBOX_DISABLED      6
#define PROP_GROUPBOX_CURSOR        7
#define PROP_GROUPBOX_CAPTION       8
#define PROP_GROUPBOX_FONT          9
#define PROP_GROUPBOX_BACKCOLOR     10
#define PROP_GROUPBOX_FORECOLOR     11

#pragma pack(push, 1)
typedef struct _GROUPBOX_DATA {
    WIN_UNIT_BASE   base;
    LPWSTR          pszCaption;
    HFONT           hFont;
    COLORREF        clrBack;
    COLORREF        clrFore;
} GROUPBOX_DATA, *PGROUPBOX_DATA;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI GroupBox_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI GroupBox_Destroy(HUNIT hUnit);
BOOL  WINAPI GroupBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI GroupBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI GroupBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI GroupBox_GetInterface(INT nInterfaceNO);

#ifdef __cplusplus
}
#endif
#endif // __KRNLN_GROUPBOX_H__
