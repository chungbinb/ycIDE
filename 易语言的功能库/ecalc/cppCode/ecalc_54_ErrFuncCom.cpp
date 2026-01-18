#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (其他计算).求误差余, 命令说明: "求解误差函数的余函数"
// 参数<1>: x SDT_DOUBLE, 参数说明: "该参数提供任意一个不等于0的数"
// 参数<2>: &错误码 SDT_INT, 参数说明: "可为以下常量之一：0、#无错误；1、#参数必须是一个不为0的数字；2、#参数不能为负数；3、#多项式序号必须大于0；4、#参数必须大于-1"
ECALC_EXTERN_C void ecalc_ErrFuncCom_54_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[1].m_double;
    PINT     arg2 = pArgInf[2].m_pInt;

}

