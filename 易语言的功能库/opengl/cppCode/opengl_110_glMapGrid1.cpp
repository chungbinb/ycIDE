#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置曲线等分点, 命令说明: "先使用“设置曲线控制点”命令生成曲线，然后设置曲线的等分点数，让曲线光滑的过度。\r\n\t最终得到的每两个相邻U参数的间隔为：（U向等分下限 - U向等分上限）/ U向等分数。"
// 参数<1>: U向等分数 SDT_INT, 参数说明: "等分数目越多，曲线越光滑，占用的系统资源也越大。"
// 参数<2>: U向等分上限 SDT_DOUBLE, 参数说明: "U参数的起始值。"
// 参数<3>: U向等分下限 SDT_DOUBLE, 参数说明: "U参数的终止值。"
OPENGL_EXTERN_C void opengl_glMapGrid1_110_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    double   arg2 = pArgInf[1].m_double;
    double   arg3 = pArgInf[2].m_double;

}

