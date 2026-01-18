#include "..\include_ethread_header.h"

// 调用格式: SDT_INT 挂起线程, 命令说明: "将指定线程的执行挂起,注意，同一个线程可以被挂起多次，如果挂起多次，就要恢复多次"
// 参数<1>: 线程句柄 SDT_INT, 参数说明: "可通过\"启动线程\" 或者 \"启动线程Ex\" 或者 \"打开线程\" 获得的线程句柄"
ETHREAD_EXTERN_C void ethread_SuspendThread_23_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

