#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建缩放矩阵, 命令说明: "创建一个缩放矩阵"
// 参数<1>: X轴缩放因子 SDT_FLOAT, 参数说明: "沿着X轴的缩放系数"
// 参数<2>: Y轴缩放因子 SDT_FLOAT, 参数说明: "沿着Y轴的缩放系数"
// 参数<3>: Z轴缩放因子 SDT_FLOAT, 参数说明: "沿着Z轴的缩放系数"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixScaling_327_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;
    FLOAT    arg2 = pArgInf[2].m_float;
    FLOAT    arg3 = pArgInf[3].m_float;

}

