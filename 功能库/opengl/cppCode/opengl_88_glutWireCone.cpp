#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 创建空心圆锥体, 命令说明: "创建一个中心在坐标原点的空心圆锥体。"
// 参数<1>: 底面半径 SDT_DOUBLE, 参数说明: "负数自动被转化为正数，如果等于0，将由1来取代。"
// 参数<2>: 圆锥体高度 SDT_DOUBLE, 参数说明: "负数自动被转化为正数，如果等于0，将由1来取代。"
// 参数<3>: 经线条数 SDT_INT, 参数说明: "负数自动被转化为正数，如果等于0，将由1来取代。数值越大越逼真，占用的系统资源也越大。"
// 参数<4>: 纬线条数 SDT_INT, 参数说明: "负数自动被转化为正数，如果等于0，将由1来取代。数值越大越逼真，占用的系统资源也越大。"
OPENGL_EXTERN_C void opengl_glutWireCone_88_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;

}

