#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网扩展).编码文本, 命令说明: "将一段文本编码为URL格式的。"
// 参数<1>: 文本 SDT_TEXT, 参数说明: "本参数表示要编码的文本。"
// 参数<2>: 连接符 SDT_BYTE, 参数说明: "本参数表示要将空格替换为哪个字符，默认为'+'。"
ISAPI_EXTERN_C void isapi_EncodeText_25_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BYTE     arg2 = pArgInf[2].m_byte;

}

