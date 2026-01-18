#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (其他计算).求不完全伽玛, 命令说明: "求指定数的不完全伽玛函数，本函数是定义在所有大于0的实数上的"
// 参数<1>: a SDT_DOUBLE, 参数说明: "欲求Gamma值的a"
// 参数<2>: x SDT_DOUBLE, 参数说明: "欲求Gamma值的x"
ECALC_EXTERN_C void ecalc_IncGamma_46_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[1].m_double;
    double   arg2 = pArgInf[2].m_double;

}

