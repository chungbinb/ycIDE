#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置渲染状态, 命令说明: "设置设备的渲染状态。"
// 参数<1>: 渲染状态 MAKELONG(0x2B, 0), 参数说明: "渲染状态。为渲染状态常量中的任一常量。"
// 参数<2>: 渲染状态值 SDT_INT, 参数说明: "指定设备渲染状态值。"
OGRELIB_EXTERN_C void ogrelib_SetRenderState_157_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;

}

