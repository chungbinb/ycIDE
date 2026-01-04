#include "..\include_erawsock_header.h"

// 调用格式: SDT_BOOL (ARP协议).发送, 命令说明: "发送指定的ARP包。成功返回真；失败返回假。"
// 参数<1>: [发送次数 SDT_INT], 参数说明: "在发送一个ARP包时，重复发送的次数。"
ERAWSOCK_EXTERN_C void erawsock_SendArpPacket_15_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

