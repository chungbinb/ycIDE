#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_BOOL (远程服务).取客户IP, 命令说明: "根据客户的句柄值取得客户的IP地址。成功取得客户的IP地址，返回真，如果客户已经不可用或其他意外情况发生，返回假"
// 参数<1>: 客户句柄 SDT_INT, 参数说明: "客户端的句柄值"
// 参数<2>: &客户IP地址 SDT_TEXT, 参数说明: "客户端的IP地址。该参数作为方法“取客户IP”的返回值"
EXMLRPC_EXTERN_C void exmlrpc_GetClientIPAddr_9_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR*   arg2 = pArgInf[2].m_ppText;

}

