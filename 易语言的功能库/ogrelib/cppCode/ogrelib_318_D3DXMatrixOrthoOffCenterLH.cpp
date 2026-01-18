#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).自定义正交投影矩阵, 命令说明: "创建一个用户定制的左手坐标系的正交投影矩阵"
// 参数<1>: X轴最小值 SDT_FLOAT, 参数说明: "观察时X轴最小值"
// 参数<2>: X轴最大值 SDT_FLOAT, 参数说明: "观察时X轴最大值"
// 参数<3>: Y轴最小值 SDT_FLOAT, 参数说明: "观察时Y轴最小值"
// 参数<4>: Y轴最大值 SDT_FLOAT, 参数说明: "观察时Y轴最大值"
// 参数<5>: Z轴最小值 SDT_FLOAT, 参数说明: "观察时Z轴最小值"
// 参数<6>: Z轴最大值 SDT_FLOAT, 参数说明: "观察时Z轴最大值"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixOrthoOffCenterLH_318_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;
    FLOAT    arg2 = pArgInf[2].m_float;
    FLOAT    arg3 = pArgInf[3].m_float;
    FLOAT    arg4 = pArgInf[4].m_float;
    FLOAT    arg5 = pArgInf[5].m_float;
    FLOAT    arg6 = pArgInf[6].m_float;

}

