#include "..\include_ecalc_header.h"

// 调用格式: SDT_DOUBLE (其他计算).求误差实部, 命令说明: "计算误差函数Z = x + iy 的实部"
// 参数<1>: x SDT_DOUBLE, 参数说明: "保存复数的实部"
// 参数<2>: y SDT_DOUBLE, 参数说明: "保存复数的实部"
// 参数<3>: &错误码 SDT_INT, 参数说明: "可为以下常量之一：0、#无错误；1、#参数必须是一个不为0的数字；2、#参数不能为负数；3、#多项式序号必须大于0；4、#参数必须大于-1"
ECALC_EXTERN_C void ecalc_ErrFuncR_55_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[1].m_double;
    double   arg2 = pArgInf[2].m_double;
    PINT     arg3 = pArgInf[3].m_pInt;

}

