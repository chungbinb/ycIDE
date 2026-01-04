#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (矩阵运算).[]实矩阵逆, 命令说明: "计算一个实矩阵的逆。如果参数错误或者计算失败，本方法将返回一个大小为 1 的一维数组。不是所有矩阵都有逆矩阵。"
// 参数<1>: 实数矩阵 数组 SDT_DOUBLE, 参数说明: "该实数矩阵必须是一个正方型矩阵"
// 参数<2>: [&保留参数 SDT_DOUBLE], 参数说明: NULL
ECALC_EXTERN_C void ecalc_MatrixInvert_17_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    double*  arg2 = pArgInf[2].m_pDouble;

}

