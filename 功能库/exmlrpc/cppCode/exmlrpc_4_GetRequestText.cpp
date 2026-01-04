#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_BOOL (远程服务).取请求文本, 命令说明: "取得客户发出的请求信息文本。如果正确得到客户的请求信息，该方法返回真，否则返回假"
// 参数<1>: 消息地址 SDT_INT, 参数说明: "客户端发送过来的消息地址。“取请求文本”方法必须在用户定义的“处理函数”中调用，所以该值就是“处理函数”的参数值"
// 参数<2>: &请求代码 SDT_TEXT, 参数说明: "服务器所接收的请求信息来源，该代码唯一标识客户端的一次请求，禁止修改它的值。该参数作为方法“取请求文本”的返回值"
// 参数<3>: &请求信息 SDT_TEXT, 参数说明: "服务器所接收到的请求信息的详细内容。该参数作为方法“取请求文本”的返回值"
EXMLRPC_EXTERN_C void exmlrpc_GetRequestText_4_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR*   arg2 = pArgInf[2].m_ppText;
    LPSTR*   arg3 = pArgInf[3].m_ppText;

}

