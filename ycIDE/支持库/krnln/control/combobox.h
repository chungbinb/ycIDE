/**
 * @file combobox.h
 * @brief 组合框组件定义
 * @date 2026-03-05
 */
#ifndef __KRNLN_COMBOBOX_H__
#define __KRNLN_COMBOBOX_H__
#include "control_base.h"

#define PROP_COMBOBOX_LEFT          0
#define PROP_COMBOBOX_TOP           1
#define PROP_COMBOBOX_WIDTH         2
#define PROP_COMBOBOX_HEIGHT        3
#define PROP_COMBOBOX_TAG           4
#define PROP_COMBOBOX_VISIBLE       5
#define PROP_COMBOBOX_DISABLED      6
#define PROP_COMBOBOX_CURSOR        7
#define PROP_COMBOBOX_ITEMS         8   // 初始项目
#define PROP_COMBOBOX_FONT          9
#define PROP_COMBOBOX_SORTED        10
#define PROP_COMBOBOX_STYLE         11  // 0=下拉列表式 1=简单 2=可输入下拉
#define PROP_COMBOBOX_SELINDEX      12  // 当前选中索引
#define PROP_COMBOBOX_BACKCOLOR     13
#define PROP_COMBOBOX_FORECOLOR     14

// 兼容别名
#define PROP_COMBOBOX_CURSEL        PROP_COMBOBOX_SELINDEX

// 组合框样式
#define COMBOBOX_STYLE_DROPDOWNLIST 0   // 下拉列表（只读）
#define COMBOBOX_STYLE_SIMPLE       1   // 简单（始终展开）
#define COMBOBOX_STYLE_DROPDOWN     2   // 可输入下拉

// 事件
#define EVENT_COMBOBOX_SELCHANGE    0   // 选择改变
#define EVENT_COMBOBOX_EDITCHANGE   1   // 编辑内容改变
#define EVENT_COMBOBOX_DROPDOWN     2   // 下拉框打开
#define EVENT_COMBOBOX_CLOSEUP      3   // 下拉框关闭

#pragma pack(push, 1)
typedef struct _COMBOBOX_DATA {
    WIN_UNIT_BASE   base;
    HFONT           hFont;
    COLORREF        clrBack;
    COLORREF        clrFore;
    INT             nStyle;         // COMBOBOX_STYLE_*
    INT             nSelIndex;      // 当前选中索引
    UINT            uMyID;          // 控件ID
} COMBOBOX_DATA, *PCOMBOBOX_DATA;
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI ComboBox_Create(PUNIT_CREATE_PARAMS pParams);
BOOL  WINAPI ComboBox_Destroy(HUNIT hUnit);
BOOL  WINAPI ComboBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL  WINAPI ComboBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL  WINAPI ComboBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI ComboBox_GetInterface(INT nInterfaceNO);

INT   WINAPI ComboBox_AddItem(HUNIT hUnit, LPCSTR pszText);
INT   WINAPI ComboBox_InsertItem(HUNIT hUnit, INT nIndex, LPCSTR pszText);
BOOL  WINAPI ComboBox_DeleteItem(HUNIT hUnit, INT nIndex);
BOOL  WINAPI ComboBox_ClearAll(HUNIT hUnit);
INT   WINAPI ComboBox_GetCount(HUNIT hUnit);
INT   WINAPI ComboBox_GetSelection(HUNIT hUnit);
BOOL  WINAPI ComboBox_SetSelection(HUNIT hUnit, INT nIndex);
LPSTR WINAPI ComboBox_GetItemText(HUNIT hUnit, INT nIndex);
LPSTR WINAPI ComboBox_GetText(HUNIT hUnit);
BOOL  WINAPI ComboBox_SetText(HUNIT hUnit, LPCSTR pszText);

#ifdef __cplusplus
}
#endif
#endif // __KRNLN_COMBOBOX_H__
