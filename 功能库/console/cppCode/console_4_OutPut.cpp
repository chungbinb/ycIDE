#include "..\include_console_header.h"

// 调用格式: SDT_BOOL (控制台对象).输出, 命令说明: "向控制台输出数据，如果“横坐标”或“纵坐标”为空，那么从当前光标位置开始输出"
// 参数<1>: [横坐标 SDT_INT], 参数说明: "指定输出数据时光标的横坐标"
// 参数<2>: [纵坐标 SDT_INT], 参数说明: "指定输出数据时光标的纵坐标"
// 参数<3>: 保存当前光标 SDT_BOOL, 参数说明: "是否保存当前光标位置"
// 参数<4>: 前景颜色 SDT_INT, 参数说明: "前景即字体的颜色，颜色值为“控制台颜色”数据类型中定义的值"
// 参数<5>: 背景颜色 SDT_INT, 参数说明: "背景的颜色，颜色值为“控制台颜色”数据类型中定义的值"
// 参数<6>: 输出数据 SDT_TEXT, 参数说明: "待输出的数据"
CONSOLE_EXTERN_C void console_OutPut_4_console(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    BOOL     arg3 = pArgInf[3].m_bool;
    INT      arg4 = pArgInf[4].m_int;
    INT      arg5 = pArgInf[5].m_int;
    LPSTR    arg6 = pArgInf[6].m_pText;

}

