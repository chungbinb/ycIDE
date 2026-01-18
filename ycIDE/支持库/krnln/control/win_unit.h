/**
 * @file win_unit.h
 * @brief 窗口组件定义 - 所有可视化组件的基类
 * @date 2026-01-18
 */

#ifndef __KRNLN_WIN_UNIT_H__
#define __KRNLN_WIN_UNIT_H__

#include "control_base.h"

// ========== 窗口（窗体）组件属性索引 ==========
// 固定属性（0-7由系统定义）
#define PROP_WINDOW_LEFT            0   // 左边
#define PROP_WINDOW_TOP             1   // 顶边  
#define PROP_WINDOW_WIDTH           2   // 宽度
#define PROP_WINDOW_HEIGHT          3   // 高度
#define PROP_WINDOW_TAG             4   // 标记
#define PROP_WINDOW_VISIBLE         5   // 可视
#define PROP_WINDOW_DISABLED        6   // 禁止
#define PROP_WINDOW_CURSOR          7   // 鼠标指针

// 窗口特有属性（从8开始）
#define PROP_WINDOW_TITLE           8   // 标题
#define PROP_WINDOW_BORDERSTYLE     9   // 边框
#define PROP_WINDOW_MAXBUTTON       10  // 最大化按钮
#define PROP_WINDOW_MINBUTTON       11  // 最小化按钮
#define PROP_WINDOW_CONTROLBOX      12  // 控制按钮
#define PROP_WINDOW_ICON            13  // 图标
#define PROP_WINDOW_BACKCOLOR       14  // 背景颜色
#define PROP_WINDOW_STARTPOS        15  // 启动位置
#define PROP_WINDOW_MOVEABLE        16  // 可否移动
#define PROP_WINDOW_HELPFILE        17  // 帮助文件
#define PROP_WINDOW_SHOWTASKBAR     18  // 显示在任务条
#define PROP_WINDOW_TOPMOST         19  // 总在最前
#define PROP_WINDOW_OPACITY         20  // 不透明度
#define PROP_WINDOW_TRANSCOLOR      21  // 透明色
#define PROP_WINDOW_FONT            22  // 字体
#define PROP_WINDOW_BACKPIC         23  // 底图
#define PROP_WINDOW_BACKPICMODE     24  // 底图模式
#define PROP_WINDOW_SYSMENU         25  // 弹出系统菜单

// ========== 窗口边框样式 ==========
#define WINDOW_BORDER_NONE          0   // 无边框
#define WINDOW_BORDER_FIXED_SINGLE  1   // 固定单边框
#define WINDOW_BORDER_SIZABLE       2   // 可调整边框
#define WINDOW_BORDER_FIXED_DIALOG  3   // 固定对话框边框
#define WINDOW_BORDER_FIXED_TOOL    4   // 固定工具条边框
#define WINDOW_BORDER_SIZABLE_TOOL  5   // 可调整工具条边框

// ========== 窗口启动位置 ==========
#define WINDOW_STARTPOS_MANUAL      0   // 手工调整
#define WINDOW_STARTPOS_CENTER_OWNER 1  // 相对于父窗口居中
#define WINDOW_STARTPOS_CENTER_SCREEN 2 // 相对于屏幕居中
#define WINDOW_STARTPOS_DEFAULT     3   // 系统默认

// ========== 底图显示模式 ==========
#define BACKPIC_MODE_TOPLEFT        0   // 图片居左上
#define BACKPIC_MODE_CENTER         1   // 图片居中
#define BACKPIC_MODE_STRETCH        2   // 缩放图片
#define BACKPIC_MODE_TILE           3   // 平铺图片

// ========== 窗口事件定义 ==========
#define EVENT_WINDOW_LOAD           0   // 创建完毕
#define EVENT_WINDOW_UNLOAD         1   // 将被销毁
#define EVENT_WINDOW_ACTIVATE       2   // 被激活
#define EVENT_WINDOW_DEACTIVATE     3   // 被取消激活
#define EVENT_WINDOW_PAINT          4   // 需要重画
#define EVENT_WINDOW_CANCLOSE       5   // 可否被关闭
#define EVENT_WINDOW_RESIZE         6   // 尺寸被改变
#define EVENT_WINDOW_MOVE           7   // 位置被改变
#define EVENT_WINDOW_QUERYUNLOAD    8   // 尝试关闭窗口
#define EVENT_WINDOW_DROPFILE       9   // 拖放文件
#define EVENT_WINDOW_KEYDOWN        10  // 按下某键（预处理）
#define EVENT_WINDOW_CHAR           11  // 字符输入（预处理）
#define EVENT_WINDOW_SCROLL         12  // 滚动条位置被改变
#define EVENT_WINDOW_CONTEXTMENU    13  // 即将弹出菜单

// ========== 窗口组件数据结构 ==========
#pragma pack(push, 1)

typedef struct _WINDOW_DATA {
    WIN_UNIT_BASE   base;           // 基础数据
    
    // 窗口特有数据
    LPWSTR          pszTitle;       // 标题
    COLORREF        clrBack;        // 背景颜色
    HFONT           hFont;          // 字体句柄
    HICON           hIcon;          // 图标句柄
    HBITMAP         hBackPic;       // 底图位图
    
    INT             nBorderStyle;   // 边框样式
    INT             nStartPos;      // 启动位置
    INT             nBackPicMode;   // 底图模式
    INT             nOpacity;       // 不透明度 (0-255)
    COLORREF        clrTrans;       // 透明色
    
    BOOL            bMaxButton;     // 最大化按钮
    BOOL            bMinButton;     // 最小化按钮
    BOOL            bControlBox;    // 控制按钮
    BOOL            bMoveable;      // 可否移动
    BOOL            bShowTaskbar;   // 显示在任务条
    BOOL            bTopMost;       // 总在最前
    BOOL            bSysMenu;       // 弹出系统菜单
    
    // 内部状态
    BOOL            bIsClosing;     // 正在关闭
    BOOL            bIsModal;       // 模态显示
    INT             nModalResult;   // 模态返回值
    
} WINDOW_DATA, *PWINDOW_DATA;

#pragma pack(pop)

// ========== 窗口组件命令函数声明 ==========
#ifdef __cplusplus
extern "C" {
#endif

// 创建和销毁
HUNIT WINAPI Window_Create(PUNIT_CREATE_PARAMS pParams);
BOOL WINAPI Window_Destroy(HUNIT hUnit);

// 属性操作
BOOL WINAPI Window_GetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue);
BOOL WINAPI Window_SetProperty(HUNIT hUnit, INT nIndex, PUNIT_PROPERTY_VALUE pValue, LPCSTR* ppszTip);
BOOL WINAPI Window_PropertyUpdateUI(HUNIT hUnit, INT nIndex);
HGLOBAL WINAPI Window_GetAllPropertyData(HUNIT hUnit);

// 接口获取
PFN_INTERFACE WINAPI Window_GetInterface(INT nInterfaceNO);

// 窗口命令
void WINAPI Window_Show(PWINDOW_DATA pWindow);
void WINAPI Window_Hide(PWINDOW_DATA pWindow);
INT WINAPI Window_ShowModal(PWINDOW_DATA pWindow);
void WINAPI Window_Close(PWINDOW_DATA pWindow);
void WINAPI Window_Move(PWINDOW_DATA pWindow, INT x, INT y);
void WINAPI Window_Resize(PWINDOW_DATA pWindow, INT cx, INT cy);
void WINAPI Window_Center(PWINDOW_DATA pWindow, BOOL bRelativeToScreen);
void WINAPI Window_SetTopMost(PWINDOW_DATA pWindow, BOOL bTopMost);
void WINAPI Window_Activate(PWINDOW_DATA pWindow);
void WINAPI Window_SetTitle(PWINDOW_DATA pWindow, LPCWSTR pszTitle);
LPCWSTR WINAPI Window_GetTitle(PWINDOW_DATA pWindow);
void WINAPI Window_SetBackColor(PWINDOW_DATA pWindow, COLORREF clr);
COLORREF WINAPI Window_GetBackColor(PWINDOW_DATA pWindow);
void WINAPI Window_SetOpacity(PWINDOW_DATA pWindow, INT nOpacity);
INT WINAPI Window_GetOpacity(PWINDOW_DATA pWindow);
void WINAPI Window_Invalidate(PWINDOW_DATA pWindow, BOOL bErase);
void WINAPI Window_Update(PWINDOW_DATA pWindow);

// 坐标转换
void WINAPI Window_ClientToScreen(PWINDOW_DATA pWindow, LPPOINT ppt);
void WINAPI Window_ScreenToClient(PWINDOW_DATA pWindow, LPPOINT ppt);

#ifdef __cplusplus
}
#endif

#endif // __KRNLN_WIN_UNIT_H__
