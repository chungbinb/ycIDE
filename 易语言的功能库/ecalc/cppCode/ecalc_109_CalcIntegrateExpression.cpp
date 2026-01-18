#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (算式解析).求积分表达式, 命令说明: "本方法可求各种复杂的积分表达式，可含任何数学函数，如  3*exp(x)*tan(x)/(8+log(x))，并且可以检查出表达式的错误，如果积分表达式有误或计算失败，本方法将返回假，计算成功本方法返回真"
// 参数<1>: 计算表达式 SDT_TEXT, 参数说明: "该参数提供表示式文本，支持各种复杂的积分表达式，可含任何数学函数，如  3*exp(x)*tan(x)/(8+log(x))"
// 参数<2>: 积分上限 SDT_DOUBLE, 参数说明: "积分的上限"
// 参数<3>: 积分下限 SDT_DOUBLE, 参数说明: "积分的下限"
// 参数<4>: 误差 SDT_DOUBLE, 参数说明: "允许的误差"
// 参数<5>: &计算结果 SDT_DOUBLE, 参数说明: "计算的结果"
ECALC_EXTERN_C void ecalc_CalcIntegrateExpression_109_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    double   arg2 = pArgInf[2].m_double;
    double   arg3 = pArgInf[3].m_double;
    double   arg4 = pArgInf[4].m_double;
    double*  arg5 = pArgInf[5].m_pDouble;

}

