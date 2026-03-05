/**
 * @file controls.h
 * @brief 窗口组件统一头文件
 * @date 2026-01-18
 * 
 * 包含所有窗口组件的定义
 */

#ifndef __KRNLN_CONTROLS_H__
#define __KRNLN_CONTROLS_H__

// 基础定义
#include "control_base.h"

// 窗口组件
#include "win_unit.h"

// 编辑框组件
#include "editbox.h"

// 按钮组件
#include "button.h"

// 标签组件
#include "label.h"

// 选择框组件
#include "checkbox.h"

// 单选框组件
#include "radiobox.h"

// 分组框组件
#include "groupbox.h"

// 列表框组件
#include "listbox.h"

// 组合框组件
#include "combobox.h"

// 滚动条组件
#include "scrollbar.h"

// 进度条组件
#include "progressbar.h"

// 滑块条组件
#include "sliderbar.h"

// 定时器组件
#include "timer.h"

// ========== 组件类型ID定义 ==========
// 这些ID与krnln_dtType.cpp中的数据类型索引对应
#define UNIT_TYPE_WINDOW        0       // 窗口
#define UNIT_TYPE_MENU          1       // 菜单
#define UNIT_TYPE_FONT          2       // 字体
#define UNIT_TYPE_EDITBOX       3       // 编辑框
#define UNIT_TYPE_PICBOX        4       // 图片框
#define UNIT_TYPE_SHAPEBOX      5       // 外形框
#define UNIT_TYPE_DRAWPANEL     6       // 画板
#define UNIT_TYPE_GROUPBOX      7       // 分组框
#define UNIT_TYPE_LABEL         8       // 标签
#define UNIT_TYPE_BUTTON        9       // 按钮
#define UNIT_TYPE_CHECKBOX      10      // 选择框
#define UNIT_TYPE_RADIOBOX      11      // 单选框
#define UNIT_TYPE_COMBOBOX      12      // 组合框
#define UNIT_TYPE_LISTBOX       13      // 列表框
#define UNIT_TYPE_CHKLISTBOX    14      // 选择列表框
#define UNIT_TYPE_HSCROLLBAR    15      // 横向滚动条
#define UNIT_TYPE_VSCROLLBAR    16      // 纵向滚动条
#define UNIT_TYPE_PROCESSBAR    17      // 进度条
#define UNIT_TYPE_SLIDERBAR     18      // 滑块条
#define UNIT_TYPE_TAB           19      // 选择夹
#define UNIT_TYPE_ANIMATEBOX    20      // 影像框
#define UNIT_TYPE_DATEPICKER    21      // 日期框
#define UNIT_TYPE_MONTHCALENDAR 22      // 月历
#define UNIT_TYPE_DRIVERBOX     23      // 驱动器框
#define UNIT_TYPE_DIRBOX        24      // 目录框
#define UNIT_TYPE_FILEBOX       25      // 文件框
#define UNIT_TYPE_COLORPICKER   26      // 颜色选择器
#define UNIT_TYPE_HYPERLINKER   27      // 超级链接框
#define UNIT_TYPE_SPIN          28      // 调节器
#define UNIT_TYPE_COMMONDLG     29      // 通用对话框
#define UNIT_TYPE_TIMER         30      // 时钟
#define UNIT_TYPE_PRINTER       31      // 打印机

// 组件总数
#define UNIT_TYPE_COUNT         32

// ========== 组件注册信息 ==========
typedef struct _UNIT_REGISTER_INFO {
    LPCWSTR             szTypeName;     // 组件类型名称（中文）
    LPCWSTR             szEngName;      // 组件英文名称
    DWORD               dwFlags;        // 组件标志
    PFN_GET_INTERFACE   pfnGetInterface;// 获取接口函数
} UNIT_REGISTER_INFO, *PUNIT_REGISTER_INFO;

// 组件标志
#define UNIT_FLAG_WIN_UNIT          0x0001  // 窗口组件
#define UNIT_FLAG_CONTAINER         0x0002  // 容器组件
#define UNIT_FLAG_FUNCTION_ONLY     0x0004  // 仅功能组件（无可视外形）
#define UNIT_FLAG_NO_FOCUS          0x0008  // 不能接收焦点

#ifdef __cplusplus
extern "C" {
#endif

// ========== 组件注册系统 ==========

/**
 * 初始化控件系统
 */
void Controls_Initialize();

/**
 * 反初始化控件系统
 */
void Controls_Uninitialize();

/**
 * 获取组件注册信息
 * @param nTypeIndex 组件类型索引
 * @return 组件注册信息指针，失败返回NULL
 */
PUNIT_REGISTER_INFO Controls_GetRegisterInfo(INT nTypeIndex);

/**
 * 根据类型名称获取组件类型索引
 * @param pszTypeName 组件类型名称
 * @return 组件类型索引，失败返回-1
 */
INT Controls_GetTypeIndex(LPCWSTR pszTypeName);

/**
 * 创建组件
 * @param nTypeIndex 组件类型索引
 * @param pParams 创建参数
 * @return 组件句柄，失败返回NULL
 */
HUNIT Controls_CreateUnit(INT nTypeIndex, PUNIT_CREATE_PARAMS pParams);

/**
 * 销毁组件
 * @param nTypeIndex 组件类型索引
 * @param hUnit 组件句柄
 * @return 成功返回TRUE
 */
BOOL Controls_DestroyUnit(INT nTypeIndex, HUNIT hUnit);

/**
 * 获取组件接口
 * @param nTypeIndex 组件类型索引
 * @param nInterfaceNO 接口编号
 * @return 接口函数指针
 */
PFN_INTERFACE Controls_GetInterface(INT nTypeIndex, INT nInterfaceNO);

#ifdef __cplusplus
}
#endif

#endif // __KRNLN_CONTROLS_H__
