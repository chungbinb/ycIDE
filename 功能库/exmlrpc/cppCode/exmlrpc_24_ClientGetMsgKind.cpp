#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_INT (请求客户端).取消息类型, 命令说明: "取得触发客户端处理函数的消息类型。该方法必须在客户端的触发函数中调用。如果服务器关闭或者与客户的连接意外断开返回0，如果客户端接收到数据返回1，出错返回-1"
// 参数<1>: 消息地址 SDT_INT, 参数说明: "触发客户端处理函数的消息地址"
EXMLRPC_EXTERN_C void exmlrpc_ClientGetMsgKind_24_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

