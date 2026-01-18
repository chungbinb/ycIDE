#include "..\include_isapi_header.h"

// 调用格式: _SDT_NULL (互联网扩展).会话注册变量和值, 命令说明: "为当前会话注册变量和值，如果变量已注册则修改变量的值。"
// 参数<1>: 变量名 SDT_TEXT, 参数说明: "本参数表示要注册或修改变量名。"
// 参数<2>: 值 SDT_TEXT, 参数说明: "本参数表示赋予变量的值。"
ISAPI_EXTERN_C void isapi_SessionRegVarAndVal_20_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

