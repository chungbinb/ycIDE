#include "..\include_ethread_header.h"

// 调用格式: SDT_BOOL 投递线程消息, 命令说明: "向有消息循环的线程投递一条消息, 成功返回真, 失败返回假, 返回假时可以调用 GetLastError 获取错误码\r\n        如果投递 WM_QUIT(18) 消息, 则 \"创建线程消息循环\" 创建的消息循环会退出循环\r\n    【注意】如果向其他进程投递线程消息则会有权限的问题, 如果返回假, 并且 GetLastError() 返回5, 那就是当前进程的权限不支持投递消息到目标进程"
// 参数<1>: 线程ID SDT_INT, 参数说明: "创建线程消息循环 返回的值"
// 参数<2>: 消息值 SDT_INT, 参数说明: "消息值的限制请查看MSDN, 大于65535的消息会投递失败"
// 参数<3>: [参数1 SDT_INT], 参数说明: "投递到消息队列里的参数1"
// 参数<4>: [参数2 SDT_INT], 参数说明: "投递到消息队列里的参数2"
ETHREAD_EXTERN_C void ethread_PostThreadMsg_28_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;

}

