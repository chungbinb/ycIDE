#include "..\include_ethread_header.h"

// 调用格式: SDT_BOOL 删除进入许可证, 命令说明: "删除由“创建进入许可证”命令所创建返回的进入许可证。成功返回真，失败返回假"
// 参数<1>: 进入许可证 SDT_INT, 参数说明: "本参数应提供由“创建进入许可证”命令所创建并返回的数值"
ETHREAD_EXTERN_C void ethread_DeleteCriticalSection_2_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

