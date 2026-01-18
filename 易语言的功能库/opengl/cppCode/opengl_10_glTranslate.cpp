#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 坐标平移, 命令说明: "平移当前坐标矩阵。在矩阵复位以前，将影响接下来的坐标操作。"
// 参数<1>: 横向位置 SDT_DOUBLE, 参数说明: "默认情况下水平向右为正数，向左为负数，如果坐标已经变换，那么在上一次坐标系的基础上继续叠加。"
// 参数<2>: 竖向位置 SDT_DOUBLE, 参数说明: "默认情况下竖直向上为正数，向下为负数，如果坐标已经变换，那么在上一次坐标系的基础上继续叠加。"
// 参数<3>: 纵向位置 SDT_DOUBLE, 参数说明: "默认情况下屏幕前方为正数，后方为负数，如果坐标已经变换，那么在上一次坐标系的基础上继续叠加。"
OPENGL_EXTERN_C void opengl_glTranslate_10_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    double   arg3 = pArgInf[2].m_double;

}

