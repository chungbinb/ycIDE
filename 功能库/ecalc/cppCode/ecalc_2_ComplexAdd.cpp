#include "..\include_ecalc_header.h"

// 调用格式: MAKELONG(0x01, 0) (复数运算).复数相加, 命令说明: "求两个复数之和"
// 参数<1>: 复数数据1 MAKELONG(0x01, 0), 参数说明: "该参数提供参与计算的复数数据1，例如：复数1 + 复数2，本参数提供复数1"
// 参数<2>: 复数数据2 MAKELONG(0x01, 0), 参数说明: "该参数提供参与计算的复数数据2，例如：复数1 + 复数2算式，本参数提供复数2"
ECALC_EXTERN_C void ecalc_ComplexAdd_2_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

