/**
 * @file button.h
 * @brief 按钮组件定义
 * @date 2026-01-18
 */

#ifndef __KRNLN_BUTTON_H__
#define __KRNLN_BUTTON_H__

#include "control_base.h"

// ========== 按钮属性索引 ==========
// 固定属性（0-7由系统定义）
#define PROP_BUTTON_LEFT            0
#define PROP_BUTTON_TOP             1
#define PROP_BUTTON_WIDTH           2
#define PROP_BUTTON_HEIGHT          3
#define PROP_BUTTON_TAG             4
#define PROP_BUTTON_VISIBLE         5
#define PROP_BUTTON_DISABLED        6
#define PROP_BUTTON_CURSOR          7

// 按钮特有属性（从8开始）
#define PROP_BUTTON_CAPTION         8   // 标题
#define PROP_BUTTON_FONT            9   // 字体
#define PROP_BUTTON_PICTURE         10  // 图片
#define PROP_BUTTON_ALIGN           11  // 对齐方式
#define PROP_BUTTON_FLAT            12  // 平面
#define PROP_BUTTON_DEFAULT         13  // 默认按钮
#define PROP_BUTTON_BACKCOLOR       14  // 背景颜色
#define PROP_BUTTON_FORECOLOR       15  // 前景颜色

// ========== 对齐方式 ==========
#define BUTTON_ALIGN_LEFT           0   // 左对齐
#define BUTTON_ALIGN_CENTER         1   // 居中
#define BUTTON_ALIGN_RIGHT          2   // 右对齐

// ========== 按钮事件 ==========
#define EVENT_BUTTON_CLICK          0   // 被单击
#define EVENT_BUTTON_DBLCLICK       1   // 被双击

// ========== 按钮数据结构 ==========
#pragma pack(push, 1)

typedef struct _BUTTON_DATA {
    WIN_UNIT_BASE   base;           // 基础数据
    
    // 按钮特有数据
    LPWSTR          pszCaption;     // 标题
    HFONT           hFont;          // 字体句柄
    HBITMAP         hPicture;       // 图片
    COLORREF        clrBack;        // 背景颜色
    COLORREF        clrFore;        // 前景颜色
    INT             nAlign;         // 对齐方式
    BOOL            bFlat;          // 平面样式
    BOOL            bDefault;       // 默认按钮
    
    // 内部状态
    BOOL            bHover;         // 鼠标悬停
    BOOL            bPressed;       // 被按下
    
} BUTTON_DATA, *PBUTTON_DATA;

#pragma pack(pop)

// ========== 按钮组件函数声明 ==========
#ifdef __cplusplus
extern "C" {
#endif

// 创建和销毁
HUNIT WINAPI Button_Create(PUNIT_CREATE_PARAMS pParams);
BOOL WINAPI Button_Destroy(HUNIT hUnit);

// 属性操作
BOOL WINAPI Button_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL WINAPI Button_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL WINAPI Button_PropertyUpdateUI(HUNIT hUnit, INT nIndex);

// 接口获取
PFN_INTERFACE WINAPI Button_GetInterface(INT nInterfaceNO);

// 按钮命令
void WINAPI Button_SetCaption(PBUTTON_DATA pButton, LPCWSTR pszCaption);
LPCWSTR WINAPI Button_GetCaption(PBUTTON_DATA pButton);
void WINAPI Button_SetFont(PBUTTON_DATA pButton, const LOGFONTW* pLogFont);
void WINAPI Button_SetPicture(PBUTTON_DATA pButton, HBITMAP hBitmap);
void WINAPI Button_Click(PBUTTON_DATA pButton);  // 模拟单击

#ifdef __cplusplus
}
#endif

#endif // __KRNLN_BUTTON_H__
