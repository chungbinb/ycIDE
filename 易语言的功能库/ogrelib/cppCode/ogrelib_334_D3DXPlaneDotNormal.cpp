#include "..\include_ogrelib_header.h"

// 调用格式: SDT_FLOAT (平面).求正规点乘三维向量, 命令说明: "计算平面与3-D向量的点积。并且平面系数里的W为0。如果平面方程系数是(a, b, c, d)，三维向量是 (x, y, z)，那么返回的结果就是用下面公式计算： a*x + b*y + c*z + d*0"
// 参数<1>: 参数向量 MAKELONG(0x02, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXPlaneDotNormal_334_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

