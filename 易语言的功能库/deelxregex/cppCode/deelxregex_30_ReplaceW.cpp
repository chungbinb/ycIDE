#include "..\include_deelxregex_header.h"

// 调用格式: SDT_BIN (正则表达式DEELX).替换W, 命令说明: "进行文本替换操作。返回替换后的文本(Unicode格式文本，字节集类型)。"
// 参数<1>: 欲被替换的文本 SDT_BIN, 参数说明: "Unicode格式，被进行替换的初始文本。"
// 参数<2>: [用作替换的文本 SDT_BIN], 参数说明: "Unicode格式，“替换为”字符串，将匹配到的子字符串替换成 此变量的 字符串。"
// 参数<3>: [起始替换位置 SDT_INT], 参数说明: "进行查找替换的开始位置。留空默认(-1)表示根据是否“从右向左(RIGHTTOLEFT)”自动决定开始位置。"
// 参数<4>: [替换进行的次数 SDT_INT], 参数说明: "指定进行替换的次数。留空默认(-1)表示替换所有匹配。"
DEELXREGEX_EXTERN_C void deelxregex_ReplaceW_30_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    LPBYTE   arg2 = pArgInf[2].m_pBin;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;

}

