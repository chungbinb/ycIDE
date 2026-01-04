#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网扩展).获取存根, 命令说明: "获取存根(Cookie)的值。"
// 参数<1>: 名称 SDT_TEXT, 参数说明: "本参数表示存根(Cookie)的名称。是“设置存根”方法设置的名称。"
ISAPI_EXTERN_C void isapi_GetCookie_17_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

