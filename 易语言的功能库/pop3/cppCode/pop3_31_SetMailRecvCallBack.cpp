#include "..\include_pop3_header.h"

// 调用格式: SDT_BOOL 注册邮件接收回调函数, 命令说明: "注册一个回调函数，主要用于接收邮件时的进度显示。注：此回调函数中不要进行耗时的操作，否则邮件服务器会断开!不要调用邮件接收支持库中和服务器交互的命令，否则可能出现死循环!"
// 参数<1>: 函数地址 SDT_SUB_PTR, 参数说明: "本参数表示函数的地址，是易语言定义的子程序的地址。为0表示取消注册的回调函数。子程序的参数第一个为“整数型”表示接收邮件命令类型值，是“命令类型”数据类型中的常量值；参数第二个为“整数型”表示接收第几封邮件；参数第三个为“整数型”表示本次接收到的数据量单位为字节。子程序返回值为逻辑型。返回真表示继续接收此邮件，返回假表示不接收此邮件。"
POP3_EXTERN_C void pop3_SetMailRecvCallBack_31_pop3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    DWORD    arg1 = pArgInf[0].m_dwSubCodeAdr;

}

