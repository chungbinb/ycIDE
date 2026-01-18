#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (矩阵运算).[]矩阵相加, 命令说明: "求两个二维矩阵之和，两个矩阵的宽度和高度必须相等，如果参数错误或者计算失败，本方法将返回一个1维宽度为1的数组"
// 参数<1>: 实数矩阵1 数组 SDT_DOUBLE, 参数说明: "参与运算的实数矩阵1"
// 参数<2>: 实数矩阵2 数组 SDT_DOUBLE, 参数说明: "参与运算的实数矩阵2"
ECALC_EXTERN_C void ecalc_MatrixAdd_15_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;

}

