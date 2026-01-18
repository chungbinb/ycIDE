#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 标记边界线, 命令说明: "在“设置点”之前使用，指定多边形的该点或者线是否是边界线的一部分。只有当多边形的模式为点或者线的时候才有意义。"
// 参数<1>: 是否为边界线 SDT_BOOL, 参数说明: "真表示属于多边形，需要显示，假表示不显示。"
OPENGL_EXTERN_C void opengl_glEdgeFlag_30_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL     arg1 = pArgInf[0].m_bool;

}

