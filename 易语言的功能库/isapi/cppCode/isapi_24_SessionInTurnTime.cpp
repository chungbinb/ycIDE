#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网扩展).会话轮询时间, 命令说明: "程序多长时间轮询一次，将失效的会话删掉。"
// 参数<1>: 时间 SDT_INT, 参数说明: "本参数表示要轮询的时间，单位为秒。注：如果本参数设的时间过长可能会使“会话超时”不准，如果本参数设的时间过短对性能有影响。默认为2分钟。"
ISAPI_EXTERN_C void isapi_SessionInTurnTime_24_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

