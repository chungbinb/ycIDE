#include "..\include_ethread_header.h"

// 调用格式: SDT_BOOL 取信息, 命令说明: "获取线程池基础信息"
// 参数<1>: &接收信息变量 MAKELONG(0x02, 0), 参数说明: "接收线程池信息的结构变量"
ETHREAD_EXTERN_C void ethread_ThreadPool_GetInfo_31_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

