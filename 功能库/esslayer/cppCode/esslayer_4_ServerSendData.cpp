#include "..\include_esslayer_header.h"

// 调用格式: SDT_BOOL (保密服务器).发送数据, 命令说明: "向指定客户端发送数据，该方法执行成功返回真，失败返回假"
// 参数<1>: 客户端句柄 SDT_INT, 参数说明: "欲发送到客户端的句柄"
// 参数<2>: 数据 _SDT_ALL, 参数说明: "准备发送的数据"
ESSLAYER_EXTERN_C void esslayer_ServerSendData_4_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    PVOID    arg2 = pArgInf[2].m_pByte;

}

