#include "..\include_erawsock_header.h"

// 调用格式: _SDT_NULL (ARP协议).构造ARP头, 命令说明: "自行填充ARP头数据。"
// 参数<1>: ARP头 MAKELONG(0x07, 0), 参数说明: NULL
ERAWSOCK_EXTERN_C void erawsock_FillArpHeader_14_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

