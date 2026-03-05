/*******************************************************************************
 * 文件名: unit_interface.cpp
 * 描述: 控件接口层实现
 * 作者: ycIDE
 * 创建时间: 2026-01-07
 * 修改时间: 2026-03-05
 * 
 * 说明：
 * Window_GetInterface    已在 win_unit.cpp    中实现
 * Button_GetInterface    已在 button.cpp      中实现
 * EditBox_GetInterface   已在 editbox.cpp     中实现
 * Label_GetInterface     已在 label.cpp       中实现
 * CheckBox_GetInterface  已在 checkbox.cpp    中实现
 * RadioBox_GetInterface  已在 radiobox.cpp    中实现
 * GroupBox_GetInterface  已在 groupbox.cpp    中实现
 * ListBox_GetInterface   已在 listbox.cpp     中实现
 * ComboBox_GetInterface  已在 combobox.cpp    中实现
 * HScrollBar_GetInterface已在 scrollbar.cpp   中实现
 * VScrollBar_GetInterface已在 scrollbar.cpp   中实现
 * ProgressBar_GetInterface已在progressbar.cpp 中实现
 * SliderBar_GetInterface 已在 sliderbar.cpp   中实现
 * Timer_GetInterface     已在 timer.cpp       中实现
 * 本文件只实现 NullUnit_GetInterface
 ******************************************************************************/

#include "../include_krnln_header.h"
#include "unit_interface.h"
#include "controls.h"

//=============================================================================
// 空接口函数（用于尚未实现的组件）
//=============================================================================

PFN_INTERFACE WINAPI NullUnit_GetInterface(INT nInterfaceNO)
{
    // 用于尚未实现的组件，返回空接口
    return NULL;
}

//=============================================================================
// 初始化
//=============================================================================

void UnitInterface_Initialize(void)
{
    // 初始化控件系统
    Controls_Initialize();
}
