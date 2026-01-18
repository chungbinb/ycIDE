#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).调用扩展函数, 命令说明: "调用服务器提供的扩展函数，完成其他各种功能，具体视参数“欲调用的函数”而定"
// 参数<1>: 欲调用的函数 SDT_INT, 参数说明: "应为枚举型常量 #互联网扩展函数.xxx 成员之一"
// 参数<2>: 缓冲区地址 SDT_INT, 参数说明: NULL
// 参数<3>: &缓冲区长度 SDT_INT, 参数说明: "返回后此参数值可能会被改写"
// 参数<4>: [&数据类型 SDT_INT], 参数说明: "仅在调用部分扩展函数时用到"
ISAPI_EXTERN_C void isapi_ServerSupportFunction_28_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    PINT     arg3 = pArgInf[3].m_pInt;
    PINT     arg4 = pArgInf[4].m_pInt;

}

