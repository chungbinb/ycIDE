#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).写出回复头, 命令说明: "等效于调用扩展函数HSE_REQ_SEND_RESPONSE_HEADER_EX"
// 参数<1>: [HTTP状态 SDT_TEXT], 参数说明: "如果被省略，默认为空文本"
// 参数<2>: [HTTP头 SDT_TEXT], 参数说明: "如果被省略，默认为空文本。通常应以两个 #换行符 结尾。"
// 参数<3>: [是否保持连接 SDT_BOOL], 参数说明: "如果被省略，默认为假"
ISAPI_EXTERN_C void isapi_SendResponeHeaderEx_30_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    BOOL     arg3 = pArgInf[3].m_bool;

}

