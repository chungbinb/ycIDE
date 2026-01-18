#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).会话开始, 命令说明: "开始一个新的会话(session)。"
// 参数<1>: 超时 SDT_INT, 参数说明: "本参数表示此会话过多长时间失效。单位为秒。"
ISAPI_EXTERN_C void isapi_SessionStart_18_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

