#include "..\include_einterprocess_header.h"

// 调用格式: _SDT_NULL 断开管道连接, 命令说明: "一个命名管道的客户断开与命名管道的连接"
// 参数<1>: 命名管道句柄 SDT_INT, 参数说明: "已经连接的命名管道句柄"
EINTERPROCESS_EXTERN_C void einterprocess_DisConnectNamedPipe_5_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

