#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取渲染目标页面, 命令说明: NULL
// 参数<1>: 渲染目标序号 SDT_INT, 参数说明: NULL
// 参数<2>: 渲染目标页面 MAKELONG(0x08, 0), 参数说明: "要取得的渲染目标页面。"
OGRELIB_EXTERN_C void ogrelib_GetRenderTarget_182_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

