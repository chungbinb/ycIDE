#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 创建空心正方体, 命令说明: "创建一个中心在坐标原点的空心正方体。"
// 参数<1>: 棱长 SDT_DOUBLE, 参数说明: "负数自动被转化为正数，如果等于0，将由1来取代。"
OPENGL_EXTERN_C void opengl_glutWireCube_86_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;

}

