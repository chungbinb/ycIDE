#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_BOOL (远程服务).取客户句柄, 命令说明: "根据客户的请求信息地址取得客户的句柄。成功取得客户句柄，返回真，如果客户已经不可用或其他意外情况发生，返回假"
// 参数<1>: 消息地址 SDT_INT, 参数说明: "客户端发送过来的消息地址。“取客户句柄”方法必须在用户定义的“处理函数”中调用，所以该值就是“处理函数”的参数值"
// 参数<2>: &客户句柄 SDT_INT, 参数说明: "服务器所接收到的信息来源的客户句柄。该参数作为方法“取客户句柄”的返回值"
EXMLRPC_EXTERN_C void exmlrpc_GetClientHandle_8_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    PINT     arg2 = pArgInf[2].m_pInt;

}

