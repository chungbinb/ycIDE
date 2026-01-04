#include "..\include_deelxregex_header.h"

// 本命令被隐藏, 原始名字 = "释放字符串"
// 调用格式: SDT_BOOL (正则表达式DEELX).释放字符串, 命令说明: "释放字符串"
// 参数<1>: [由(替换)返回的字符串 SDT_TEXT], 参数说明: "释放由 \"替换\" 返回的字符串。"
DEELXREGEX_EXTERN_C void deelxregex_ReleaseString_16_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

