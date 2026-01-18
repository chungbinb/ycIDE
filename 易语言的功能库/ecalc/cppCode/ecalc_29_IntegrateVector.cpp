#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (微积分).离散化数值积分, 命令说明: "计算一个离散化的抽样数据集的数值积分(即通常曲线下的面积)。采用的算法是Simpson 1/3法则和Simpson 3/8法则，与其他方法不同，这个方法可以同时处理偶数次和奇数次观察的数据集"
// 参数<1>: 浮点向量 数组 SDT_DOUBLE, 参数说明: "一维数组，它保存离散样本集"
// 参数<2>: 样本区间 SDT_DOUBLE, 参数说明: "定义采样数据集之间的样本区间"
// 参数<3>: 积分下限 SDT_INT, 参数说明: "积分的下限，本参数为大于等于0且小于等于\"浮点向量\"参数数组下标数减1的整数，该参数必须小于参数\"积分上限\"，否则将出错"
// 参数<4>: 积分上限 SDT_INT, 参数说明: "积分的上限，本参数为大于等于0且小于等于\"浮点向量\"参数数组下标数减1的整数，该参数必须大于参数\"积分下限\""
ECALC_EXTERN_C void ecalc_IntegrateVector_29_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    double   arg2 = pArgInf[2].m_double;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;

}

