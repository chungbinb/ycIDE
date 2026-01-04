#include "..\include_ecalc_header.h"

// 本命令被隐藏, 原始名字 = "求次方"
// 调用格式: MAKELONG(0x0A, 0) (大数).求次方, 命令说明: "计算并返回本对象的指定次方"
// 参数<1>: 次方数 MAKELONG(0x0A, 0), 参数说明: "该参数指定对欲求次方数值的运算指数"
ECALC_EXTERN_C void ecalc_Pow_98_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

