#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x02, 0) (三维向量).到屏幕空间, 命令说明: "把向量从对象空间投影到屏幕空间"
// 参数<1>: 视口 MAKELONG(0x07, 0), 参数说明: NULL
// 参数<2>: 投影矩阵 MAKELONG(0x04, 0), 参数说明: NULL
// 参数<3>: 视变换矩阵 MAKELONG(0x04, 0), 参数说明: NULL
// 参数<4>: 世界变换矩阵 MAKELONG(0x04, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXVec3Project_299_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    void*    arg4 = pArgInf[4].m_pCompoundData;

}

