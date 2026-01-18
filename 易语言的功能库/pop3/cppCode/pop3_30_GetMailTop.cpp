#include "..\include_pop3_header.h"

// 调用格式: MAKELONG(0x01, 0) 接收邮件前几行, 命令说明: "从服务器上获取邮件前几行的内容。"
// 参数<1>: 第几封 SDT_INT, 参数说明: "指定是第几封邮件。"
// 参数<2>: 共几行 SDT_INT, 参数说明: "指定要获取邮件的前几行内容。"
POP3_EXTERN_C void pop3_GetMailTop_30_pop3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

