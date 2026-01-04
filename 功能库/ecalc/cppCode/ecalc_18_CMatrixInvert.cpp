#include "..\include_ecalc_header.h"

// 本命令被隐藏, 原始名字 = "复矩阵逆"
// 调用格式: MAKELONG(0x01, 0) (矩阵运算).[]复矩阵逆, 命令说明: "计算复矩阵的逆阵。采用的算法是把N x N复矩阵转换成2N x 2N的实数矩阵。标准Gauss Jordan函数被用于计算2N x 2N数组的逆。这个数组的逆又被变换回所要求的复数逆数组"
// 参数<1>: 复数矩阵 数组 MAKELONG(0x01, 0), 参数说明: "该复数矩阵必须是一个正方型矩阵"
// 参数<2>: &行列式 SDT_DOUBLE, 参数说明: NULL
ECALC_EXTERN_C void ecalc_CMatrixInvert_18_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    double*  arg2 = pArgInf[2].m_pDouble;

}

