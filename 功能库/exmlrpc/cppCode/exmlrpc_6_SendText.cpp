#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_BOOL (远程服务).发送文本, 命令说明: "服务程序发送文本数据到客户端。该方法执行成功返回真，失败返回假"
// 参数<1>: 目的端 _SDT_ALL, 参数说明: "该参数可以接受两种不同的数据，一个是“请求代码”，一个是“客户句柄”，这两种数据都可以作为服务程序发送数据的目的端，当参数为“请求代码”时，发送的目的端就是该请求代码的来源"
// 参数<2>: 发送信息 SDT_TEXT, 参数说明: "服务器所要发送的文本形式的信息"
EXMLRPC_EXTERN_C void exmlrpc_SendText_6_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

