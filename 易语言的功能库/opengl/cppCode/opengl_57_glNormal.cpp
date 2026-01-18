#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置法向量, 命令说明: "指定下一顶点的法向量。能在“开始画”和“停止画”之间执行本语句。启用“#单位法向量”后，法向量的长度就会变成1。"
// 参数<1>: 横向量 SDT_DOUBLE, 参数说明: NULL
// 参数<2>: 竖向量 SDT_DOUBLE, 参数说明: NULL
// 参数<3>: 纵向量 SDT_DOUBLE, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glNormal_57_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    double   arg3 = pArgInf[2].m_double;

}

