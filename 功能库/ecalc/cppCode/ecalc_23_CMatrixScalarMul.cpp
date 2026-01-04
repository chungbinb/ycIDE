#include "..\include_ecalc_header.h"

// 本命令被隐藏, 原始名字 = "复矩阵标量乘积"
// 调用格式: MAKELONG(0x01, 0) (矩阵运算).[]复矩阵标量乘积, 命令说明: "计算矩阵和一个复数的乘积"
// 参数<1>: 复数矩阵 数组 MAKELONG(0x01, 0), 参数说明: "二维复数数组"
// 参数<2>: 标量操作数 MAKELONG(0x01, 0), 参数说明: "包含标量错误数，同样也是复数类型"
ECALC_EXTERN_C void ecalc_CMatrixScalarMul_23_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

