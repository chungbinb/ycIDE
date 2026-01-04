#include "..\include_deelxregex_header.h"

// 本命令被隐藏, 原始名字 = "对象匹配"
// 调用格式: MAKELONG(0x02, 0) (正则表达式DEELX).对象匹配, 命令说明: "经过初始化的查找匹配上下文对象，用来在 “对象匹配” 中使用。"
// 参数<1>: [用来匹配的文本 SDT_TEXT], 参数说明: "进行匹配的字符串"
// 参数<2>: [开始查找匹配的位置 SDT_INT], 参数说明: "开始查找匹配的位置."
DEELXREGEX_EXTERN_C void deelxregex_PrepareMatch_14_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;

}

