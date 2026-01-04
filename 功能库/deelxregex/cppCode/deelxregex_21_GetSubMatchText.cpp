#include "..\include_deelxregex_header.h"

// 调用格式: SDT_TEXT (搜索结果DEELX).取子匹配文本, 命令说明: "取得与正则表达式中某个子表达式匹配的子文本。"
// 参数<1>: 子表达式索引或名称 _SDT_ALL, 参数说明: "欲取其值的子表达式。该参数对应与正则表达式中的一个子表达式（以圆括号标记）。子匹配索引从1开始，0表示匹配文本。\r\n\t或者传递文本型的子表达式分组名称，比如“(?<数字>\\d+)(?<小写字母>[a-z]+)(?<大写字母>[A-Z]+)”这样的表达式，可以传递“数字”、“小写字母”等名称。\r\n\t也可以传递字节集形式的参数，当成Unicode格式的名称对待。"
DEELXREGEX_EXTERN_C void deelxregex_GetSubMatchText_21_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;

}

