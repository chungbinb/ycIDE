#include "..\include_erawsock_header.h"

// 调用格式: SDT_BOOL (ARP协议).打开网络适配器, 命令说明: "打开指定的网络适配器。成功返回真；失败返回假。"
// 参数<1>: [网卡序号 SDT_INT], 参数说明: "要打开的网卡序号。"
ERAWSOCK_EXTERN_C void erawsock_OpenAdapter_10_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

