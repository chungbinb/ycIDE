#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置观察坐标, 命令说明: "新的视图空间以坐标中心位置为坐标原点，坐标中心位置到眼睛的矢量为纵向坐标轴的正方向，以正上方矢量为竖向坐标轴的正方向。"
// 参数<1>: 眼睛横坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<2>: 眼睛竖坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<3>: 眼睛纵坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<4>: 中心横坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<5>: 中心竖坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<6>: 中心纵坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<7>: 正上方横坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<8>: 正上方竖坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<9>: 正上方纵坐标 SDT_DOUBLE, 参数说明: NULL
OPENGL_EXTERN_C void opengl_gluLookAt_102_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    double   arg3 = pArgInf[2].m_double;
    double   arg4 = pArgInf[3].m_double;
    double   arg5 = pArgInf[4].m_double;
    double   arg6 = pArgInf[5].m_double;
    double   arg7 = pArgInf[6].m_double;
    double   arg8 = pArgInf[7].m_double;
    double   arg9 = pArgInf[8].m_double;

}

