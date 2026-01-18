#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置光栅位置, 命令说明: "设置下一次对光栅操作的坐标位置，会受到坐标变换的影响。如果要改变像素的颜色必须在本命令之前使用。"
// 参数<1>: 横向位置 SDT_DOUBLE, 参数说明: "光栅原点到当前矩阵的坐标系原点的横向距离，单位与坐标系相同。正数时光栅原点在坐标原点右边，反之在左边，0表示相同。"
// 参数<2>: 竖向位置 SDT_DOUBLE, 参数说明: "光栅原点到当前矩阵的坐标系原点的竖向距离，单位与坐标系相同。正数时光栅原点在坐标原点上边，反之在下边，0表示相同。"
OPENGL_EXTERN_C void opengl_glRasterPos_76_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;

}

