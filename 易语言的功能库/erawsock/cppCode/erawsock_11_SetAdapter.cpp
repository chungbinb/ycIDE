#include "..\include_erawsock_header.h"

// 调用格式: SDT_BOOL (ARP协议).配置网络适配器, 命令说明: "设置网络适配器的属性。成功返回真；失败返回假。"
// 参数<1>: [网卡模式 SDT_INT], 参数说明: "可以为以下常量值之一：1、直接模式； 2、混杂模式（此模式可以监听所有流过本地主机的数据报）。"
// 参数<2>: [缓冲区大小 SDT_INT], 参数说明: "设置网卡接收数据包的缓冲区大小。"
// 参数<3>: [等待时间 SDT_INT], 参数说明: "设置接收一个数据报后的等待时间。单位为毫秒。"
ERAWSOCK_EXTERN_C void erawsock_SetAdapter_11_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

