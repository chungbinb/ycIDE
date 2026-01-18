#include "..\include_esslayer_header.h"

// 调用格式: SDT_BOOL (保密客户端).发送数据, 命令说明: "向指定服务器发送数据"
// 参数<1>: 数据 _SDT_ALL, 参数说明: "准备发送的数据"
ESSLAYER_EXTERN_C void esslayer_send_client_21_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;

}

