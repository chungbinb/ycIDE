#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网扩展).取服务器变量, 命令说明: "取服务器变量。"
// 参数<1>: 变量类型 SDT_INT, 参数说明: "要取的变量类型，可以为“服务器变量类型”中的常量值。"
ISAPI_EXTERN_C void isapi_GetServerVar_5_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

