#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x02, 0) (三维向量).求叉乘, 命令说明: "两个向量交叉乘，返回结果向量，结果向量垂直于两条叉乘的向量"
// 参数<1>: 参数向量 MAKELONG(0x02, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXVec3Cross_291_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

