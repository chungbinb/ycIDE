#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建纹理, 命令说明: "创建一个纹理资源。"
// 参数<1>: 宽度 SDT_INT, 参数说明: "顶层纹理的宽度（以像素为单位）。"
// 参数<2>: 高度 SDT_INT, 参数说明: "顶层纹理的高度（以像素为单位）。"
// 参数<3>: 级数 SDT_INT, 参数说明: "纹理的级数。"
// 参数<4>: 资源用途 MAKELONG(0x37, 0), 参数说明: "为“资源用途”常量值之一。"
// 参数<5>: 页面格式 MAKELONG(0x35, 0), 参数说明: "为“页面格式”常量值之一。"
// 参数<6>: 内存池类型 MAKELONG(0x39, 0), 参数说明: "为“内存池类型”常量值之一。"
// 参数<7>: 纹理 MAKELONG(0x0F, 0), 参数说明: "要创建的纹理资源。"
OGRELIB_EXTERN_C void ogrelib_CreateTexture_206_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;
    void*    arg4 = pArgInf[4].m_pCompoundData;
    void*    arg5 = pArgInf[5].m_pCompoundData;
    void*    arg6 = pArgInf[6].m_pCompoundData;
    void*    arg7 = pArgInf[7].m_pCompoundData;

}

