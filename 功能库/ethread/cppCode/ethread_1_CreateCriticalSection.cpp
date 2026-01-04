#include "..\include_ethread_header.h"

// 调用格式: SDT_INT 创建进入许可证, 命令说明: "创建并返回一个进入许可证数值，此许可证值用作进入程序中的指定许可代码区，以避免多线程冲突。成功返回非零整数值，失败返回0。所创建的许可证在不再使用后，必须使用“删除进入许可证”命令将其删除"
// 无参数
ETHREAD_EXTERN_C void ethread_CreateCriticalSection_1_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

