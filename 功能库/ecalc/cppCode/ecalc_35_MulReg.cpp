#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (多重回归).多重回归, 命令说明: "对数据集建立一个最小二乘线性多重回归方程。它返回回归方程的系数、y的估值、残差、估计的标准误差、回归系数的标准误差、确定系数和相关系数"
// 参数<1>: 原始数据 数组 SDT_DOUBLE, 参数说明: "该参数是二维数组，保存被分析的原始数据"
// 参数<2>: x矩阵行列式 数组 SDT_DOUBLE, 参数说明: "列数和第一个参数行数一样的一维数组"
// 参数<3>: 回归方程系数 数组 SDT_DOUBLE, 参数说明: "该参数为列数和第一个参数一样的一维数组，它返回计算出来的回归方程系数"
// 参数<4>: y估值 数组 SDT_DOUBLE, 参数说明: "该参数为列数与第一个参数行数一样的二维数组，它返回用回归系数计算的y的估值"
// 参数<5>: 残差值 数组 SDT_DOUBLE, 参数说明: "该参数为列数与第一个参数行数一样的二维数组，它返回残差值(y的估值与y的观察值之差)"
// 参数<6>: &标准误差 SDT_DOUBLE, 参数说明: "返回估计的标准误差"
// 参数<7>: 估计标准误差 数组 SDT_DOUBLE, 参数说明: "该参数是列数与第一个参数一样的一维数组，它返回系数估计的标准误差"
// 参数<8>: &r平方值 SDT_DOUBLE, 参数说明: "返回r平方值 - 样本确定系数"
// 参数<9>: &r值 SDT_DOUBLE, 参数说明: "返回r值 - 样本相关系数"
// 参数<10>: &是否回归矩阵奇异 SDT_INT, 参数说明: "如果回归矩阵奇异，则返回一个非\"0\"值，否则返回\"0\""
ECALC_EXTERN_C void ecalc_MulReg_35_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
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

