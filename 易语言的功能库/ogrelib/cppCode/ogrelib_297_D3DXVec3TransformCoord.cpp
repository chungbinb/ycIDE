#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x02, 0) (三维向量).变换坐标, 命令说明: "将三维向量格式的坐标值和变换矩阵相乘，得到变换后的坐标值"
// 参数<1>: 变换矩阵 MAKELONG(0x04, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXVec3TransformCoord_297_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

