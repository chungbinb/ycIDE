#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 创建正投影, 命令说明: "在屏幕里面建立一个长方体形状的正投影的三维坐标空间。长方体的前后两面平行于屏幕，左右两面垂直屏幕并竖直，上下两面垂直屏幕并水平。近处物体和远处物体一样的大。"
// 参数<1>: 最左端 SDT_DOUBLE, 参数说明: "长方体形状的三维坐标空间的最左端"
// 参数<2>: 最右端 SDT_DOUBLE, 参数说明: "长方体形状的三维坐标空间的最左端"
// 参数<3>: 最底端 SDT_DOUBLE, 参数说明: "长方体形状的三维坐标空间的最底端"
// 参数<4>: 最顶端 SDT_DOUBLE, 参数说明: "长方体形状的三维坐标空间的最顶端"
// 参数<5>: 最近端 SDT_DOUBLE, 参数说明: "长方体形状的三维坐标空间的最近端"
// 参数<6>: 最远端 SDT_DOUBLE, 参数说明: "长方体形状的三维坐标空间的最远端"
OPENGL_EXTERN_C void opengl_glOrtho_14_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    double   arg3 = pArgInf[2].m_double;
    double   arg4 = pArgInf[3].m_double;
    double   arg5 = pArgInf[4].m_double;
    double   arg6 = pArgInf[5].m_double;

}

