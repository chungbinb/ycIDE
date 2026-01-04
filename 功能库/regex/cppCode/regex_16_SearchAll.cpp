#include "..\include_regex_header.h"

// 调用格式: MAKELONG(0x02, 0) (正则表达式).[]搜索全部, 命令说明: "使用指定的正则表达式搜索指定文本中与该表达式匹配的所有子文本。返回值包含所有的搜索结果的一维数组，数组原有内容将被销毁，维数也将根据需要做相应调整。本命令的内部是通过循环多次调用“搜索”，每次指定适当的参数，来实现搜索整个文本的。返回值数组的各个成员分别对应每次调用“搜索”的返回值。本命令可高效地一次性取得目标文本中所有的匹配子文本信息"
// 参数<1>: 欲被搜索的文本 SDT_TEXT, 参数说明: NULL
REGEX_EXTERN_C void regex_SearchAll_16_regex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

