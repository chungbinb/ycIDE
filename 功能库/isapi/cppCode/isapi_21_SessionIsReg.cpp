#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).会话变量是否注册, 命令说明: "检查当前会话中此变量是否注册。如果注册过了返回真，否则返回假。"
// 参数<1>: 变量名 SDT_TEXT, 参数说明: "本参数表示查询的变量名。"
ISAPI_EXTERN_C void isapi_SessionIsReg_21_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

