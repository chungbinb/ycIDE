#include "..\include_regex_header.h"

// 调用格式: SDT_TEXT (正则表达式).替换, 命令说明: "使用指定的正则表达式搜索指定文本中与该表达式匹配的子文本，并按照指定的格式进行替换。返回替换后的文本"
// 参数<1>: 欲被替换的文本 SDT_TEXT, 参数说明: NULL
// 参数<2>: 替换格式 SDT_TEXT, 参数说明: "本参数指定替换的具体方式。比如“<font color = gray>$1</font>”表示把第一个子表达式所匹配的文本显示为灰色（在HTML中）"
// 参数<3>: 起始替换位置 SDT_INT, 参数说明: NULL
// 参数<4>: [替换长度 SDT_INT], 参数说明: "本参数和“起始替换位置”一起指定替换的范围。如果本参数被省略，默认值为从“起始替换位置”到文本结尾的长度"
// 参数<5>: 是否包含不匹配部分 SDT_BOOL, 参数说明: "本参数指定替换结果中是否包含与正则表达式不匹配的部分。如果本参数为假，则不匹配部分将被抛弃"
// 参数<6>: 是否全部替换 SDT_BOOL, 参数说明: "如果本参数为真，则参数1中所有匹配的子文本均被替换，否则仅第一个匹配的子文本被替换"
REGEX_EXTERN_C void regex_Replace_15_regex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;
    BOOL     arg5 = pArgInf[5].m_bool;
    BOOL     arg6 = pArgInf[6].m_bool;

}

