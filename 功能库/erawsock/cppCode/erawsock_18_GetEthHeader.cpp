#include "..\include_erawsock_header.h"

// 调用格式: _SDT_NULL (ARP协议).取以太网头, 命令说明: "从指定的字节集中取得以太网头信息。"
// 参数<1>: 数据 SDT_BIN, 参数说明: "待取得报头信息的数据。"
// 参数<2>: 以太网头 MAKELONG(0x06, 0), 参数说明: "待取得的以太网头。"
ERAWSOCK_EXTERN_C void erawsock_GetEthHeader_18_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

