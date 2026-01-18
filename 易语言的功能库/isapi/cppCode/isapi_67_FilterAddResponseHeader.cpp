#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网筛选器).添加回复头, 命令说明: "建议在 #筛选器事件.发送回复 之前的筛选器事件中使用此命令。当“取事件类型()”返回值为 #筛选器事件.预处理头、#筛选器事件.发送回复、#筛选器事件.认证完成 的情况下，应使用“加HTTP头()”或“置HTTP头()”"
// 参数<1>: 名称 SDT_TEXT, 参数说明: NULL
// 参数<2>: 值 SDT_TEXT, 参数说明: NULL
ISAPI_EXTERN_C void isapi_FilterAddResponseHeader_67_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

