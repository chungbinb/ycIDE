#include "..\include_erawsock_header.h"

// 调用格式: SDT_BIN (原始套接字).接收, 命令说明: "接收所有通过的数据包。出错返回空字节集，每次接受的最大长度为20480个字节。"
// 参数<1>: [等待时间 SDT_INT], 参数说明: "等待接收的时间范围。单位为毫秒。"
ERAWSOCK_EXTERN_C void erawsock_recvfrom_5_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

