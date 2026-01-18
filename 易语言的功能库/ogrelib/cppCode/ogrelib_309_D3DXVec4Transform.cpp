#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x03, 0) (四维向量).变换向量, 命令说明: "将四维向量和变换矩阵相乘，得到一个四维向量作为结果返回"
// 参数<1>: 变换矩阵 MAKELONG(0x04, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXVec4Transform_309_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

