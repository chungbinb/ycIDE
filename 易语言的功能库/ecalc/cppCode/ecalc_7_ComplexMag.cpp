#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (复数运算).求复数长度, 命令说明: "计算一个复数的长度"
// 参数<1>: 复数数据 MAKELONG(0x01, 0), 参数说明: "该参数提供需要参与计算的复数数据"
ECALC_EXTERN_C void ecalc_ComplexMag_7_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

