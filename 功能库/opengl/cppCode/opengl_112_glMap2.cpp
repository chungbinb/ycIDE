#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 设置曲面控制点, 命令说明: "为Bezier曲面定义控制点。定义了控制点后，才可以画曲面。成功返回有效的控制点数，失败返回0。必须启用“#曲面坐标”才有效，不使用的时候需要关闭。始终保持这样的曲面，直到再次使用本命令改变控制点。\r\n\t本命令使用参数方程来定义曲线上的每个顶点。\r\n\t\tX = f （U ，V）\r\n\t\tY = f （U ，V）\r\n\t\tZ = f （U ，V）\r\n\t其中X、Y、Z分别代表曲线上的点的横向位置、竖向位置和纵向位置，U、V代表参数方程f（）的参数\r\n当参数 U 、V发生变化的时候，X、Y、Z也相应的改变。"
// 参数<1>: U参数上限 SDT_DOUBLE, 参数说明: "U参数的起始值。"
// 参数<2>: U参数下限 SDT_DOUBLE, 参数说明: "U参数的终止值。"
// 参数<3>: U向控制点数 SDT_INT, 参数说明: "U向控制点的数目,必须为非零整数。负数自动取正。"
// 参数<4>: V参数上限 SDT_DOUBLE, 参数说明: "V参数的起始值。"
// 参数<5>: V参数下限 SDT_DOUBLE, 参数说明: "V参数的终止值。"
// 参数<6>: 控制点 数组 SDT_DOUBLE, 参数说明: "用来描述控制点的数组，按照U1V1（X、Y、Z），U2V1（X、Y、Z）...，U1V2（X、Y、Z），U2V2（X、Y、Z）...的顺序排列。"
OPENGL_EXTERN_C void opengl_glMap2_112_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    INT      arg3 = pArgInf[2].m_int;
    double   arg4 = pArgInf[3].m_double;
    double   arg5 = pArgInf[4].m_double;
    void*    arg6 = pArgInf[5].m_pAryData;

}

