#include "..\include_ecalc_header.h"

// 调用格式: MAKELONG(0x0A, 0) (大数).求余切, 命令说明: "反回本对象的余切值， 目前支持计算整数的余切值，小数部分将被忽略"
// 参数<1>: 计算精度 SDT_INT, 参数说明: "指明计算精度，注意：该精度为计算中的除法精度"
ECALC_EXTERN_C void ecalc_Atn_108_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

