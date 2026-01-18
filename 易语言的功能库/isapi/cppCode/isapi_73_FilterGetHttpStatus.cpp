#include "..\include_isapi_header.h"

// 调用格式: SDT_INT (互联网筛选器).HTTP状态码, 命令说明: "返回HTTP状态码，也可以通过参数设定新的HTTP状态码。仅当“取事件类型()”返回值为 #筛选器事件.预处理头 的情况下才能调用此方法。"
// 参数<1>: [HTTP状态码 SDT_INT], 参数说明: "如果此参数未被省略，参数值将被设定为新的HTTP状态码"
ISAPI_EXTERN_C void isapi_FilterGetHttpStatus_73_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

