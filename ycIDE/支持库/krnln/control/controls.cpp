/**
 * @file controls.cpp
 * @brief 窗口组件注册系统实现
 * @date 2026-01-18
 */

#include "../include_krnln_header.h"
#include "controls.h"
#include <string.h>
#include <commctrl.h>

#pragma comment(lib, "comctl32.lib")

// ========== 组件注册表 ==========
static UNIT_REGISTER_INFO g_UnitRegisterTable[] = 
{
    // 0: 窗口
    { L"窗口", L"window", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_CONTAINER, Window_GetInterface },
    
    // 1: 菜单 (暂未实现)
    { L"菜单", L"menu", 0, NULL },
    
    // 2: 字体 (非窗口组件)
    { L"字体", L"font", 0, NULL },
    
    // 3: 编辑框
    { L"编辑框", L"EditBox", UNIT_FLAG_WIN_UNIT, EditBox_GetInterface },
    
    // 4: 图片框 (暂未实现)
    { L"图片框", L"PicBox", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 5: 外形框 (暂未实现)
    { L"外形框", L"ShapeBox", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_NO_FOCUS, NULL },
    
    // 6: 画板 (暂未实现)
    { L"画板", L"DrawPanel", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 7: 分组框
    { L"分组框", L"GroupBox", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_CONTAINER | UNIT_FLAG_NO_FOCUS, GroupBox_GetInterface },
    
    // 8: 标签
    { L"标签", L"Label", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_NO_FOCUS, Label_GetInterface },
    
    // 9: 按钮
    { L"按钮", L"Button", UNIT_FLAG_WIN_UNIT, Button_GetInterface },
    
    // 10: 选择框
    { L"选择框", L"CheckBox", UNIT_FLAG_WIN_UNIT, CheckBox_GetInterface },
    
    // 11: 单选框
    { L"单选框", L"RadioBox", UNIT_FLAG_WIN_UNIT, RadioBox_GetInterface },
    
    // 12: 组合框
    { L"组合框", L"ComboBox", UNIT_FLAG_WIN_UNIT, ComboBox_GetInterface },
    
    // 13: 列表框
    { L"列表框", L"ListBox", UNIT_FLAG_WIN_UNIT, ListBox_GetInterface },
    
    // 14: 选择列表框 (暂未实现)
    { L"选择列表框", L"ChkListBox", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 15: 横向滚动条
    { L"横向滚动条", L"HScrollBar", UNIT_FLAG_WIN_UNIT, HScrollBar_GetInterface },
    
    // 16: 纵向滚动条
    { L"纵向滚动条", L"VScrollBar", UNIT_FLAG_WIN_UNIT, VScrollBar_GetInterface },
    
    // 17: 进度条
    { L"进度条", L"ProcessBar", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_NO_FOCUS, ProgressBar_GetInterface },
    
    // 18: 滑块条
    { L"滑块条", L"SliderBar", UNIT_FLAG_WIN_UNIT, SliderBar_GetInterface },
    
    // 19: 选择夹 (暂未实现)
    { L"选择夹", L"Tab", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_CONTAINER, NULL },
    
    // 20: 影像框 (暂未实现)
    { L"影像框", L"AnimateBox", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_NO_FOCUS, NULL },
    
    // 21: 日期框 (暂未实现)
    { L"日期框", L"DatePicker", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 22: 月历 (暂未实现)
    { L"月历", L"MonthCalendar", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 23: 驱动器框 (暂未实现)
    { L"驱动器框", L"DriverBox", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 24: 目录框 (暂未实现)
    { L"目录框", L"DirBox", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 25: 文件框 (暂未实现)
    { L"文件框", L"FileBox", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 26: 颜色选择器 (暂未实现)
    { L"颜色选择器", L"ColorPicker", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 27: 超级链接框 (暂未实现)
    { L"超级链接框", L"HyperLinker", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_NO_FOCUS, NULL },
    
    // 28: 调节器 (暂未实现)
    { L"调节器", L"Spin", UNIT_FLAG_WIN_UNIT, NULL },
    
    // 29: 通用对话框 (暂未实现)
    { L"通用对话框", L"CommonDlg", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_FUNCTION_ONLY, NULL },
    
    // 30: 时钟
    { L"时钟", L"Timer", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_FUNCTION_ONLY, Timer_GetInterface },
    
    // 31: 打印机 (暂未实现)
    { L"打印机", L"printer", UNIT_FLAG_WIN_UNIT | UNIT_FLAG_FUNCTION_ONLY, NULL },
};

static BOOL g_bInitialized = FALSE;

// ========== 实现 ==========

void Controls_Initialize()
{
    if (g_bInitialized) return;
    
    // 初始化公共控件库
    INITCOMMONCONTROLSEX icc = {0};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES | ICC_DATE_CLASSES | ICC_USEREX_CLASSES;
    InitCommonControlsEx(&icc);
    
    g_bInitialized = TRUE;
}

void Controls_Uninitialize()
{
    g_bInitialized = FALSE;
}

PUNIT_REGISTER_INFO Controls_GetRegisterInfo(INT nTypeIndex)
{
    if (nTypeIndex < 0 || nTypeIndex >= UNIT_TYPE_COUNT)
        return NULL;
    
    return &g_UnitRegisterTable[nTypeIndex];
}

INT Controls_GetTypeIndex(LPCWSTR pszTypeName)
{
    if (!pszTypeName) return -1;
    
    for (INT i = 0; i < UNIT_TYPE_COUNT; i++)
    {
        if (g_UnitRegisterTable[i].szTypeName &&
            wcscmp(g_UnitRegisterTable[i].szTypeName, pszTypeName) == 0)
        {
            return i;
        }
        // 也检查英文名
        if (g_UnitRegisterTable[i].szEngName &&
            _wcsicmp(g_UnitRegisterTable[i].szEngName, pszTypeName) == 0)
        {
            return i;
        }
    }
    
    return -1;
}

HUNIT Controls_CreateUnit(INT nTypeIndex, PUNIT_CREATE_PARAMS pParams)
{
    PUNIT_REGISTER_INFO pInfo = Controls_GetRegisterInfo(nTypeIndex);
    if (!pInfo || !pInfo->pfnGetInterface)
        return NULL;
    
    PFN_CREATE_UNIT pfnCreate = (PFN_CREATE_UNIT)pInfo->pfnGetInterface(ITF_CREATE_UNIT);
    if (!pfnCreate)
        return NULL;
    
    return pfnCreate(
        pParams->pAllData,
        pParams->nAllDataSize,
        pParams->dwStyle,
        pParams->hParentWnd,
        pParams->uID,
        pParams->hMenu,
        pParams->x,
        pParams->y,
        pParams->cx,
        pParams->cy,
        pParams->dwFormID,
        pParams->dwUnitID,
        pParams->hDesignWnd,
        pParams->blInDesignMode
    );
}

BOOL Controls_DestroyUnit(INT nTypeIndex, HUNIT hUnit)
{
    if (!hUnit) return FALSE;
    
    // 对于窗口组件，直接调用对应的销毁函数
    switch (nTypeIndex)
    {
    case UNIT_TYPE_WINDOW:     return Window_Destroy(hUnit);
    case UNIT_TYPE_EDITBOX:    return EditBox_Destroy(hUnit);
    case UNIT_TYPE_BUTTON:     return Button_Destroy(hUnit);
    case UNIT_TYPE_LABEL:      return Label_Destroy(hUnit);
    case UNIT_TYPE_CHECKBOX:   return CheckBox_Destroy(hUnit);
    case UNIT_TYPE_RADIOBOX:   return RadioBox_Destroy(hUnit);
    case UNIT_TYPE_GROUPBOX:   return GroupBox_Destroy(hUnit);
    case UNIT_TYPE_LISTBOX:    return ListBox_Destroy(hUnit);
    case UNIT_TYPE_COMBOBOX:   return ComboBox_Destroy(hUnit);
    case UNIT_TYPE_HSCROLLBAR: return ScrollBar_Destroy(hUnit);
    case UNIT_TYPE_VSCROLLBAR: return ScrollBar_Destroy(hUnit);
    case UNIT_TYPE_PROCESSBAR: return ProgressBar_Destroy(hUnit);
    case UNIT_TYPE_SLIDERBAR:  return SliderBar_Destroy(hUnit);
    case UNIT_TYPE_TIMER:      return Timer_Destroy(hUnit);
    default:
        return FALSE;
    }
}

PFN_INTERFACE Controls_GetInterface(INT nTypeIndex, INT nInterfaceNO)
{
    PUNIT_REGISTER_INFO pInfo = Controls_GetRegisterInfo(nTypeIndex);
    if (!pInfo || !pInfo->pfnGetInterface)
        return NULL;
    
    return pInfo->pfnGetInterface(nInterfaceNO);
}
