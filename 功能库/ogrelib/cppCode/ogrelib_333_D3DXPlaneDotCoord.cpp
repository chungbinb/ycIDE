#include "..\include_ogrelib_header.h"

// 调用格式: SDT_FLOAT (平面).求点乘三维向量, 命令说明: "计算平面跟三维向量的点积，并且平面系数里的W为1如果平面方程系数是 (a, b, c, d)，并且三维向量是 (x, y, z)，那么返回值就用下面的公式计算： a*x + b*y + c*z + d*1"
// 参数<1>: 参数向量 MAKELONG(0x02, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXPlaneDotCoord_333_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

