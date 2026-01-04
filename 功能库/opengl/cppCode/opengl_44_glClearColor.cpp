#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 清除颜色缓存, 命令说明: "设置颜色缓存的默认值。不执行本命令前参数全部为0。"
// 参数<1>: 红色位 SDT_FLOAT, 参数说明: "范围在[0，1]内的数值。"
// 参数<2>: 绿色位 SDT_FLOAT, 参数说明: "范围在[0，1]内的数值。"
// 参数<3>: 蓝色位 SDT_FLOAT, 参数说明: "范围在[0，1]内的数值。"
// 参数<4>: 通道位 SDT_FLOAT, 参数说明: "范围在[0，1]内的数值。"
OPENGL_EXTERN_C void opengl_glClearColor_44_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[0].m_float;
    FLOAT    arg2 = pArgInf[1].m_float;
    FLOAT    arg3 = pArgInf[2].m_float;
    FLOAT    arg4 = pArgInf[3].m_float;

}

