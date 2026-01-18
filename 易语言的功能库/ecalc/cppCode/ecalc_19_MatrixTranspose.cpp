#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (矩阵运算).[]矩阵转置, 命令说明: "计算矩阵的转置,如果参数错误或者计算失败，本方法将返回一个1维宽度为1的数组"
// 参数<1>: 实数矩阵 数组 SDT_DOUBLE, 参数说明: "该参数提供将被转置的矩阵"
ECALC_EXTERN_C void ecalc_MatrixTranspose_19_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;

}

