#include "..\include_ethread_header.h"

// 调用格式: SDT_BOOL (线程池).销毁, 命令说明: "销毁线程池, 根据传递的参数决定如何销毁, 默认是等待所有任务结束"
// 参数<1>: [销毁模式 SDT_INT], 参数说明: "1=让任务线程自生自灭, 2=挂起所有任务, 永远不唤醒, 其他=等待任务结束"
ETHREAD_EXTERN_C void ethread_ThreadPool_Destroy_13_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

