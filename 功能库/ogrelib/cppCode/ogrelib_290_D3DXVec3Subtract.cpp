#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x02, 0) (三维向量).减去, 命令说明: "两个三维向量相减，返回结果向量"
// 参数<1>: 参数向量 MAKELONG(0x02, 0), 参数说明: NULL
// 参数<2>: 参数向量 MAKELONG(0x02, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXVec3Subtract_290_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

