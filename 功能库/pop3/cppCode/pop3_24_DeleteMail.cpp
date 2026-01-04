#include "..\include_pop3_header.h"

// 调用格式: SDT_BOOL 删除邮件, 命令说明: "将服务器上某封邮件标记为删除"
// 参数<1>: 第几封 SDT_INT, 参数说明: "表示删除第几封邮件。"
POP3_EXTERN_C void pop3_DeleteMail_24_pop3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

