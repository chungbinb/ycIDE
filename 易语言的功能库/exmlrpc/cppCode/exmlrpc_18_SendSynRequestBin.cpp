#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_INT (请求客户端).同步发送字节集, 命令说明: "在客户端发送同步请求字节集到服务器，当收到服务器返回的结果后，该方法才返回，否则为阻塞状态直到超时。如果该方法正确执行并接收到服务器返回的结果，该方法返回1。如果在超时时间内没能接收到服务器的返回结果，该方法返回0。如果出现其他错误没能接收到服务器的返回结果，该方法返回-1"
// 参数<1>: 请求字节集 SDT_BIN, 参数说明: "客户端发送的请求字节集的内容"
// 参数<2>: &结果字节集 SDT_BIN, 参数说明: "服务器返回的结果内容"
// 参数<3>: 超时 SDT_INT, 参数说明: "同步机制下“发送请求”方法的最大允许等待时间。单位为毫秒。该值必须大于等于0"
EXMLRPC_EXTERN_C void exmlrpc_SendSynRequestBin_18_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    LPBYTE*  arg2 = pArgInf[2].m_ppBin;
    INT      arg3 = pArgInf[3].m_int;

}

