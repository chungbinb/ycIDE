#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_BOOL (请求客户端).连接, 命令说明: "连接到指定地址和端口号的服务器。连接成功返回真，连接失败返回假"
// 参数<1>: 端口号 SDT_INT, 参数说明: "本参数作为欲连接的服务器的端口号，该值的取值范围在0到65535之间"
// 参数<2>: 服务器地址 SDT_TEXT, 参数说明: "本参数是欲连接的服务器的IP地址，也可以是服务器的域名或者是内网的计算机名"
// 参数<3>: 请求方式 SDT_BOOL, 参数说明: "本参数作为客户端的请求方式，该值为真代表客户端采用同步的请求方式，这时忽略该方法的最后一个参数，该值为假代表客户端采用异步的请求方式。同步方式不能发送异步请求，同样异步方式不能发送同步请求"
// 参数<4>: [处理函数 SDT_SUB_PTR], 参数说明: "同步请求的方式下，该参数没有意义可以省略。异步请求模式下如果客户端接收到数据或者与服务器的连接被断开，该函数就会被系统调用。该函数有一个整形的参数，该参数代表触发该处理函数的消息地址，客户端可以通过该地址得到消息的类型和内容"
EXMLRPC_EXTERN_C void exmlrpc_ClientConnect_15_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR    arg2 = pArgInf[2].m_pText;
    BOOL     arg3 = pArgInf[3].m_bool;
    DWORD    arg4 = pArgInf[4].m_dwSubCodeAdr;

}

