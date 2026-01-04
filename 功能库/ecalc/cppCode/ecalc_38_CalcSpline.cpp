#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (曲线拟合).三次样条插值, 命令说明: "该函数计算给定一个x值和由\"三次多项式方程\"计算的三次样条系数矩阵，y值的三次样条插值"
// 参数<1>: x值 数组 SDT_DOUBLE, 参数说明: "该参数为一维数组，它保存离散数据集的x值。用来确定在计算过程中使用哪个样条区间"
// 参数<2>: 三次样条方程系数 数组 SDT_DOUBLE, 参数说明: "该参数是行数和第一个参数一样，列数为4的二维数组。它保存每个子区间的三次样条方程的系数"
// 参数<3>: 插值x SDT_DOUBLE, 参数说明: "包含在插值中使用的x值"
// 参数<4>: &y值 SDT_DOUBLE, 参数说明: "被计算的y值"
ECALC_EXTERN_C void ecalc_CalcSpline_38_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    double   arg3 = pArgInf[3].m_double;
    double*  arg4 = pArgInf[4].m_pDouble;

}

