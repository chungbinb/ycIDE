#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建正交投影矩阵, 命令说明: "创建一个左手坐标系的正交投影矩阵"
// 参数<1>: 观察宽度 SDT_FLOAT, 参数说明: "观察时的宽度"
// 参数<2>: 观察高度 SDT_FLOAT, 参数说明: "观察时的高度"
// 参数<3>: Z轴近点 SDT_FLOAT, 参数说明: "观察时Z轴最近点"
// 参数<4>: Z轴远点 SDT_FLOAT, 参数说明: "观察时Z轴最远点"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixOrthoLH_317_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;
    FLOAT    arg2 = pArgInf[2].m_float;
    FLOAT    arg3 = pArgInf[3].m_float;
    FLOAT    arg4 = pArgInf[4].m_float;

}

