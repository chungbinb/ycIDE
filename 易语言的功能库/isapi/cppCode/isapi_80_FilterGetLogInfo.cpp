#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网筛选器).取日志信息, 命令说明: "获取日志信息，通过变量参数返回。仅当“取事件类型()”返回值为 #筛选器事件.记录日志 的情况下才能调用本方法。"
// 参数<1>: [&主机名 SDT_TEXT], 参数说明: "The client's host name"
// 参数<2>: [&用户名 SDT_TEXT], 参数说明: "The client's user name"
// 参数<3>: [&服务器名 SDT_TEXT], 参数说明: "The name of the server to which the client is connected"
// 参数<4>: [&操作类型 SDT_TEXT], 参数说明: "The HTTP operation type"
// 参数<5>: [&目标 SDT_TEXT], 参数说明: "The target of the HTTP command"
// 参数<6>: [&参数 SDT_TEXT], 参数说明: "The parameters passed to the HTTP command"
// 参数<7>: [&HTTP状态码 SDT_INT], 参数说明: "The HTTP return status"
// 参数<8>: [&Win32错误码 SDT_INT], 参数说明: "The Win32 error code"
// 参数<9>: [&发送字节数 SDT_INT], 参数说明: "The number of bytes sent from the server to the client"
// 参数<10>: [&接收字节数 SDT_INT], 参数说明: "The number of bytes received by the server from the client"
// 参数<11>: [&处理时间(ms) SDT_INT], 参数说明: "单位毫秒。The time in milliseconds required to process the client request"
ISAPI_EXTERN_C void isapi_FilterGetLogInfo_80_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR*   arg1 = pArgInf[1].m_ppText;
    LPSTR*   arg2 = pArgInf[2].m_ppText;
    LPSTR*   arg3 = pArgInf[3].m_ppText;
    LPSTR*   arg4 = pArgInf[4].m_ppText;
    LPSTR*   arg5 = pArgInf[5].m_ppText;
    LPSTR*   arg6 = pArgInf[6].m_ppText;
    PINT     arg7 = pArgInf[7].m_pInt;
    PINT     arg8 = pArgInf[8].m_pInt;
    PINT     arg9 = pArgInf[9].m_pInt;
    PINT     arg10 = pArgInf[10].m_pInt;
    PINT     arg11 = pArgInf[11].m_pInt;

}

