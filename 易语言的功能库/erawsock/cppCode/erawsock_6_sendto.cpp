#include "..\include_erawsock_header.h"

// 调用格式: SDT_BOOL (原始套接字).发送, 命令说明: "向指定的位置发送原始套接字数据报。"
// 参数<1>: 对方IP SDT_TEXT, 参数说明: "接收方的IP信息。"
// 参数<2>: [对方端口 SDT_INT], 参数说明: "接收方的端口信息。"
// 参数<3>: [等待时间 SDT_INT], 参数说明: "等待发送的时间范围。单位为毫秒。"
// 参数<4>: [数据 _SDT_ALL], 参数说明: "准备发送的数据。"
ERAWSOCK_EXTERN_C void erawsock_sendto_6_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;
    PVOID    arg4 = pArgInf[4].m_pByte;

}

