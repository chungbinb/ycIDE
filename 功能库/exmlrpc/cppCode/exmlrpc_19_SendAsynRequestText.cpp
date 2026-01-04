#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_BOOL (请求客户端).异步发送文本, 命令说明: "客户端发送异步请求文本到服务器。该方法执行成功返回真，执行失败返回假"
// 参数<1>: 请求信息 SDT_TEXT, 参数说明: "客户端发送的请求信息的内容"
EXMLRPC_EXTERN_C void exmlrpc_SendAsynRequestText_19_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

