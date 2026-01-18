/**
 * @file control_base.h
 * @brief 窗口组件基础定义
 * @date 2026-01-18
 * 
 * 本文件定义了易语言系统核心支持库中所有窗口组件的基础接口和结构
 */

#ifndef __KRNLN_CONTROL_BASE_H__
#define __KRNLN_CONTROL_BASE_H__

// 防止 Windows.h 定义 min/max 宏，与 std::min/std::max 冲突
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "../include_krnln_header.h"
#include <windows.h>

// 组件版本
#define CONTROL_VERSION_MAJOR   1
#define CONTROL_VERSION_MINOR   0

// 组件句柄类型
typedef HWND HWINUNIT;

// 事件ID定义
#define EVENT_CLICK             0       // 被单击
#define EVENT_DBLCLICK          1       // 被双击
#define EVENT_MOUSEDOWN         2       // 鼠标左键被按下
#define EVENT_MOUSEUP           3       // 鼠标左键被放开
#define EVENT_MOUSEMOVE         4       // 鼠标移动
#define EVENT_KEYDOWN           5       // 按下某键
#define EVENT_KEYUP             6       // 放开某键
#define EVENT_CHAR              7       // 字符输入
#define EVENT_GOTFOCUS          8       // 获得焦点
#define EVENT_LOSTFOCUS         9       // 失去焦点
#define EVENT_MOUSEENTER        10      // 鼠标进入
#define EVENT_MOUSELEAVE        11      // 鼠标离开
#define EVENT_CREATE            12      // 创建完毕
#define EVENT_DESTROY           13      // 销毁之前
#define EVENT_POSCHANGED        14      // 位置被改变
#define EVENT_SIZECHANGED       15      // 尺寸被改变

// 窗口组件状态标志
#define UNIT_STATE_VISIBLE      0x0001  // 可视
#define UNIT_STATE_DISABLED     0x0002  // 禁止
#define UNIT_STATE_FOCUSED      0x0004  // 拥有焦点
#define UNIT_STATE_CAPTURING    0x0008  // 正在捕获鼠标
#define UNIT_STATE_DRAGGING     0x0010  // 正在拖动

// 组件内部数据结构
#pragma pack(push, 1)

// 通用属性结构（所有窗口组件共用的8个固定属性）
typedef struct _UNIT_FIXED_PROPS {
    INT         left;           // 左边
    INT         top;            // 顶边
    INT         width;          // 宽度
    INT         height;         // 高度
    LPSTR       tag;            // 标记
    BOOL        visible;        // 可视
    BOOL        disabled;       // 禁止
    INT         cursor;         // 鼠标指针
} UNIT_FIXED_PROPS, *PUNIT_FIXED_PROPS;

// 窗口组件基础数据
typedef struct _WIN_UNIT_BASE {
    DWORD       dwFormID;       // 所属窗体ID
    DWORD       dwUnitID;       // 组件ID
    HWND        hWnd;           // Windows窗口句柄
    HWND        hParent;        // 父窗口句柄
    DATA_TYPE   dtDataType;     // 数据类型
    DWORD       dwState;        // 组件状态
    WNDPROC     pfnOldProc;     // 原窗口过程
    LPVOID      pUserData;      // 用户数据
} WIN_UNIT_BASE, *PWIN_UNIT_BASE;

#pragma pack(pop)

// 组件创建参数
typedef struct _UNIT_CREATE_PARAMS {
    LPBYTE      pAllData;       // 所有属性数据
    INT         nAllDataSize;   // 数据大小
    DWORD       dwStyle;        // Windows窗口样式
    HWND        hParentWnd;     // 父窗口
    UINT        uID;            // 控件ID
    HMENU       hMenu;          // 菜单（对窗体有效）
    INT         x, y, cx, cy;   // 位置和尺寸
    DWORD       dwFormID;       // 窗体ID
    DWORD       dwUnitID;       // 组件ID
    HWND        hDesignWnd;     // 设计窗口（设计时有效）
    BOOL        blInDesignMode; // 是否为设计模式
} UNIT_CREATE_PARAMS, *PUNIT_CREATE_PARAMS;

// 组件接口函数声明
typedef HUNIT (WINAPI* PFN_UNIT_CREATE)(PUNIT_CREATE_PARAMS pParams);
typedef BOOL  (WINAPI* PFN_UNIT_DESTROY)(HUNIT hUnit);
typedef BOOL  (WINAPI* PFN_UNIT_GET_PROPERTY)(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
typedef BOOL  (WINAPI* PFN_UNIT_SET_PROPERTY)(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
typedef BOOL  (WINAPI* PFN_UNIT_PROPERTY_UPDATE_UI)(HUNIT hUnit, INT nIndex);
typedef BOOL  (WINAPI* PFN_UNIT_IS_NEED_KEY)(HUNIT hUnit, WORD wKey);

// 组件接口表
typedef struct _UNIT_INTERFACE {
    PFN_UNIT_CREATE             pfnCreate;
    PFN_UNIT_DESTROY            pfnDestroy;
    PFN_UNIT_GET_PROPERTY       pfnGetProperty;
    PFN_UNIT_SET_PROPERTY       pfnSetProperty;
    PFN_UNIT_PROPERTY_UPDATE_UI pfnPropertyUpdateUI;
    PFN_UNIT_IS_NEED_KEY        pfnIsNeedKey;
} UNIT_INTERFACE, *PUNIT_INTERFACE;

// ============ 辅助函数声明 ============

#ifdef __cplusplus
extern "C" {
#endif

// 组件管理
PWIN_UNIT_BASE Unit_Alloc(SIZE_T nExtraSize);
void Unit_Free(PWIN_UNIT_BASE pUnit);
PWIN_UNIT_BASE Unit_FromHWnd(HWND hWnd);
void Unit_SetHWnd(PWIN_UNIT_BASE pUnit, HWND hWnd);

// 事件触发
void Unit_FireEvent(PWIN_UNIT_BASE pUnit, INT nEventIndex, INT nArgCount, ...);
void Unit_FireEvent2(PWIN_UNIT_BASE pUnit, INT nEventIndex, INT nArgCount, PEVENT_ARG_VALUE pArgs);

// 属性数据解析
LPBYTE Unit_ParsePropData(LPBYTE pData, INT nDataSize, INT nPropIndex, INT* pnPropSize);
BOOL Unit_ReadPropInt(LPBYTE pData, INT nDataSize, INT nPropIndex, INT* pValue);
BOOL Unit_ReadPropBool(LPBYTE pData, INT nDataSize, INT nPropIndex, BOOL* pValue);
BOOL Unit_ReadPropText(LPBYTE pData, INT nDataSize, INT nPropIndex, LPSTR* ppText);
BOOL Unit_ReadPropColor(LPBYTE pData, INT nDataSize, INT nPropIndex, COLORREF* pColor);
BOOL Unit_ReadPropFont(LPBYTE pData, INT nDataSize, INT nPropIndex, LOGFONTW* pFont);

// 窗口子类化
LRESULT CALLBACK Unit_SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void Unit_Subclass(PWIN_UNIT_BASE pUnit);
void Unit_Unsubclass(PWIN_UNIT_BASE pUnit);

// 颜色和字体
HFONT Unit_CreateFont(const LOGFONTW* pLogFont);
COLORREF Unit_ParseColorValue(INT nColorValue);
HBRUSH Unit_GetBackgroundBrush(PWIN_UNIT_BASE pUnit);

// 字符串转换（Unicode支持）
LPWSTR Unit_AnsiToWide(LPCSTR pszAnsi, INT* pnLen);
LPSTR Unit_WideToAnsi(LPCWSTR pszWide, INT* pnLen);
void Unit_FreeString(LPVOID pStr);

#ifdef __cplusplus
}
#endif

#endif // __KRNLN_CONTROL_BASE_H__
