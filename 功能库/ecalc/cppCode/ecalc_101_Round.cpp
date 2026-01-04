#include "..\include_ecalc_header.h"

// 调用格式: MAKELONG(0x0A, 0) (大数).四舍五入, 命令说明: "返回本对象按照指定的方式进行四舍五入运算的结果"
// 参数<1>: 舍入位置 SDT_INT, 参数说明: "如果大于0，表示小数点右边应保留的位数；如果等于0，表示舍入到整数；如果小于0，表示小数点左边舍入到的位置。例如：四舍五入 (1056.65, 1) 返回 1056.7； 四舍五入 (1056.65, 0) 返回 1057； 四舍五入 (1056.65, -1) 返回 1060"
ECALC_EXTERN_C void ecalc_Round_101_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

