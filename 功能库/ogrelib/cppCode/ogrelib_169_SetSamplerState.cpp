#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置取样器状态, 命令说明: NULL
// 参数<1>: 取样器索引 SDT_INT, 参数说明: NULL
// 参数<2>: 取样器状态类型 SDT_INT, 参数说明: "为“取样器状态类型”常量中的任一值。"
// 参数<3>: 状态值 SDT_INT, 参数说明: "要设置的取样器状态值。该状态值的意义由取样器状态类型决定。"
OGRELIB_EXTERN_C void ogrelib_SetSamplerState_169_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

