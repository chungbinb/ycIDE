#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (其他计算).求雅各比方程, 命令说明: "求解雅各比方程。由于雅各比方程中包含特殊符号，因此方程原型无法在此显示，对此表示抱歉"
// 参数<1>: n SDT_INT, 参数说明: "本参数为指定多项式的次数的整数，必须大于0"
// 参数<2>: a SDT_DOUBLE, 参数说明: "本参数为任一大于-1的数"
// 参数<3>: b SDT_DOUBLE, 参数说明: "本参数为任一大于-1的数"
// 参数<4>: x SDT_DOUBLE, 参数说明: "本参数为任一浮点参数"
// 参数<5>: &错误码 SDT_INT, 参数说明: "可为以下常量之一：0、#无错误；1、#参数必须是一个不为0的数字；2、#参数不能为负数；3、#多项式序号必须大于0；4、#参数必须大于-1"
ECALC_EXTERN_C void ecalc_Jacobi_60_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    double   arg2 = pArgInf[2].m_double;
    double   arg3 = pArgInf[3].m_double;
    double   arg4 = pArgInf[4].m_double;
    PINT     arg5 = pArgInf[5].m_pInt;

}

