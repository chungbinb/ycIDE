#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 生成法线贴图, 命令说明: "将高度贴图转换为法线贴图。"
// 参数<1>: &目标纹理 MAKELONG(0x0F, 0), 参数说明: "指定的目标纹理。"
// 参数<2>: 源纹理 MAKELONG(0x0F, 0), 参数说明: "源高度贴图纹理。"
// 参数<3>: &源调色板单元 MAKELONG(0x1F, 0), 参数说明: "源256色调色板。"
// 参数<4>: 标志 SDT_INT, 参数说明: "控制生成法线贴图的标志。"
// 参数<5>: 通道 SDT_INT, 参数说明: "指定源高度贴图通道信息。"
// 参数<6>: Amplitude SDT_FLOAT, 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXComputeNormalMap_248_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[0].m_ppCompoundData;
    void*    arg2 = pArgInf[1].m_pCompoundData;
    void**   arg3 = pArgInf[2].m_ppCompoundData;
    INT      arg4 = pArgInf[3].m_int;
    INT      arg5 = pArgInf[4].m_int;
    FLOAT    arg6 = pArgInf[5].m_float;

}

