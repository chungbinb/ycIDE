#include "..\include_ethread_header.h"

// 调用格式: _SDT_NULL 进入许可区, 命令说明: "根据已经创建的许可证进入指定许可代码区，在此线程未退出之前，其它线程如要通过同一个进入许可证进入该许可代码区则必须先等待此线程退出许可代码区，从而保证了指定许可代码区在任何时候都只能有一条线程进入并执行"
// 参数<1>: 进入许可证 SDT_INT, 参数说明: "本参数应提供由“创建进入许可证”命令所创建并返回的数值"
ETHREAD_EXTERN_C void ethread_EnterCriticalSection_3_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

