#include "..\include_regex_header.h"

// 调用格式: SDT_TEXT (搜索结果).取子匹配文本, 命令说明: "取得与正则表达式中某个子表达式匹配的子文本"
// 参数<1>: 被搜索的文本 SDT_TEXT, 参数说明: "应该使用和所属的“正则表达式.搜索”命令同样的参数"
// 参数<2>: 子表达式索引 SDT_INT, 参数说明: "欲取其值的子表达式。该参数对应与正则表达式中的一个子表达式（以圆括号标记）。索引从1开始"
// 参数<3>: [&起始位置 SDT_INT], 参数说明: "该命令调用成功后，本参数存放取得的子文本在整个被搜索文本中的起始位置"
REGEX_EXTERN_C void regex_GetSubMatchText_12_regex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;
    PINT     arg3 = pArgInf[3].m_pInt;

}

