#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).写出文本, 命令说明: "写出文本给客户端。文本一般是(HTML)文本。"
// 参数<1>: 文本 SDT_TEXT, 参数说明: "本参数表示要写的文本。"
ISAPI_EXTERN_C void isapi_WriteText_4_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

