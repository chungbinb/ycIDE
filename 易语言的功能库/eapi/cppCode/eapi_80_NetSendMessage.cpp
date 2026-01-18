#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 发送局域网消息, 命令说明: "发送消息到指定的机器（即Windows信使服务，只有启动了Messenger服务的机器才能收到该命令发送的消息）。成功返回真；失败返回假。"
// 参数<1>: [发送目标 SDT_TEXT], 参数说明: "将要接收消息的目标机器，可以为机器名、IP地址、工作组名（具体由目标类型决定）"
// 参数<2>: 消息 SDT_TEXT, 参数说明: "将要发送的消息文本"
// 参数<3>: [保留参数 SDT_INT], 参数说明: NULL
EAPI_EXTERN_C void eapi_NetSendMessage_80_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    INT      arg3 = pArgInf[2].m_int;

}

