#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 清除深度缓存, 命令说明: "设置深度缓存的默认值。"
// 参数<1>: 深度 SDT_DOUBLE, 参数说明: "范围在[0，1]内的数值。"
OPENGL_EXTERN_C void opengl_glClearDepth_45_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;

}

