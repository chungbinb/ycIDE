#include "..\include_erawsock_header.h"

// 调用格式: SDT_BOOL (原始套接字).创建, 命令说明: "将创建一个用于访问指定协议报文的原始套接字。成功返回真，失败返回假。"
// 参数<1>: 协议类型 SDT_INT, 参数说明: "用来指明所要处理的协议类型。"
ERAWSOCK_EXTERN_C void erawsock_CreateSocket_0_erawsock(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

