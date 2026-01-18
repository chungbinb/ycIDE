#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (复数运算).置实数, 命令说明: "设置复数的实数部分"
// 参数<1>: 要设置的数据 SDT_DOUBLE, 参数说明: "要设置的数据，实数部分或虚数部分"
ECALC_EXTERN_C void ecalc_SetRealComplex_11_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[1].m_double;

}

