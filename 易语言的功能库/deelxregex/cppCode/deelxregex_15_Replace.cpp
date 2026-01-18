#include "..\include_deelxregex_header.h"

// 调用格式: SDT_TEXT (正则表达式DEELX).替换, 命令说明: "进行文本替换操作。返回替换后的文本。"
// 参数<1>: 欲被替换的文本 SDT_TEXT, 参数说明: "被进行替换的初始文本。"
// 参数<2>: [用作替换的文本 SDT_TEXT], 参数说明: "“替换为”字符串，将匹配到的子字符串替换成 此变量的 字符串。"
// 参数<3>: [起始替换位置 SDT_INT], 参数说明: "进行查找替换的开始位置。留空默认(-1)表示根据是否“从右向左(RIGHTTOLEFT)”自动决定开始位置。"
// 参数<4>: [替换进行的次数 SDT_INT], 参数说明: "指定进行替换的次数。留空默认(-1)表示替换所有匹配。"
DEELXREGEX_EXTERN_C void deelxregex_Replace_15_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;

}

