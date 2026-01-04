#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 画矩形, 命令说明: "在当前坐标原点指定的深度，使用先前选定的颜色画出一个填充颜色的矩形。"
// 参数<1>: 左上角横坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<2>: 左上角纵坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<3>: 右下角横坐标 SDT_DOUBLE, 参数说明: NULL
// 参数<4>: 右下角纵坐标 SDT_DOUBLE, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glRect_24_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    double   arg3 = pArgInf[2].m_double;
    double   arg4 = pArgInf[3].m_double;

}

