#include "..\include_ecalc_header.h"

// 调用格式: MAKELONG(0x01, 0) (复数运算).求复数指数, 命令说明: "计算以e为底，以复数为幂的值"
// 参数<1>: 复数数据 MAKELONG(0x01, 0), 参数说明: "该参数提供需要参与计算的复数数据"
ECALC_EXTERN_C void ecalc_ComplexExp_6_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

