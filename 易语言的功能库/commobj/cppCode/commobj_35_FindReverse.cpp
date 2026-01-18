#include "..\include_commobj_header.h"

// 调用格式: SDT_INT (快速文本对象).倒找文本, 命令说明: "在本对象文本中从“起始寻找位置”开始向文本开头方向寻找指定文本第一次出现时的位置索引（相对于文本头），未找到则返回-1。"
// 参数<1>: 欲寻找的文本 SDT_TEXT, 参数说明: NULL
// 参数<2>: [起始寻找位置 SDT_INT], 参数说明: "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。"
// 参数<3>: 是否区分大小写 SDT_BOOL, 参数说明: NULL
COMMOBJ_EXTERN_C void commobj_FindReverse_35_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;
    BOOL     arg3 = pArgInf[3].m_bool;

}

