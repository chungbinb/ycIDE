#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_INT (远程服务).取消息类型, 命令说明: "取得触发服务端处理函数的消息类型。有客户连接到服务端返回0，有客户主动或意外断开与服务端的连接返回1，客户端发送字节流到服务器返回2，出错返回-1"
// 参数<1>: 消息地址 SDT_INT, 参数说明: "触发服务器端处理函数的消息地址"
EXMLRPC_EXTERN_C void exmlrpc_GetMsgKind_27_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

