#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).重定向, 命令说明: "进行重定向，让浏览器访问一个指定的网页。"
// 参数<1>: 地址 SDT_TEXT, 参数说明: "本参数表示要重定向的地址，所有HEAD的其它信息如Content-Type，可以写在地址的后面，注意加回车换行。"
ISAPI_EXTERN_C void isapi_Redirect_27_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

