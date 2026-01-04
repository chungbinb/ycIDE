#include "..\include_exmlparser_header.h"

// 调用格式: SDT_BOOL 匹配通配符, 命令说明: "该方法第一个参数包含通配符（*,?），通配符“*”代表零个或多个任意字符，通配符“?”代表一个任意字符，第二个参数是一个不包含通配符的常量字符串，该方法得到匹配字符串和常量字符串的比较结果。比较相等返回真，不等返回假"
// 参数<1>: 匹配文本 SDT_TEXT, 参数说明: "包含通配符的文本"
// 参数<2>: 常量文本 SDT_TEXT, 参数说明: "不包含通配符的普通文本"
EXMLPARSER_EXTERN_C void exmlparser_BlurMatch_23_exmlparser(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

