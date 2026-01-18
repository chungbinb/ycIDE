#include "..\include_ogrelib_header.h"

// 调用格式: SDT_FLOAT (平面).求点乘四维向量, 命令说明: "计算平面方程与四维向量的点乘。如果平面方程系数是(a, b, c, d)和四维向量(x, y, z, w)，那么返回值就按下面的公式计算： a*x + b*y + c*z + d*w"
// 参数<1>: 参数向量 MAKELONG(0x03, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXPlaneDot_332_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

