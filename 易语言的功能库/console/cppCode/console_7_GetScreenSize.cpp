#include "..\include_console_header.h"

// 调用格式: _SDT_NULL (控制台对象).取显示区大小, 命令说明: "取显示区大小"
// 参数<1>: &宽度 SDT_INT, 参数说明: "显示区宽度，以字符为单位"
// 参数<2>: &高度 SDT_INT, 参数说明: "显示区高度，以字符为单位"
CONSOLE_EXTERN_C void console_GetScreenSize_7_console(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PINT     arg1 = pArgInf[1].m_pInt;
    PINT     arg2 = pArgInf[2].m_pInt;

}

