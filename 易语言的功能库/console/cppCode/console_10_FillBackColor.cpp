#include "..\include_console_header.h"

// 调用格式: SDT_INT (控制台对象).填充背景颜色, 命令说明: "填充背景颜色，返回实际填充的长度，失败返回-1"
// 参数<1>: 横坐标 SDT_INT, 参数说明: "开始填充的横坐标"
// 参数<2>: 纵坐标 SDT_INT, 参数说明: "开始填充的纵坐标"
// 参数<3>: 背景颜色 SDT_INT, 参数说明: "背景颜色"
// 参数<4>: 填充长度 SDT_INT, 参数说明: "填充区域的长度"
CONSOLE_EXTERN_C void console_FillBackColor_10_console(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;
    INT      arg4 = pArgInf[4].m_int;

}

