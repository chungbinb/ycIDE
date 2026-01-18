#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).渲染索引图元, 命令说明: "渲染指定的索引顶点序列。"
// 参数<1>: 灵活的向量格式 SDT_INT, 参数说明: "绘制的图元（即图形的基本单位）类型。为以下常量值之一：1、#设备常量.点列；2、#设备常量.线列；3、#设备常量.线带；4、#设备常量.三角形列；5、#设备常量.三角形带；6、#设备常量.三角扇形。"
// 参数<2>: 索引缓冲偏移量 SDT_INT, 参数说明: "索引缓冲区的偏移量。"
// 参数<3>: 最小顶点索引值 SDT_INT, 参数说明: "要绘制的最小的顶点索引值。"
// 参数<4>: 顶点个数 SDT_INT, 参数说明: "要绘制的顶点个数。从（索引缓冲偏移量 + 最小顶点索引值）处开始计算。"
// 参数<5>: 索引起始位置 SDT_INT, 参数说明: "在索引数组中读取索引的起始位置。"
// 参数<6>: 图元个数 SDT_INT, 参数说明: "要绘制的图元的个数。"
OGRELIB_EXTERN_C void ogrelib_DrawIndexedPrimitive_154_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;
    INT      arg5 = pArgInf[5].m_int;
    INT      arg6 = pArgInf[6].m_int;

}

