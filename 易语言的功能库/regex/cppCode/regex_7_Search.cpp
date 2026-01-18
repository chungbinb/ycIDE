#include "..\include_regex_header.h"

// 调用格式: MAKELONG(0x02, 0) (正则表达式).搜索, 命令说明: "使用指定的正则表达式搜索指定文本中与该表达式匹配的子文本。注意，出于搜索效率考虑，被搜索的文本没有存放在返回的“搜索结果”中，所以在调用“搜索结果.取匹配文本”和“搜索结果.取子匹配文本”时，请注意给“被搜索的文本”传递的参数应该和对应的本命令的“欲被搜索的文本”参数相同。"
// 参数<1>: 欲被搜索的文本 SDT_TEXT, 参数说明: NULL
// 参数<2>: 起始搜索位置 SDT_INT, 参数说明: NULL
// 参数<3>: [搜索长度 SDT_INT], 参数说明: "本参数和“起始搜索位置”一起指定搜索的范围。如果本参数被省略，默认值为从“起始搜索位置”到文本结尾的长度"
REGEX_EXTERN_C void regex_Search_7_regex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

