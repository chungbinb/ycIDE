#include "..\include_ecalc_header.h"

// 本命令被隐藏, 原始名字 = "复矩阵相乘"
// 调用格式: MAKELONG(0x01, 0) (矩阵运算).[]复矩阵相乘, 命令说明: "计算两个复矩阵的乘积，注意：必须保证矩阵1的列数等于矩阵2的行数，如果参数错误或者计算失败，本方法将返回一个1维宽度为1的数组"
// 参数<1>: 复数矩阵1 数组 MAKELONG(0x01, 0), 参数说明: "参与运算的复数矩阵1"
// 参数<2>: 复数矩阵2 数组 MAKELONG(0x01, 0), 参数说明: "参与运算的复数矩阵2"
ECALC_EXTERN_C void ecalc_CMatrixMul_14_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;

}

