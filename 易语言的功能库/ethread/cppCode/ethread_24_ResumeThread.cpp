#include "..\include_ethread_header.h"

// 调用格式: SDT_INT 唤醒线程, 命令说明: "唤醒挂起线程"
// 参数<1>: 线程句柄 SDT_INT, 参数说明: "可通过\"启动线程\" 或者 \"启动线程Ex\" 或者 \"打开线程\" 获得的线程句柄"
ETHREAD_EXTERN_C void ethread_ResumeThread_24_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

