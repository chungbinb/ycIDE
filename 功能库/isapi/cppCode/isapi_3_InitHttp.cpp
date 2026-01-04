#include "..\include_isapi_header.h"

// 调用格式: _SDT_NULL (互联网扩展).初始化互联网扩展, 命令说明: "本方法应该在(HttpExtensionProc)子程序中调用"
// 参数<1>: 扩展程序上下文 SDT_INT, 参数说明: "要求必须是 HttpExtensionProc 函数的参数"
ISAPI_EXTERN_C void isapi_InitHttp_3_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

