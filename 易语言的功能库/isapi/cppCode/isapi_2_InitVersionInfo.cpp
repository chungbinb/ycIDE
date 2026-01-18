#include "..\include_isapi_header.h"

// 调用格式: SDT_BOOL (互联网扩展).初始化版本信息, 命令说明: "本方法应该在GetExtensionVersion子程序中调用"
// 参数<1>: 版本信息 SDT_INT, 参数说明: "要求必须是 GetExtensionVersion 函数的参数"
// 参数<2>: 描述信息 SDT_TEXT, 参数说明: "用于描述此扩展程序的文本，可能会被显示在IIS管理器中"
ISAPI_EXTERN_C void isapi_InitVersionInfo_2_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

