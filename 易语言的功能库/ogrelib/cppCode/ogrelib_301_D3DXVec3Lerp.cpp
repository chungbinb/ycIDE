#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x02, 0) (三维向量).线形内插值, 命令说明: "操作两个三维向量的线性内插值，返回结果向量。向量线形插值的公式为“V1 + s(V2-V1)”"
// 参数<1>: 参数向量 MAKELONG(0x02, 0), 参数说明: NULL
// 参数<2>: 插值参数 SDT_FLOAT, 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXVec3Lerp_301_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    FLOAT    arg2 = pArgInf[2].m_float;

}

