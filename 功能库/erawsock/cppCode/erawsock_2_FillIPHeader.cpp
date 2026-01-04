#include "..\include_erawsock_header.h"

// 调用格式: _SDT_NULL (原始套接字).构造IP报头, 命令说明: "自行填充IP报头数据。"
// 参数<1>: IP报头 MAKELONG(0x03, 0), 参数说明: NULL
ERAWSOCK_EXTERN_C void erawsock_FillIPHeader_2_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

