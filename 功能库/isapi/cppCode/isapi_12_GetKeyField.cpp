#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网扩展).据关键字找文本, 命令说明: "根据关键字找文本。如这样一个文本“key1=value1+key2=value2”根据“key”找“value”。"
// 参数<1>: 文本 SDT_TEXT, 参数说明: "本参数表示要查找的文本"
// 参数<2>: 关键字 SDT_TEXT, 参数说明: "关键字名字"
// 参数<3>: 分割字符 SDT_BYTE, 参数说明: "分割的字符。黙认为'&'。 "
ISAPI_EXTERN_C void isapi_GetKeyField_12_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    BYTE     arg3 = pArgInf[3].m_byte;

}

