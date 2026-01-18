#include "..\include_ethread_header.h"

// 调用格式: SDT_INT 打开线程, 命令说明: "打开一个线程, 返回打开的线程句柄, 不使用这个句柄时需要调用\"关闭线程句柄\"释放这个句柄"
// 参数<1>: 线程ID SDT_INT, 参数说明: "启动线程Ex/创建线程消息循环 返回的值"
ETHREAD_EXTERN_C void ethread_OpenThread_25_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

