#include "..\include_regex_header.h"

// 调用格式: SDT_TEXT (搜索结果).取匹配文本, 命令说明: "取得与整个正则表达式匹配的子文本"
// 参数<1>: 被搜索的文本 SDT_TEXT, 参数说明: "应该使用和所属的“正则表达式.搜索”命令同样的参数"
// 参数<2>: [&起始位置 SDT_INT], 参数说明: "该命令调用成功后，本参数存放取得的子文本在整个被搜索文本中的起始位置"
REGEX_EXTERN_C void regex_GetMatchText_11_regex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    PINT     arg2 = pArgInf[2].m_pInt;

}

