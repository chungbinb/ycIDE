#include "..\include_ogrelib_header.h"

// 调用格式: SDT_INT (设备).取渲染状态, 命令说明: "取指定的渲染状态。成功返回状态值，失败返回-1。"
// 参数<1>: 渲染状态 MAKELONG(0x2B, 0), 参数说明: "渲染状态。为渲染状态常量中的任一常量。"
OGRELIB_EXTERN_C void ogrelib_GetRenderState_181_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

