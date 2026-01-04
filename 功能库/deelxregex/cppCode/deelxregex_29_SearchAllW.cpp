#include "..\include_deelxregex_header.h"

// 调用格式: MAKELONG(0x02, 0) (正则表达式DEELX).[]搜索全部W, 命令说明: "使用指定的正则表达式搜索指定文本中与该表达式匹配的所有子文本。返回值包含所有的搜索结果的一维数组，数组原有内容将被销毁，维数也将根据需要做相应调整。本命令的内部是通过循环多次调用“搜索”，每次指定适当的参数，来实现搜索整个文本的。返回值数组的各个成员分别对应每次调用“搜索”的返回值。本命令可高效地一次性取得目标文本中所有的匹配子文本信息。"
// 参数<1>: [用来匹配的文本 SDT_BIN], 参数说明: "Unicode格式，进行匹配的字符串"
// 参数<2>: [开始查找匹配的位置 SDT_INT], 参数说明: "开始查找匹配的位置."
DEELXREGEX_EXTERN_C void deelxregex_SearchAllW_29_deelxregex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    INT      arg2 = pArgInf[2].m_int;

}

