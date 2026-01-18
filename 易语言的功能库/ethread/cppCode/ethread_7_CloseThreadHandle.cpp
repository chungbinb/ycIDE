#include "..\include_ethread_header.h"

// 调用格式: SDT_BOOL 关闭线程句柄, 命令说明: "返回真表示已成功关闭线程句柄。在Linux下，如果线程已经结束，本命令可能返回假。"
// 参数<1>: 线程句柄 SDT_INT, 参数说明: "可通过“启动线程”的第三个参数获取线程句柄。"
ETHREAD_EXTERN_C void ethread_CloseThreadHandle_7_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

