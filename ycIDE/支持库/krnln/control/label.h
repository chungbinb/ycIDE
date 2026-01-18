/**
 * @file label.h
 * @brief 标签组件定义
 * @date 2026-01-18
 */

#ifndef __KRNLN_LABEL_H__
#define __KRNLN_LABEL_H__

#include "control_base.h"

// ========== 标签属性索引 ==========
#define PROP_LABEL_LEFT             0
#define PROP_LABEL_TOP              1
#define PROP_LABEL_WIDTH            2
#define PROP_LABEL_HEIGHT           3
#define PROP_LABEL_TAG              4
#define PROP_LABEL_VISIBLE          5
#define PROP_LABEL_DISABLED         6
#define PROP_LABEL_CURSOR           7

// 标签特有属性
#define PROP_LABEL_CAPTION          8   // 标题
#define PROP_LABEL_FONT             9   // 字体
#define PROP_LABEL_ALIGN            10  // 横向对齐
#define PROP_LABEL_VALIGN           11  // 纵向对齐
#define PROP_LABEL_AUTOSIZE         12  // 自动调整
#define PROP_LABEL_WORDWRAP         13  // 自动换行
#define PROP_LABEL_BORDERSTYLE      14  // 边框
#define PROP_LABEL_BACKCOLOR        15  // 背景颜色
#define PROP_LABEL_FORECOLOR        16  // 前景颜色
#define PROP_LABEL_TRANSPARENT      17  // 透明

// ========== 对齐方式 ==========
#define LABEL_ALIGN_LEFT            0   // 左对齐
#define LABEL_ALIGN_CENTER          1   // 居中
#define LABEL_ALIGN_RIGHT           2   // 右对齐

// ========== 纵向对齐 ==========
#define LABEL_VALIGN_TOP            0   // 顶端
#define LABEL_VALIGN_CENTER         1   // 居中
#define LABEL_VALIGN_BOTTOM         2   // 底端

// ========== 边框样式 ==========
#define LABEL_BORDER_NONE           0   // 无边框
#define LABEL_BORDER_SINGLE         1   // 凹入式单线边框
#define LABEL_BORDER_FIXED3D        2   // 固定立体边框

// ========== 标签事件 ==========
#define EVENT_LABEL_CLICK           0   // 被单击
#define EVENT_LABEL_DBLCLICK        1   // 被双击

// ========== 标签数据结构 ==========
#pragma pack(push, 1)

typedef struct _LABEL_DATA {
    WIN_UNIT_BASE   base;           // 基础数据
    
    LPWSTR          pszCaption;     // 标题
    HFONT           hFont;          // 字体句柄
    COLORREF        clrBack;        // 背景颜色
    COLORREF        clrFore;        // 前景颜色
    
    INT             nAlign;         // 横向对齐
    INT             nVAlign;        // 纵向对齐
    INT             nBorderStyle;   // 边框样式
    
    BOOL            bAutoSize;      // 自动调整
    BOOL            bWordWrap;      // 自动换行
    BOOL            bTransparent;   // 透明背景
    
} LABEL_DATA, *PLABEL_DATA;

#pragma pack(pop)

// ========== 标签组件函数声明 ==========
#ifdef __cplusplus
extern "C" {
#endif

HUNIT WINAPI Label_Create(PUNIT_CREATE_PARAMS pParams);
BOOL WINAPI Label_Destroy(HUNIT hUnit);
BOOL WINAPI Label_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL WINAPI Label_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL WINAPI Label_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
PFN_INTERFACE WINAPI Label_GetInterface(INT nInterfaceNO);

void WINAPI Label_SetCaption(PLABEL_DATA pLabel, LPCWSTR pszCaption);
LPCWSTR WINAPI Label_GetCaption(PLABEL_DATA pLabel);

#ifdef __cplusplus
}
#endif

#endif // __KRNLN_LABEL_H__
