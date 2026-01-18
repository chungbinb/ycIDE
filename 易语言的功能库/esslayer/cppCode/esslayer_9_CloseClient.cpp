#include "..\include_esslayer_header.h"

// 调用格式: SDT_BOOL (保密服务器).断开连接, 命令说明: "断开指定客户端连接"
// 参数<1>: 客户句柄 SDT_INT, 参数说明: "客户的句柄值"
ESSLAYER_EXTERN_C void esslayer_CloseClient_9_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

