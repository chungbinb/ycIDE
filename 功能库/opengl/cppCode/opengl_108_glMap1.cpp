#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 设置曲线控制点, 命令说明: "为Bezier曲线定义控制点。定义了控制点后，才可以画曲线。成功返回有效的控制点数，失败返回0。始终保持这样的曲线，直到再次使用本命令改变控制点。\r\n\t本命令使用参数方程来定义曲线上的每个顶点。\r\n\t\tX = f （U ）\r\n\t\tY = f （U ）\r\n\t\tZ = f （U ）\r\n\t其中X、Y、Z分别代表曲线上的点的横向位置、竖向位置和纵向位置，U代表参数方程f（）的参数\r\n当参数 U 发生变化的时候，X、Y、Z也相应的改变。"
// 参数<1>: U参数上限 SDT_DOUBLE, 参数说明: "U参数的起始值。"
// 参数<2>: U参数下限 SDT_DOUBLE, 参数说明: "U参数的终止值。"
// 参数<3>: 控制点 数组 SDT_DOUBLE, 参数说明: "用来描述控制点的数组，按照U1（X、Y、Z），U2（X、Y、Z），U3（X、Y、Z）...的顺序排列。"
OPENGL_EXTERN_C void opengl_glMap1_108_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    void*    arg3 = pArgInf[2].m_pAryData;

}

