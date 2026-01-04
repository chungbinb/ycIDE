#include "..\include_sock_header.h"

// 调用格式: SDT_BOOL (网络服务器).启动, 命令说明: "启动网络服务器，绑定到指定端口，成功真，失败返回假"
// 参数<1>: 端口 SDT_INT, 参数说明: "服务器的端口"
SOCK_EXTERN_C void sock_start_server_0_sock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

