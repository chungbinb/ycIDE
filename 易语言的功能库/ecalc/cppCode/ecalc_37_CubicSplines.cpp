#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (曲线拟合).三次多项式方程, 命令说明: "本函数对离散数据集拟合一系列三次多项式方程。通过把原始数据点区间上的三次方程应用于插值中要使用的x值，在那个区间上的y值可能被插值。在每个子区间上(sv[i],sv[i+1])，三次样条形如：y的估值 = a[i] + b[i] * (x - xv[i]) + c[i] * (x - sv[i]) ^ 2 + d[i] * (s - sv[i]) ^ 3"
// 参数<1>: x值 数组 SDT_DOUBLE, 参数说明: "该参数是一个一维数组，它保存离散数据的x值"
// 参数<2>: y值 数组 SDT_DOUBLE, 参数说明: "该参数是列数和第一个参数一样的一维数组，它保存离散数据集的y值"
// 参数<3>: 三次样条方程系数 数组 SDT_DOUBLE, 参数说明: "该参数是行数和第一个参数列数一样，列数为4的二维数组，它返回每个子区间上三次样条方程的系数"
ECALC_EXTERN_C void ecalc_CubicSplines_37_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    void*    arg3 = pArgInf[3].m_pAryData;

}

