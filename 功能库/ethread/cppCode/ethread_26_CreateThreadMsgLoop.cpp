#include "..\include_ethread_header.h"

// 调用格式: SDT_INT 创建线程消息循环, 命令说明: "创建一个线程消息循环, 返回线程ID, 可以通过线程ID向这个消息循环投递消息\r\n        本命令创建的消息循环遇到 WM_QUIT(18) 消息时就会退出消息循环"
// 参数<1>: 消息回调 SDT_SUB_PTR, 参数说明: "接收线程消息回调的函数, 整数型 线程消息回调(整数型 消息值, 整数型 参数1, 整数型 参数2), 返回值保留给后续版本升级使用"
ETHREAD_EXTERN_C void ethread_CreateThreadMsgLoop_26_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    DWORD    arg1 = pArgInf[0].m_dwSubCodeAdr;

}

