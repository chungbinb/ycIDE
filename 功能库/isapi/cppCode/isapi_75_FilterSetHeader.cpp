#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网筛选器).置HTTP头, 命令说明: "设置HTTP头中指定名称所对应的值，或在参数“值”被省略或为空文本的情况下删除“名称”。仅当“取事件类型()”返回值为 #筛选器事件.预处理头、#筛选器事件.发送回复、#筛选器事件.认证完成 的情况下才能调用此方法。"
// 参数<1>: 名称 SDT_TEXT, 参数说明: "普通名称必须以\":\"结尾。另有一些特殊名称可供使用：\"method\", \"url\", \"version\", \"status\"，其中前三个在 #筛选器事件.预处理头 和 #筛选器事件.认证完成 事件中有效，最后一个\"status\"在 #筛选器事件.发送回复 事件中有效。"
// 参数<2>: [值 SDT_TEXT], 参数说明: "如果本参数被省略或为空文本，表示从HTTP头中删除对应的名称"
ISAPI_EXTERN_C void isapi_FilterSetHeader_75_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

