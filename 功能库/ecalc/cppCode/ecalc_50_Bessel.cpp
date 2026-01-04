#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (其他计算).求贝塞尔, 命令说明: "通过指定参数求贝塞尔"
// 参数<1>: v SDT_DOUBLE, 参数说明: "该参数表示次数，必须大于等于 0"
// 参数<2>: x SDT_DOUBLE, 参数说明: "该参数必须大于0或者小于0\t"
// 参数<3>: &错误码 SDT_INT, 参数说明: "可为以下常量之一：0、#无错误；1、#参数必须是一个不为0的数字；2、#参数不能为负数；3、#多项式序号必须大于0；4、#参数必须大于-1"
ECALC_EXTERN_C void ecalc_Bessel_50_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[1].m_double;
    double   arg2 = pArgInf[2].m_double;
    PINT     arg3 = pArgInf[3].m_pInt;

}

