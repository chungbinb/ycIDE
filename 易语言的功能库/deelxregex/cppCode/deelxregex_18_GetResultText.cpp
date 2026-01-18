#include "..\include_deelxregex_header.h"

// 调用格式: SDT_TEXT (正则表达式DEELX).取结果文本, 命令说明: "取搜索结果的文本"
// 参数<1>: 搜索结果起始位置 SDT_INT, 参数说明: "搜索结果起始位置"
// 参数<2>: 搜索结果结束位置 SDT_INT, 参数说明: "搜索结果结束位置"
DEELXREGEX_EXTERN_C void deelxregex_GetResultText_18_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

