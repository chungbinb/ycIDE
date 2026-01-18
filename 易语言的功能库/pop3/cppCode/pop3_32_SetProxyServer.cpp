#include "..\include_pop3_header.h"

// 调用格式: SDT_BOOL 设置代理服务器, 命令说明: "设置代理服务器信息。注：此方法要在“连接收信服务器”命令之前调用。"
// 参数<1>: 类型 SDT_INT, 参数说明: "表示代理服务器的类型，0：不用代理，1：HTTP1.1代理，2：SOCKS5代理。"
// 参数<2>: 地址 SDT_TEXT, 参数说明: "表示代理服务器的IP地址。"
// 参数<3>: 端口 SDT_INT, 参数说明: "表示代理服务器的端口。"
// 参数<4>: 用户名 SDT_TEXT, 参数说明: "表示代理服务器需要验证的用户名。"
// 参数<5>: 口令 SDT_TEXT, 参数说明: "表示代理服务器需要验证的口令。"
POP3_EXTERN_C void pop3_SetProxyServer_32_pop3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    INT      arg3 = pArgInf[2].m_int;
    LPSTR    arg4 = pArgInf[3].m_pText;
    LPSTR    arg5 = pArgInf[4].m_pText;

}

