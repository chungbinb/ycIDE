#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (其他计算).求误差虚部, 命令说明: "计算误差函数Z = x + iy的虚部"
// 参数<1>: x SDT_DOUBLE, 参数说明: "保存复数的实部"
// 参数<2>: y SDT_DOUBLE, 参数说明: "保存复数的虚部"
ECALC_EXTERN_C void ecalc_ErrFuncI_56_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[1].m_double;
    double   arg2 = pArgInf[2].m_double;

}

