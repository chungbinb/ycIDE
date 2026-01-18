#include "..\include_sock_header.h"

// 调用格式: SDT_BOOL (网络服务器).断开连接, 命令说明: "断开指定客户端连接"
// 参数<1>: 客户端句柄 SDT_INT, 参数说明: "通过“网络服务器.监听()”得到的客户端句柄"
SOCK_EXTERN_C void sock_close_client_5_sock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

