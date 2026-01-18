#include "..\include_commobj_header.h"

// 调用格式: SDT_INT (快速文本对象).替换子文本, 命令说明: "在本对象文本中查找指定文本，并将其替换为其它文本。返回替换成功次数。"
// 参数<1>: 被替换文本 SDT_TEXT, 参数说明: NULL
// 参数<2>: [替换为文本 SDT_TEXT], 参数说明: "如果本参数被省略，则表示将删除被替换文本。"
// 参数<3>: [起始替换位置 SDT_INT], 参数说明: "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。"
// 参数<4>: [最大替换次数 SDT_INT], 参数说明: "如果为-1，表示不限制替换次数。如果本参数被省略，默认为-1。"
// 参数<5>: 是否区分大小写 SDT_BOOL, 参数说明: NULL
COMMOBJ_EXTERN_C void commobj_ReplaceSubText_37_commobj(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;
    BOOL     arg5 = pArgInf[5].m_bool;

}

