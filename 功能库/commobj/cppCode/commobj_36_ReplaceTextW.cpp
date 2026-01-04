#include "..\include_commobj_header.h"

// 调用格式: SDT_BOOL (快速文本对象).替换文本, 命令说明: NULL
// 参数<1>: 起始替换位置 SDT_INT, 参数说明: "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。"
// 参数<2>: 替换长度 SDT_INT, 参数说明: NULL
// 参数<3>: [替换为文本 SDT_TEXT], 参数说明: "如果本参数被省略，则删除文本中的指定部分。"
COMMOBJ_EXTERN_C void commobj_ReplaceTextW_36_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    LPSTR    arg3 = pArgInf[3].m_pText;

}

