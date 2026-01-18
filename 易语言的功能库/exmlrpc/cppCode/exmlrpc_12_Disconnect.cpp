#include "..\include_exmlrpc_header.h"

// 调用格式: _SDT_NULL (远程服务).断开连接, 命令说明: "断开与一个客户的连接"
// 参数<1>: 客户句柄 SDT_INT, 参数说明: "欲断开的客户端的句柄"
EXMLRPC_EXTERN_C void exmlrpc_Disconnect_12_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

