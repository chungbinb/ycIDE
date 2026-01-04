#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (微积分).函数数值积分, 命令说明: "计算一个所指定的函数的数值积分(即通常曲线下的面积)。采用的算法是Simpson 1/3法则和Simpson 3/8 法则"
// 参数<1>: 样本区间 SDT_INT, 参数说明: "数据集的样本区间，该参数必须为正整数，否则将出错"
// 参数<2>: 积分下限 SDT_DOUBLE, 参数说明: "积分下限，该参数必须小于参数\"积分上限\"，否则将出错"
// 参数<3>: 积分上限 SDT_DOUBLE, 参数说明: "积分上限，该参数必须大于参数\"积分下限\"，否则将出错"
ECALC_EXTERN_C void ecalc_IntegrateFunction_30_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    double   arg2 = pArgInf[2].m_double;
    double   arg3 = pArgInf[3].m_double;

}

