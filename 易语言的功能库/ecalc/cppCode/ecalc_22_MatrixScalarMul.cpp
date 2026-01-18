#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (矩阵运算).[]矩阵标量乘积, 命令说明: "计算矩阵和一个标量值的乘积,如果参数错误或者计算失败，本方法将返回一个1维宽度为1的数组"
// 参数<1>: 实数矩阵 数组 SDT_DOUBLE, 参数说明: "提供一个二维数组"
// 参数<2>: 标量操作数 SDT_DOUBLE, 参数说明: "定义一个标量操作数"
ECALC_EXTERN_C void ecalc_MatrixScalarMul_22_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    double   arg2 = pArgInf[2].m_double;

}

