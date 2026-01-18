/**
 * @file editbox.h
 * @brief 编辑框组件定义
 * @date 2026-01-18
 */

#ifndef __KRNLN_EDITBOX_H__
#define __KRNLN_EDITBOX_H__

#include "control_base.h"

// ========== 编辑框属性索引 ==========
// 固定属性（0-7由系统定义）
#define PROP_EDITBOX_LEFT           0
#define PROP_EDITBOX_TOP            1
#define PROP_EDITBOX_WIDTH          2
#define PROP_EDITBOX_HEIGHT         3
#define PROP_EDITBOX_TAG            4
#define PROP_EDITBOX_VISIBLE        5
#define PROP_EDITBOX_DISABLED       6
#define PROP_EDITBOX_CURSOR         7

// 编辑框特有属性（从8开始）
#define PROP_EDITBOX_CONTENT        8   // 内容
#define PROP_EDITBOX_FONT           9   // 字体
#define PROP_EDITBOX_BORDERSTYLE    10  // 边框
#define PROP_EDITBOX_ALIGN          11  // 对齐方式
#define PROP_EDITBOX_MULTILINE      12  // 是否允许多行
#define PROP_EDITBOX_SCROLLBAR      13  // 滚动条
#define PROP_EDITBOX_MAXLEN         14  // 最大允许长度
#define PROP_EDITBOX_PASSWORD       15  // 输入方式
#define PROP_EDITBOX_PWDCHAR        16  // 密码遮盖字符
#define PROP_EDITBOX_READONLY       17  // 是否只读
#define PROP_EDITBOX_HIDESEL        18  // 隐藏选择
#define PROP_EDITBOX_INPUTMODE      19  // 输入模式
#define PROP_EDITBOX_BACKCOLOR      20  // 背景颜色
#define PROP_EDITBOX_FORECOLOR      21  // 前景颜色

// ========== 边框样式 ==========
#define EDITBOX_BORDER_NONE         0   // 无边框
#define EDITBOX_BORDER_SINGLE       1   // 凹入式单线边框
#define EDITBOX_BORDER_FIXED3D      2   // 固定立体边框

// ========== 对齐方式 ==========
#define EDITBOX_ALIGN_LEFT          0   // 左对齐
#define EDITBOX_ALIGN_CENTER        1   // 居中
#define EDITBOX_ALIGN_RIGHT         2   // 右对齐

// ========== 滚动条 ==========
#define EDITBOX_SCROLLBAR_NONE      0   // 无滚动条
#define EDITBOX_SCROLLBAR_HORIZ     1   // 水平滚动条
#define EDITBOX_SCROLLBAR_VERT      2   // 垂直滚动条
#define EDITBOX_SCROLLBAR_BOTH      3   // 水平和垂直

// ========== 输入方式 ==========
#define EDITBOX_INPUT_NORMAL        0   // 通常方式
#define EDITBOX_INPUT_PASSWORD      1   // 密码输入

// ========== 输入模式 ==========
#define EDITBOX_MODE_ALL            0   // 全部字符
#define EDITBOX_MODE_NUMBER         1   // 整数文本
#define EDITBOX_MODE_FLOAT          2   // 小数文本
#define EDITBOX_MODE_ALPHA          3   // 英文字符

// ========== 编辑框事件 ==========
#define EVENT_EDITBOX_CHANGE        0   // 内容被改变
#define EVENT_EDITBOX_SELCHANGE     1   // 选择被改变
#define EVENT_EDITBOX_SCROLL        2   // 滚动条位置被改变

// ========== 编辑框数据结构 ==========
#pragma pack(push, 1)

typedef struct _EDITBOX_DATA {
    WIN_UNIT_BASE   base;           // 基础数据
    
    // 编辑框特有数据
    LPWSTR          pszContent;     // 内容
    HFONT           hFont;          // 字体句柄
    COLORREF        clrBack;        // 背景颜色
    COLORREF        clrFore;        // 前景颜色
    HBRUSH          hBackBrush;     // 背景画刷
    
    INT             nBorderStyle;   // 边框样式
    INT             nAlign;         // 对齐方式
    INT             nScrollBar;     // 滚动条
    INT             nMaxLen;        // 最大允许长度
    INT             nInputMode;     // 输入模式
    WCHAR           wchPwdChar;     // 密码遮盖字符
    
    BOOL            bMultiLine;     // 是否允许多行
    BOOL            bPassword;      // 密码输入
    BOOL            bReadOnly;      // 是否只读
    BOOL            bHideSel;       // 隐藏选择
    
} EDITBOX_DATA, *PEDITBOX_DATA;

#pragma pack(pop)

// ========== 编辑框组件函数声明 ==========
#ifdef __cplusplus
extern "C" {
#endif

// 创建和销毁
HUNIT WINAPI EditBox_Create(PUNIT_CREATE_PARAMS pParams);
BOOL WINAPI EditBox_Destroy(HUNIT hUnit);

// 属性操作
BOOL WINAPI EditBox_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL WINAPI EditBox_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL WINAPI EditBox_PropertyUpdateUI(HUNIT hUnit, INT nIndex);

// 接口获取
PFN_INTERFACE WINAPI EditBox_GetInterface(INT nInterfaceNO);

// 编辑框命令
void WINAPI EditBox_SetContent(PEDITBOX_DATA pEdit, LPCWSTR pszContent);
LPWSTR WINAPI EditBox_GetContent(PEDITBOX_DATA pEdit);
void WINAPI EditBox_AppendContent(PEDITBOX_DATA pEdit, LPCWSTR pszText);
void WINAPI EditBox_Clear(PEDITBOX_DATA pEdit);
void WINAPI EditBox_SetSel(PEDITBOX_DATA pEdit, INT nStart, INT nEnd);
void WINAPI EditBox_GetSel(PEDITBOX_DATA pEdit, INT* pnStart, INT* pnEnd);
LPWSTR WINAPI EditBox_GetSelText(PEDITBOX_DATA pEdit);
void WINAPI EditBox_ReplaceSel(PEDITBOX_DATA pEdit, LPCWSTR pszText);
INT WINAPI EditBox_GetLineCount(PEDITBOX_DATA pEdit);
INT WINAPI EditBox_GetLine(PEDITBOX_DATA pEdit, INT nLine, LPWSTR pszBuffer, INT nBufferSize);
void WINAPI EditBox_ScrollCaret(PEDITBOX_DATA pEdit);
BOOL WINAPI EditBox_CanUndo(PEDITBOX_DATA pEdit);
void WINAPI EditBox_Undo(PEDITBOX_DATA pEdit);
void WINAPI EditBox_Cut(PEDITBOX_DATA pEdit);
void WINAPI EditBox_Copy(PEDITBOX_DATA pEdit);
void WINAPI EditBox_Paste(PEDITBOX_DATA pEdit);
void WINAPI EditBox_SelectAll(PEDITBOX_DATA pEdit);

#ifdef __cplusplus
}
#endif

#endif // __KRNLN_EDITBOX_H__
