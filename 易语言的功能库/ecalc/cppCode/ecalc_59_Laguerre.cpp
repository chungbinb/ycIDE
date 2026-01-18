#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (其他计算).求拉格尔, 命令说明: "本函数是在下列方程中寻找多项式第n个根的向前方法:Ln(a)(x) = xy'' + (a + 1 - x)y' + ny = 0"
// 参数<1>: n SDT_INT, 参数说明: "该参数是指定多项式次数的一个整数"
// 参数<2>: a SDT_DOUBLE, 参数说明: "任一大于-1的数"
// 参数<3>: x SDT_DOUBLE, 参数说明: "任意浮点数"
// 参数<4>: &错误码 SDT_INT, 参数说明: "可为以下常量之一：0、#无错误；1、#参数必须是一个不为0的数字；2、#参数不能为负数；3、#多项式序号必须大于0；4、#参数必须大于-1"
ECALC_EXTERN_C void ecalc_Laguerre_59_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    double   arg2 = pArgInf[2].m_double;
    double   arg3 = pArgInf[3].m_double;
    PINT     arg4 = pArgInf[4].m_pInt;

}

