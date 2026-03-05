// 系统核心支持库头文件
// 自动生成于 2026-01-04 15:09:00
// 支持 Unicode 和 x64

#ifndef __INCLUDE_KRNLN_HEADER_H__
#define __INCLUDE_KRNLN_HEADER_H__

// 防止 Windows.h 定义 min/max 宏，与 std::min/std::max 冲突
#ifndef NOMINMAX
#define NOMINMAX
#endif

// 定义库名称宏，必须在包含 lib2.h 之前定义
#define __E_FNENAME krnln

#include "elib/lib2.h"
#include "elib/lang.h"
#include "elib/krnllib.h"
#include "elib/fnshare.h"
#include "krnln_cmd_typedef.h"

#define KRNLN_EXTERN_C EXTERN_C

// 控件基类结构
typedef struct CONTROL_STRUCT_BASE
{
    WNDPROC             oldProc;
    HFONT               hFont;
    HWND                hDesignWnd;
    HWND                hWndParent;
    HWND                hWnd;
    DWORD               hUnit;
    UINT                id;
    DWORD               dwWinFormID;
    DWORD               dwUnitID;
    BOOL                blInDesignMode;
    LONG                flags;
    DWORD               style;
    DWORD               styleEx;
    DWORD               styleCtl;
}*PCONTROL_STRUCT_BASE;

#ifndef __E_STATIC_LIB
extern LIB_CONST_INFO g_ConstInfo_krnln_global_var[];
extern int g_ConstInfo_krnln_global_var_count;
extern CMD_INFO g_cmdInfo_krnln_global_var[];
extern PFN_EXECUTE_CMD g_cmdInfo_krnln_global_var_fun[];
extern int g_cmdInfo_krnln_global_var_count;
extern ARG_INFO g_argumentInfo_krnln_global_var[];
extern LIB_DATA_TYPE_INFO g_DataType_krnln_global_var[];
extern int g_DataType_krnln_global_var_count;
#endif

// 命令函数声明宏 - _szFuncId用于生成函数名(避免C++关键字), _szEgName用于显示
#define KRNLN_DEF_CMD(_index, _szName, _szEgName, _szFuncId, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    EXTERN_C void KRNLN_NAME(_index, _szFuncId)(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
KRNLN_DEF(KRNLN_DEF_CMD)

#endif // __INCLUDE_KRNLN_HEADER_H__