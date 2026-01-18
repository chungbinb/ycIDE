#include "..\include_eapi_header.h"

// 调用格式: _SDT_NULL 打开指定网址, 命令说明: "用系统默认浏览器打开指定的网址。"
// 参数<1>: [网址url或文件路径 SDT_TEXT], 参数说明: NULL
EAPI_EXTERN_C void eapi_OpenURL_56_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

