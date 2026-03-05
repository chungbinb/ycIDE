/**
 * @file listbox.h
 * @brief 列表框组件定义
 * @date 2026-03-05
 */
#ifndef __KRNLN_LISTBOX_H__
#define __KRNLN_LISTBOX_H__
#include "control_base.h"

// 属性索引
#define PROP_LISTBOX_LEFT           0
#define PROP_LISTBOX_TOP            1
#define PROP_LISTBOX_WIDTH          2
#define PROP_LISTBOX_HEIGHT         3
#define PROP_LISTBOX_TAG            4
#define PROP_LISTBOX_VISIBLE        5
#define PROP_LISTBOX_DISABLED       6
#define PROP_LISTBOX_CURSOR         7
#define PROP_LISTBOX_ITEMS          8   // 初始项目列表
#define PROP_LISTBOX_FONT           9
#define PROP_LISTBOX_SORTED         10  // 是否排序
#define PROP_LISTBOX_MULTISEL       11  // 是否多选
#define PROP_LISTBOX_SELINDEX       12  // 当前选中索引
#define PROP_LISTBOX_BACKCOLOR      13
#define PROP_LISTBOX_FORECOLOR      14

// 事件
#define EVENT_LISTBOX_SELCHANGE     0   // 选择被改变
#define EVENT_LISTBOX_DBLCLICK      1   // 被双击

#pragma pack(push, 1)
typedef struct _LISTBOX_DATA {
    WIN_UNIT_BASE   base;
    HFONT           hFont;
    COLORREF        clrBack;
    COLORREF        clrFore;
    INT             nSelIndex;      // 当前选中索引
    BOOL            bMultiSel;      // 是否多选
    UINT            uMyID;          // 控件ID
} LISTBOX_DATA, *PLISTBOX_DATA;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI ListBox_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI ListBox_Destroy(HUNIT hUnit);
BOOL  WINAPI ListBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI ListBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI ListBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI ListBox_GetInterface(INT nInterfaceNO);

// 列表框命令
INT   WINAPI ListBox_AddItem(HUNIT hUnit, LPCSTR pszText);
INT   WINAPI ListBox_InsertItem(HUNIT hUnit, INT nIndex, LPCSTR pszText);
BOOL  WINAPI ListBox_DeleteItem(HUNIT hUnit, INT nIndex);
BOOL  WINAPI ListBox_ClearAll(HUNIT hUnit);
INT   WINAPI ListBox_GetCount(HUNIT hUnit);
INT   WINAPI ListBox_GetSelection(HUNIT hUnit);
BOOL  WINAPI ListBox_SetSelection(HUNIT hUnit, INT nIndex);
LPSTR WINAPI ListBox_GetItemText(HUNIT hUnit, INT nIndex);
INT   WINAPI ListBox_FindItem(HUNIT hUnit, LPCSTR pszText);

#ifdef __cplusplus
}
#endif
#endif // __KRNLN_LISTBOX_H__
