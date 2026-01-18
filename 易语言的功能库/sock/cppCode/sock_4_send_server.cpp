#include "..\include_sock_header.h"

// 调用格式: SDT_BOOL (网络服务器).发送, 命令说明: "向指定客户端发送数据"
// 参数<1>: 客户端句柄 SDT_INT, 参数说明: "通过“网络服务器.监听()”得到的客户端句柄"
// 参数<2>: 数据 _SDT_ALL, 参数说明: "准备发送的数据"
// 参数<3>: 等待时间 SDT_INT, 参数说明: "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）"
SOCK_EXTERN_C void sock_send_server_4_sock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    PVOID    arg2 = pArgInf[2].m_pByte;
    INT      arg3 = pArgInf[3].m_int;

}

