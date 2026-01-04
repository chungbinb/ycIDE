#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (联立方程).线性方程组, 命令说明: "本函数采用Gauss - Jordan方法求解实型线性方程组"
// 参数<1>: x变量系数 数组 SDT_DOUBLE, 参数说明: "该参数是一个二维正方形数组，数组宽度和高度相等，宽度和高度必须小于等于50，它保存x变量系数的方阵，该参数数组的数据必须提供全部不为0的数值，否则本方法将失败"
// 参数<2>: y向量 数组 SDT_DOUBLE, 参数说明: "该参数是列数和前一个参数一样的一维数组"
// 参数<3>: x变量解 数组 SDT_DOUBLE, 参数说明: "列数和第一个参数一样的一维数组"
// 参数<4>: x变量系数逆阵 数组 SDT_DOUBLE, 参数说明: "和第一个参数行列数一样的二维数组，它保存x变量系数矩阵的逆阵"
// 参数<5>: &x矩阵行列式 SDT_DOUBLE, 参数说明: "返回x变量系数矩阵的行列式"
ECALC_EXTERN_C void ecalc_GaussJordan_33_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    void*    arg3 = pArgInf[3].m_pAryData;
    void*    arg4 = pArgInf[4].m_pAryData;
    double*  arg5 = pArgInf[5].m_pDouble;

}

