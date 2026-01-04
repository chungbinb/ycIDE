#include "..\include_esslayer_header.h"

// 调用格式: SDT_BOOL (保密服务器).取回数据, 命令说明: "取得到达服务程序的数据。该方法必须在服务程序的“处理函数”中使用。使用本方法之前首先应该通过“取消息代码”方法得到消息的代码，如果消息代码等于3再调用本方法得到客户端发送到服务器的数据。如果成功取回数据返回真，否则返回假"
// 参数<1>: &数据 SDT_BIN, 参数说明: "客户端发送到服务器的数据"
ESSLAYER_EXTERN_C void esslayer_ServerGetMsgData_8_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE*  arg1 = pArgInf[1].m_ppBin;

}

