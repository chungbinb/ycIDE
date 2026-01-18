#include "..\include_sock_header.h"

// 调用格式: SDT_BOOL (网络客户端).连接, 命令说明: "连接到网络服务器,成功真，失败返回假"
// 参数<1>: IP地址 SDT_TEXT, 参数说明: "待连接服务器的IP地址"
// 参数<2>: 端口 SDT_INT, 参数说明: "待连接服务器的端口"
SOCK_EXTERN_C void sock_connect_client_8_sock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;

}

