#include "..\include_regex_header.h"

// 调用格式: SDT_BOOL (正则表达式).匹配, 命令说明: "用一个正则表达式来匹配一个文本。如果这个文本符合正则表达式指定的模式，返回真；否则返回假。比如：\r\n        正则表达式.创建 (“易语言4\\.0(模块|支持库)?”)\r\n        信息框 (正则表达式.匹配 (“易语言4.0支持库”), 0, )\r\n    将显示“真”"
// 参数<1>: 用来匹配的文本 SDT_TEXT, 参数说明: NULL
REGEX_EXTERN_C void regex_Match_0_regex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

