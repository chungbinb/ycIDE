#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_BOOL (请求客户端).取返回字节集, 命令说明: "在异步的请求方式下，取得服务器返回的结果。该方法必须在客户端的触发函数中调用。如果正确取得服务器的返回结果，该方法返回真，否则，该方法返回假"
// 参数<1>: 消息地址 SDT_INT, 参数说明: "触发客户端处理函数的信息的地址。“取返回字节集”方法必须在用户定义的“接收函数”中调用，所以该值就是“接收函数”的参数值"
// 参数<2>: &结果信息 SDT_BIN, 参数说明: "客户端得到的服务器返回的结果信息，该参数作为方法“取返回字节集”的返回值"
EXMLRPC_EXTERN_C void exmlrpc_GetResultBin_22_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPBYTE*  arg2 = pArgInf[2].m_ppBin;

}

