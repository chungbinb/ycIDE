#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT 取IP地址, 命令说明: NULL
// 参数<1>: IP地址 SDT_TEXT, 参数说明: NULL
EAPI_EXTERN_C void eapi_GetRemoteName_47_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

