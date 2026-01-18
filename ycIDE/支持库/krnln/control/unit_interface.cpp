/*******************************************************************************
 * 文件名: unit_interface.cpp
 * 描述: 控件接口层实现 - 实现未在其他文件中定义的接口
 * 作者: ycIDE
 * 创建时间: 2026-01-07
 * 修改时间: 2026-01-18
 * 
 * 说明：
 * Window_GetInterface 已在 win_unit.cpp 中实现
 * Button_GetInterface 已在 button.cpp 中实现
 * EditBox_GetInterface 已在 editbox.cpp 中实现
 * 本文件只实现 Label_GetInterface 和 NullUnit_GetInterface
 ******************************************************************************/

#include "../include_krnln_header.h"
#include "unit_interface.h"
#include "controls.h"

//=============================================================================
// 标签组件接口 (占位实现)
//=============================================================================

PFN_INTERFACE WINAPI Label_GetInterface(INT nInterfaceNO)
{
    // 标签组件暂未实现，返回空
    return NULL;
}

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
