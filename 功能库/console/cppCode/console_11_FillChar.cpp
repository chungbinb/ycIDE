#include "..\include_console_header.h"

// 调用格式: SDT_INT (控制台对象).填充字符, 命令说明: "填充字符"
// 参数<1>: 横坐标 SDT_INT, 参数说明: "开始填充的横坐标"
// 参数<2>: 纵坐标 SDT_INT, 参数说明: "开始填充的纵坐标"
// 参数<3>: 填充字符 SDT_TEXT, 参数说明: "本参数是一个文本，但只有第一个字符有效"
// 参数<4>: 填充长度 SDT_INT, 参数说明: "填充区域的长度"
CONSOLE_EXTERN_C void console_FillChar_11_console(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    LPSTR    arg3 = pArgInf[3].m_pText;
    INT      arg4 = pArgInf[4].m_int;

}

