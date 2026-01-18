#include "..\include_isapi_header.h"

// 调用格式: SDT_TEXT (互联网扩展).会话获取变量值, 命令说明: "从当前会话获取此变量的值。"
// 参数<1>: 变量名 SDT_TEXT, 参数说明: "本参数表示要取值的变量名。"
ISAPI_EXTERN_C void isapi_SessionGetVal_23_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

