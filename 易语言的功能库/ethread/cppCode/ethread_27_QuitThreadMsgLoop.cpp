#include "..\include_ethread_header.h"

// 调用格式: SDT_INT 退出线程消息循环, 命令说明: "退出线程消息循环, 只能退出由 \"创建线程消息循环\" 创建的消息循环\r\n        调用本命令是给消息循环的线程投递一条 WM_QUIT(18) 的消息\r\n        \"创建线程消息循环\" 创建的消息循环遇到 WM_QUIT(18) 消息时就会退出消息循环"
// 参数<1>: 线程ID SDT_INT, 参数说明: "创建线程消息循环 返回的值"
ETHREAD_EXTERN_C void ethread_QuitThreadMsgLoop_27_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

