#include "..\include_internet_header.h"

// 调用格式: SDT_BOOL 连接发信服务器, 命令说明: "连接到指定的SMTP邮件发送服务器，成功返回真，失败返回假"
// 参数<1>: 发信邮件服务器地址 SDT_TEXT, 参数说明: "指定用作发送邮件的SMTP邮件服务器地址"
// 参数<2>: [端口号 SDT_INT], 参数说明: "指定欲连接到SMTP邮件服务器上的端口号，如果本参数被省略，默认值为标准 25 端口"
// 参数<3>: [用户名 SDT_TEXT], 参数说明: "指定在SMTP邮件服务器上的用户帐号名称，如果本参数被省略，默认值为空文本，即无需登录"
// 参数<4>: [密码 SDT_TEXT], 参数说明: "指定在SMTP邮件服务器上的用户帐号密码，如果本参数被省略，默认值为空文本，即无需登录"
// 参数<5>: [最长等待时间 SDT_INT], 参数说明: "指定在发信过程中等待SMTP邮件服务器响应的最大时间，单位为毫秒，如果本参数被省略，默认值为 2000 毫秒"
INTERNET_EXTERN_C void internet_ConnectSmtpServer_0_internet(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    INT      arg5 = pArgInf[4].m_int;

}

