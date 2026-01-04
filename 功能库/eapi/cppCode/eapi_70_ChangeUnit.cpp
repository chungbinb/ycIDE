#include "..\include_eapi_header.h"

// 调用格式: SDT_DOUBLE 屏幕单位转换, 命令说明: NULL
// 参数<1>: 源单位值 SDT_DOUBLE, 参数说明: "屏幕显示区域的宽度，单位为像素点。"
// 参数<2>: [源单位 SDT_INT], 参数说明: "单位类型，为以下常量值之一：0、#接口常量.像素；1、#接口常量.英寸；2、#接口常量.毫米；3、#接口常量.厘米。"
// 参数<3>: [目的单位 SDT_INT], 参数说明: "单位类型，为以下常量值之一：0、#接口常量.像素；1、#接口常量.英寸；2、#接口常量.毫米；3、#接口常量.厘米。"
EAPI_EXTERN_C void eapi_ChangeUnit_70_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    double   arg1 = pArgInf[0].m_double;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;

}

