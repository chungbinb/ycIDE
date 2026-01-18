#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (其他计算).求不完全贝它, 命令说明: "根据指定参数求贝它"
// 参数<1>: a SDT_DOUBLE, 参数说明: "大于0的参数"
// 参数<2>: b SDT_DOUBLE, 参数说明: "大于0的参数"
// 参数<3>: x SDT_DOUBLE, 参数说明: "任意一个浮点数"
ECALC_EXTERN_C void ecalc_IncBeta_49_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[1].m_double;
    double   arg2 = pArgInf[2].m_double;
    double   arg3 = pArgInf[3].m_double;

}

