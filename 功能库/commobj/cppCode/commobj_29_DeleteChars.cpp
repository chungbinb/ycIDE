#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速文本对象).删除字符, 命令说明: "删除文本中从指定字符开始的指定个数的字符（包括起始删除位置处的字符）"
// 参数<1>: 起始删除位置 SDT_INT, 参数说明: "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。"
// 参数<2>: 欲删除的字符数 SDT_INT, 参数说明: "如果参数值为-1，表示删除从“起始删除位置”开始到文本结尾的所有字符。"
COMMOBJ_EXTERN_C void commobj_DeleteChars_29_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

