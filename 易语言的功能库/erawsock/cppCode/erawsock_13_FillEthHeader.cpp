#include "..\include_erawsock_header.h"

// 调用格式: _SDT_NULL (ARP协议).构造以太网头, 命令说明: "自行填充以太网头数据。"
// 参数<1>: 以太网头 MAKELONG(0x06, 0), 参数说明: "待取得的以太网头。"
ERAWSOCK_EXTERN_C void erawsock_FillEthHeader_13_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

