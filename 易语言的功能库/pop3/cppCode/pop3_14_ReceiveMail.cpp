#include "..\include_pop3_header.h"

// 调用格式: MAKELONG(0x01, 0) 接收邮件, 命令说明: "返回“邮件信息”数据类型。"
// 参数<1>: 第几封 SDT_INT, 参数说明: "表示接收第几封邮件。"
POP3_EXTERN_C void pop3_ReceiveMail_14_pop3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

