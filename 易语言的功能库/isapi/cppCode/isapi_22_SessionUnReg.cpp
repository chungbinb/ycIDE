#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).会话取消注册变量, 命令说明: "从当前会话中取消此变量。"
// 参数<1>: 变量名 SDT_TEXT, 参数说明: "本参数表示要取消的变量名。"
ISAPI_EXTERN_C void isapi_SessionUnReg_22_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

