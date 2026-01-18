#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).渲染图元, 命令说明: "渲染指定的顶点序列。"
// 参数<1>: 图元类型 SDT_INT, 参数说明: "绘制的图元（即图形的基本单位）类型。为以下常量值之一：1、#设备常量.点列；2、#设备常量.线列；3、#设备常量.线带；4、#设备常量.三角形列；5、#设备常量.三角形带；6、#设备常量.三角扇形。"
// 参数<2>: 第一个顶点序号 SDT_INT, 参数说明: "三角形顶点列表中第一个将被绘制的顶点的序号。不能为负数。"
// 参数<3>: 图元个数 SDT_INT, 参数说明: "要绘制的图元的个数。"
OGRELIB_EXTERN_C void ogrelib_DrawPrimitive_153_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

