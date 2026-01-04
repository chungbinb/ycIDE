#include "..\include_esslayer_header.h"

// 调用格式: SDT_BOOL (保密客户端).取回数据, 命令说明: "取回服务程序发送到客户端的数据。该方法必须在客户程序的“处理函数”中使用。使用本方法之前首先应该通过“取消息代码”方法得到消息的代码，如果消息代码等于2再调用本方法得到服务程序发送到客户端的数据。如果成功取回数据返回真，否则返回假"
// 参数<1>: &数据 SDT_BIN, 参数说明: "服务器发送到客户端的数据"
ESSLAYER_EXTERN_C void esslayer_recv_client_20_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE*  arg1 = pArgInf[1].m_ppBin;

}

