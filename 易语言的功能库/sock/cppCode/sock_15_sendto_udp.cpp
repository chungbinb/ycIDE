#include "..\include_sock_header.h"

// 调用格式: SDT_BOOL (网络数据报).发送, 命令说明: "向指定网络数据报发送数据"
// 参数<1>: 对方信息 MAKELONG(0x04, 0), 参数说明: "接收方IP及端口信息"
// 参数<2>: 数据 _SDT_ALL, 参数说明: "准备发送的数据"
// 参数<3>: 等待时间 SDT_INT, 参数说明: "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）"
SOCK_EXTERN_C void sock_sendto_udp_15_sock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    PVOID    arg2 = pArgInf[2].m_pByte;
    INT      arg3 = pArgInf[3].m_int;

}

