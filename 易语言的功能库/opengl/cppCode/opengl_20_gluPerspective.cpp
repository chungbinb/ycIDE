#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 创建对称透视投影, 命令说明: "在屏幕里面建立一个四棱台形状的透视投影的三维坐标空间。四棱台的前后两面平行于屏幕，其余四面呈喇叭口形状对称展开。近处物体大，远处物体小。"
// 参数<1>: 仰角 SDT_DOUBLE, 参数说明: "四棱台形状的三维坐标空间的上下两面张开的角度。角度越大，看到的范围越高，范围在0-180度之间。"
// 参数<2>: 宽度除以高度的比值 SDT_DOUBLE, 参数说明: "只有与实际的视口大小相匹配的时候，界面看起来才不会变形。"
// 参数<3>: 最近距离 SDT_DOUBLE, 参数说明: "四棱台形状的三维坐标空间距离屏幕最近的面，必须为正数。"
// 参数<4>: 最远距离 SDT_DOUBLE, 参数说明: "四棱台形状的三维坐标空间距离屏幕最远的面，必须为正数。"
OPENGL_EXTERN_C void opengl_gluPerspective_20_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    double   arg3 = pArgInf[2].m_double;
    double   arg4 = pArgInf[3].m_double;

}

