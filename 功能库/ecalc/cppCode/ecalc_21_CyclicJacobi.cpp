#include "..\include_ecalc_header.h"

// 调用格式: _SDT_NULL (矩阵运算).求矩阵特征, 命令说明: "用循环Jacobi算法计算一个实对称矩阵的特征值"
// 参数<1>: 实数矩阵 数组 SDT_DOUBLE, 参数说明: "该参数为一个正方形实数矩阵"
// 参数<2>: 向量 数组 SDT_DOUBLE, 参数说明: "该参数是一维数组，列数和第一个参数一样，返回第一个参数矩阵的特征值向量"
// 参数<3>: 返回实数矩阵 数组 SDT_DOUBLE, 参数说明: "该参数数组是维数和行列数和第一个参数一样的矩阵，它的列是原始矩阵的eigenvectors"
// 参数<4>: &记数 SDT_INT, 参数说明: "包含完成迭代的最大迭代次数，它返回完成迭代的实际迭代次数"
// 参数<5>: &收敛标志 SDT_BOOL, 参数说明: "通过该参数判断是否收敛成功"
ECALC_EXTERN_C void ecalc_CyclicJacobi_21_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pAryData;
    void*    arg3 = pArgInf[3].m_pAryData;
    PINT     arg4 = pArgInf[4].m_pInt;
    BOOL*    arg5 = pArgInf[5].m_pBool;

}

