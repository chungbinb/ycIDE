#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速文本对象).到大写, 命令说明: "将本对象文本中的全部或部分英文字母变为大写字母。"
// 参数<1>: [起始字符位置 SDT_INT], 参数说明: "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。"
// 参数<2>: [字符个数 SDT_INT], 参数说明: "如果本参数被省略，默认为从参数“起始字符位置”开始到文本结尾的字符个数。"
COMMOBJ_EXTERN_C void commobj_UpperCase_45_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

