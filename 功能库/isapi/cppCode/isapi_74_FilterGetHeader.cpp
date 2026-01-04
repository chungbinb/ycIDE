#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网筛选器).取HTTP头, 命令说明: "返回HTTP头中指定名称所对应的值。仅当“取事件类型()”返回值为 #筛选器事件.预处理头、#筛选器事件.发送回复、#筛选器事件.认证完成 的情况下才能调用此方法。"
// 参数<1>: 名称 SDT_TEXT, 参数说明: "普通名称必须以\":\"结尾。另有一些特殊名称可供使用：\"method\", \"url\", \"version\", \"status\"，其中前三个在 #筛选器事件.预处理头 和 #筛选器事件.认证完成 事件中有效，最后一个\"status\"在 #筛选器事件.发送回复 事件中有效。"
ISAPI_EXTERN_C void isapi_FilterGetHeader_74_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

