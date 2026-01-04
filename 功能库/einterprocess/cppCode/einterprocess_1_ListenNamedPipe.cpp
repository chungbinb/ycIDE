#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL 监听命名管道, 命令说明: "监听一个已经创建好的命名管道，等待客户的连接，如果有客户连接则返回，没有客户连接的情况下，该方法处于“锁定”模式，不再向下执行。如果监听失败返回假"
// 参数<1>: 命名管道句柄 SDT_INT, 参数说明: "已经创建的命名管道句柄"
EINTERPROCESS_EXTERN_C void einterprocess_ListenNamedPipe_1_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

