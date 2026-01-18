#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 创建实心圆环, 命令说明: "创建一个中心在坐标原点的实心圆环。"
// 参数<1>: 内圆半径 SDT_DOUBLE, 参数说明: "负数自动被转化为正数，如果等于0，将由1来取代。如果内圆半径大于外圆半径,就交换内圆半径和外圆半径的值。如果内圆半径和外圆半径的值相等，外圆半径自动加1。"
// 参数<2>: 外圆半径 SDT_DOUBLE, 参数说明: "负数自动被转化为正数，如果等于0，将由1来取代。如果内圆半径大于外圆半径,就交换内圆半径和外圆半径的值。如果内圆半径和外圆半径的值相等，外圆半径自动加1。"
// 参数<3>: 边数 SDT_INT, 参数说明: "负数自动被转化为正数，如果小于3，将由3来取代。数值越大越逼真，占用的系统资源也越大。"
// 参数<4>: 环数 SDT_INT, 参数说明: "负数自动被转化为正数，如果小于3，将由3来取代。数值越大越逼真，占用的系统资源也越大。"
OPENGL_EXTERN_C void opengl_glutSolidTorus_89_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    double   arg2 = pArgInf[1].m_double;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;

}

