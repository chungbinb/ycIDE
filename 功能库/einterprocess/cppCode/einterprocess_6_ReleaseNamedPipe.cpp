#include "..\include_einterprocess_header.h"

// 调用格式: _SDT_NULL 关闭命名管道, 命令说明: "关闭并释放已经创建的命名管道。如果调用了“监听命名管道”，必须等待“监听命名管道”方法执行完毕后才能执行该方法，否则该方法执行过程中处于“锁定”状态"
// 参数<1>: 命名管道句柄 SDT_INT, 参数说明: "已经创建的命名管道句柄"
EINTERPROCESS_EXTERN_C void einterprocess_ReleaseNamedPipe_6_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

