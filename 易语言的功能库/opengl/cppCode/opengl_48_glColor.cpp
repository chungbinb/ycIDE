#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置颜色, 命令说明: "设置接下来的图元的颜色。能在“开始画”和“停止画”之间执行本语句。只有在红绿蓝混合通道模式下有效。"
// 参数<1>: 红色位 SDT_DOUBLE, 参数说明: "范围在[0，1]内的数值。"
// 参数<2>: 绿色位 SDT_DOUBLE, 参数说明: "范围在[0，1]内的数值。"
// 参数<3>: 蓝色位 SDT_DOUBLE, 参数说明: "范围在[0，1]内的数值。"
// 参数<4>: 通道位 SDT_DOUBLE, 参数说明: "范围在[0，1]内的数值。"
OPENGL_EXTERN_C void opengl_glColor_48_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    double   arg3 = pArgInf[2].m_double;
    double   arg4 = pArgInf[3].m_double;

}

