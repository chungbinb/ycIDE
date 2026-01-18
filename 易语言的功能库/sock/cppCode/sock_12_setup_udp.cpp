#include "..\include_sock_header.h"

// 调用格式: SDT_BOOL (网络数据报).配置, 命令说明: "配置网络数据报,成功返回真，失败返回假"
// 参数<1>: 端口 SDT_INT, 参数说明: "数据报服务器的端口"
SOCK_EXTERN_C void sock_setup_udp_12_sock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

