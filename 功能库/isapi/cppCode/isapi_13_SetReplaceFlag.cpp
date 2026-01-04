#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).设置替换标志, 命令说明: "设置替换HTML模板的标志，如:<!--$站点$-->,“站点”前后的就是标志。"
// 参数<1>: 前标志 SDT_TEXT, 参数说明: "本参数表示变量前面的标志。"
// 参数<2>: 后标志 SDT_TEXT, 参数说明: "本参数表示变量后面的标志。"
ISAPI_EXTERN_C void isapi_SetReplaceFlag_13_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

