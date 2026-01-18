#include "..\include_deelxregex_header.h"

// 调用格式: MAKELONG(0x02, 0) (正则表达式DEELX).搜索W, 命令说明: "使用指定的正则表达式搜索指定文本中与该表达式匹配的子文本。本命令和“查找匹配”一样，为兼容原支持库增加。"
// 参数<1>: [用来匹配的文本 SDT_BIN], 参数说明: "Unicode格式，进行匹配的字符串"
// 参数<2>: [开始查找匹配的位置 SDT_INT], 参数说明: "开始查找匹配的位置."
DEELXREGEX_EXTERN_C void deelxregex_SearchW_28_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    INT      arg2 = pArgInf[2].m_int;

}

