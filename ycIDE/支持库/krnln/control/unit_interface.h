/*******************************************************************************
 * 文件名: unit_interface.h
 * 描述: 控件接口层 - 连接控件实现与库数据类型系统
 * 作者: ycIDE
 * 创建时间: 2026-01-07
 ******************************************************************************/

#ifndef _UNIT_INTERFACE_H_
#define _UNIT_INTERFACE_H_

#include "../elib/lib2.h"
#include "controls.h"

#ifdef __cplusplus
extern "C" {
#endif

// 为数据类型提供的接口函数
// 这些函数需要被设置到 LIB_DATA_TYPE_INFO 的 m_pfnGetInterface 字段

// 窗口组件接口
PFN_INTERFACE WINAPI Window_GetInterface(INT nInterfaceNO);

// 按钮组件接口  
PFN_INTERFACE WINAPI Button_GetInterface(INT nInterfaceNO);

// 编辑框组件接口
PFN_INTERFACE WINAPI EditBox_GetInterface(INT nInterfaceNO);

// 标签组件接口
PFN_INTERFACE WINAPI Label_GetInterface(INT nInterfaceNO);

// 选择框组件接口
PFN_INTERFACE WINAPI CheckBox_GetInterface(INT nInterfaceNO);

// 单选框组件接口
PFN_INTERFACE WINAPI RadioBox_GetInterface(INT nInterfaceNO);

// 分组框组件接口
PFN_INTERFACE WINAPI GroupBox_GetInterface(INT nInterfaceNO);

// 列表框组件接口
PFN_INTERFACE WINAPI ListBox_GetInterface(INT nInterfaceNO);

// 组合框组件接口
PFN_INTERFACE WINAPI ComboBox_GetInterface(INT nInterfaceNO);

// 水平滚动条组件接口
PFN_INTERFACE WINAPI HScrollBar_GetInterface(INT nInterfaceNO);

// 垂直滚动条组件接口
PFN_INTERFACE WINAPI VScrollBar_GetInterface(INT nInterfaceNO);

// 进度条组件接口
PFN_INTERFACE WINAPI ProgressBar_GetInterface(INT nInterfaceNO);

// 滑块条组件接口
PFN_INTERFACE WINAPI SliderBar_GetInterface(INT nInterfaceNO);

// 定时器组件接口
PFN_INTERFACE WINAPI Timer_GetInterface(INT nInterfaceNO);

// 通用的空接口函数（用于尚未实现的组件）
PFN_INTERFACE WINAPI NullUnit_GetInterface(INT nInterfaceNO);

// 初始化所有控件接口
void UnitInterface_Initialize(void);

#ifdef __cplusplus
}
#endif

#endif // _UNIT_INTERFACE_H_
