#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (曲线拟合).二乘曲线拟合, 命令说明: "本函数是一个广义最小二乘曲线拟合函数，它对一个相关或不相关变量数据集用线性系数拟合一个多项式"
// 参数<1>: 独立变量观察值 数组 SDT_DOUBLE, 参数说明: "该参数是一个一维数组，保存独立变量的观察值"
// 参数<2>: 相关变量观察值 数组 SDT_DOUBLE, 参数说明: "该参数为列数和第一个参数一样的一维数组，它保存相关变量的观察值"
// 参数<3>: 多项式系数 数组 SDT_DOUBLE, 参数说明: "该参数是列数小于第一个参数的一维数组，它保存拟合多项式的系数，数组第1个成员保存常数，数组第2个成员保存x ^ 1的系数，数组第3个成员保存x ^ 2的系数，依此类推"
// 参数<4>: y估值 数组 SDT_DOUBLE, 参数说明: "该参数为列数和第一个参数一样的一维数组，它返回用多项式系数计算的y的估值"
// 参数<5>: 残差值 数组 SDT_DOUBLE, 参数说明: "返回残差值。"
// 参数<6>: &标准误差 SDT_DOUBLE, 参数说明: "返回估计的标准误差"
// 参数<7>: 估计标准误差 数组 SDT_DOUBLE, 参数说明: "该参数为列数和\"多项式系数\"一样的一维数组，它返回系数估计的标准误差"
// 参数<8>: &r平方值 SDT_DOUBLE, 参数说明: "返回r平方值 - 样本确定系数"
// 参数<9>: &r值 SDT_DOUBLE, 参数说明: "返回r值 - 样本相关系数"
// 参数<10>: &是否奇异 SDT_INT, 参数说明: "如果曲线拟合矩阵奇异则返回1"
ECALC_EXTERN_C void ecalc_PolyCurveFit_36_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    void*    arg3 = pArgInf[3].m_pAryData;
    void*    arg4 = pArgInf[4].m_pAryData;
    void*    arg5 = pArgInf[5].m_pAryData;
    double*  arg6 = pArgInf[6].m_pDouble;
    void*    arg7 = pArgInf[7].m_pAryData;
    double*  arg8 = pArgInf[8].m_pDouble;
    double*  arg9 = pArgInf[9].m_pDouble;
    PINT     arg10 = pArgInf[10].m_pInt;

}

