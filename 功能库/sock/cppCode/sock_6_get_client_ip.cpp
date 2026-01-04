#include "..\include_sock_header.h"

// 调用格式: SDT_TEXT (网络服务器).取客户IP, 命令说明: "取客户端IP,成功返回客户端IP，失败返回空字符串"
// 参数<1>: 客户端句柄 SDT_INT, 参数说明: "通过“网络服务器.监听()”得到的客户端句柄"
SOCK_EXTERN_C void sock_get_client_ip_6_sock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

