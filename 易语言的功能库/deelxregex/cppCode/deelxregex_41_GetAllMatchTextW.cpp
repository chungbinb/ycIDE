#include "..\include_deelxregex_header.h"

// 调用格式: SDT_BIN (正则表达式DEELX).[]取所有匹配文本W, 命令说明: "取得与整个正则表达式匹配的文本。返回匹配到的一维字节集数组，本命令不处理正则表达式中括号里的子文本，和“分割”命令相似，但结果不同。如果失败则返回一个空数组，即没有任何成员的数组。"
// 参数<1>: 用来匹配的文本 SDT_BIN, 参数说明: "Unicode格式文本"
// 参数<2>: [要返回的匹配文本数目 SDT_INT], 参数说明: "如果被省略或等于0，则默认返回所有的匹配文本"
DEELXREGEX_EXTERN_C void deelxregex_GetAllMatchTextW_41_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    INT      arg2 = pArgInf[2].m_int;

}

