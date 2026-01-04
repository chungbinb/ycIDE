#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建离屏页面, 命令说明: NULL
// 参数<1>: 宽度 SDT_INT, 参数说明: "页面宽度。"
// 参数<2>: 高度 SDT_INT, 参数说明: "页面高度。"
// 参数<3>: 页面格式 MAKELONG(0x35, 0), 参数说明: "为“页面格式”常量值之一。"
// 参数<4>: 内存池类型 MAKELONG(0x39, 0), 参数说明: "为“内存池类型”常量值之一。"
// 参数<5>: 页面 MAKELONG(0x08, 0), 参数说明: "要创建的页面。"
OGRELIB_EXTERN_C void ogrelib_CreateOffscreenPlainSurface_199_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    void*    arg4 = pArgInf[4].m_pCompoundData;
    void*    arg5 = pArgInf[5].m_pCompoundData;

}

