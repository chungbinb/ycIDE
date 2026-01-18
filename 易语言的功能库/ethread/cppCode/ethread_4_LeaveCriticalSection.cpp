#include "..\include_ethread_header.h"

// 调用格式: _SDT_NULL 退出许可区, 命令说明: "指示当前线程将退出许可代码区，并允许其它使用同一进入许可证的线程进入此许可代码区"
// 参数<1>: 进入许可证 SDT_INT, 参数说明: "本参数应提供由“创建进入许可证”命令所创建并返回的数值"
ETHREAD_EXTERN_C void ethread_LeaveCriticalSection_4_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

