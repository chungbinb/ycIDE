#if !defined(__E_STATIC_LIB)
#include "include_krnln_header.h"

// 系统核心支持库命令信息
// 自动生成于 2026-01-04 10:36:10

// 参数信息数组
static ARG_INFO g_argumentInfo_krnln_global_var[] = 
{
    // 1=参数名称, 2=参数详细说明, 3=指定图标索引, 4=图标数目
    // 5=参数类型, 6=参数默认值, 7=参数标志
    /*0000*/ {"", NULL, 0, 0, _SDT_ALL, 0, NULL},
};

#ifdef _DEBUG
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_krnln_global_var) / sizeof(g_argumentInfo_krnln_global_var[0]);
#endif

#define KRNLN_DEF_CMDINFO(_index, _szName, _szEgName, _szFuncId, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },

// 命令信息数组
CMD_INFO g_cmdInfo_krnln_global_var[] = 
{
    KRNLN_DEF(KRNLN_DEF_CMDINFO)
};

int g_cmdInfo_krnln_global_var_count = sizeof(g_cmdInfo_krnln_global_var) / sizeof(g_cmdInfo_krnln_global_var[0]);

#endif
