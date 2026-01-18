#include "..\include_console_header.h"

// 调用格式: SDT_BOOL (控制台对象).置光标位置, 命令说明: "设置光标位置"
// 参数<1>: 横坐标 SDT_INT, 参数说明: "光标的新横坐标"
// 参数<2>: 纵坐标 SDT_INT, 参数说明: "光标的新纵坐标"
CONSOLE_EXTERN_C void console_SetCursorPosition_5_console(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

