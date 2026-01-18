#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x04, 0) (变换矩阵).建平移矩阵, 命令说明: "创建一个坐标平移矩阵"
// 参数<1>: X轴偏移 SDT_FLOAT, 参数说明: "相对于原点沿着X轴的偏移"
// 参数<2>: Y轴偏移 SDT_FLOAT, 参数说明: "相对于原点沿着Y轴的偏移"
// 参数<3>: Z轴偏移 SDT_FLOAT, 参数说明: "相对于原点沿着Z轴的偏移"
OGRELIB_EXTERN_C void ogrelib_D3DXMatrixTranslation_328_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[1].m_float;
    FLOAT    arg2 = pArgInf[2].m_float;
    FLOAT    arg3 = pArgInf[3].m_float;

}

