#include "..\include_ecalc_header.h"

// 本命令被隐藏, 原始名字 = "复矩阵转置"
// 调用格式: MAKELONG(0x01, 0) (矩阵运算).[]复矩阵转置, 命令说明: "计算一个复矩阵的转置"
// 参数<1>: 复数矩阵 数组 MAKELONG(0x01, 0), 参数说明: "该参数提供将被转置的复数矩阵"
ECALC_EXTERN_C void ecalc_CMatrixTranspose_20_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pAryData;

}

