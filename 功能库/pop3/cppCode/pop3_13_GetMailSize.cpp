#include "..\include_pop3_header.h"

// 调用格式: SDT_BOOL 获取邮件大小, 命令说明: "获取每一封邮件的大小。"
// 参数<1>: 第几封 SDT_INT, 参数说明: "指定是第几封邮件,如果此值为-1就表示获取每一封的大小。"
// 参数<2>: &大小 数组/非数组 SDT_INT, 参数说明: "返回一封或多封邮件的大小。如果为此参数提供数组变量，而参数“第几封”不为-1，则指定邮件的大小将写入数组第一个成员中。"
// 参数<3>: [总数 SDT_INT], 参数说明: "本参数在“第几封”为-1时有效,为“获取邮件信息”方法返回的邮件总数。如果被省略，则自动获取邮件总数。"
POP3_EXTERN_C void pop3_GetMailSize_13_pop3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void**   arg2 = pArgInf[1].m_ppAryData;
    INT      arg3 = pArgInf[2].m_int;

}

