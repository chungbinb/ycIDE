#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网筛选器).取服务器变量, 命令说明: NULL
// 参数<1>: 服务器变量名称 SDT_TEXT, 参数说明: NULL
ISAPI_EXTERN_C void isapi_FilterGetServerVariable_66_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

