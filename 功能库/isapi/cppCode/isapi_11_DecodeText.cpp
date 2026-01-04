#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网扩展).解码文本, 命令说明: "将客户端用获取(GET)或用投递(POST)传上来的文本进行解码。"
// 参数<1>: 文本 SDT_TEXT, 参数说明: "本参数表示要解码的文本"
// 参数<2>: 连接符 SDT_BYTE, 参数说明: "连接各文本的字符，本字符将被替换为空格，默认为 +"
ISAPI_EXTERN_C void isapi_DecodeText_11_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BYTE     arg2 = pArgInf[2].m_byte;

}

