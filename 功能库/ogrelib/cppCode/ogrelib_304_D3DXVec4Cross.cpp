#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x03, 0) (四维向量).求叉乘, 命令说明: "四维向量交叉乘，返回结果向量"
// 参数<1>: 参数向量 MAKELONG(0x03, 0), 参数说明: NULL
// 参数<2>: 参数向量 MAKELONG(0x03, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXVec4Cross_304_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

