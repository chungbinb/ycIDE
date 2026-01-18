#include "..\include_sock_header.h"

// 调用格式: SDT_INT (网络服务器).监听, 命令说明: "监听一个端口，等待一个客户端连接，如果设置为无限等待则有连接返回客户端句柄，如果设置等待时间，在等待时间内无连接则返回0，在等待时间之内有连接则返回客户端句柄,出错返回-1"
// 参数<1>: 等待时间 SDT_INT, 参数说明: "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）"
SOCK_EXTERN_C void sock_accept_server_2_sock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

