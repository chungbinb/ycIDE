#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x02, 0) (三维向量).标准变换向量, 命令说明: "这个函数用矩阵变换向量标准形式 (x, y, z, 0)，不是齐次坐标形式"
// 参数<1>: 变换矩阵 MAKELONG(0x04, 0), 参数说明: "如果一个非仿射矩阵变换，那么这个矩阵要作逆运算后再转置才传给函数"
OGRELIB_EXTERN_C void ogrelib_D3DXVec3TransformNormal_298_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

