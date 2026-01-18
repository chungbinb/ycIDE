#include "..\include_pop3_header.h"

// 调用格式: SDT_BOOL 连接收信服务器, 命令说明: "连接到指定的POP3邮件接收服务器，成功返回真，失败返回假"
// 参数<1>: 收信邮件服务器地址 SDT_TEXT, 参数说明: "指定用作接收邮件的POP3邮件服务器地址。"
// 参数<2>: 端口号 SDT_INT, 参数说明: "指定欲连接到POP3邮件服务器上的端口号，如果本参数被省略，默认值为标准 110 端口。"
// 参数<3>: 用户名 SDT_TEXT, 参数说明: "指定在POP3邮件服务器上的用户帐号名称。"
// 参数<4>: 密码 SDT_TEXT, 参数说明: "指定在POP3邮件服务器上的用户帐号密码。"
// 参数<5>: 最长等待时间 SDT_INT, 参数说明: "指定在收信过程中等待POP3邮件服务器响应的最大时间，单位为毫秒。"
// 参数<6>: 重试次数 SDT_INT, 参数说明: "指定在验证阶段，如果某一项(如：用户名或口令)验证失败后重试的次数。如果本参数被省略，默认值为 3 次。"
POP3_EXTERN_C void pop3_ConnectPop3Server_10_pop3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    INT      arg5 = pArgInf[4].m_int;
    INT      arg6 = pArgInf[5].m_int;

}

