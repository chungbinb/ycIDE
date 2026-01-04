#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_BOOL (远程服务).取客户数组, 命令说明: "取得当前连接到服务器的所有客户端的句柄。该方法执行成功返回真，执行失败返回假"
// 参数<1>: &客户句柄数组 数组 SDT_INT, 参数说明: "连接到当前的服务程序的所有客户端的句柄数组，该参数作为“取客户数组”的返回值"
EXMLRPC_EXTERN_C void exmlrpc_GetAllClient_11_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppAryData;

}

