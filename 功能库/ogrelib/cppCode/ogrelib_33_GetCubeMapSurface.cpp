#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立方体纹理).取立方体面页面, 命令说明: "获取立方体面所对应的页面。"
// 参数<1>: 哪一面 MAKELONG(0x3B, 0), 参数说明: "本参数表示要取立方体哪一面。"
// 参数<2>: 级别 SDT_INT, 参数说明: "要取的级别。"
// 参数<3>: &页面 MAKELONG(0x08, 0), 参数说明: "存放的页面。"
OGRELIB_EXTERN_C void ogrelib_GetCubeMapSurface_33_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;
    void**   arg3 = pArgInf[3].m_ppCompoundData;

}

