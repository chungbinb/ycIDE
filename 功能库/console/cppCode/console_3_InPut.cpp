#include "..\include_console_header.h"

// 调用格式: SDT_TEXT (控制台对象).输入, 命令说明: "从控制台接收输入的数据，如果“横坐标”或“纵坐标”为空，那么从当前光标位置开始输入，注意，输入时不过滤特殊字符，如退格键等"
// 参数<1>: [横坐标 SDT_INT], 参数说明: "指定输入数据时光标的横坐标"
// 参数<2>: [纵坐标 SDT_INT], 参数说明: "指定输入数据时光标的纵坐标"
// 参数<3>: 保存当前光标 SDT_BOOL, 参数说明: "是否保存当前光标位置"
// 参数<4>: 是否回显 SDT_BOOL, 参数说明: "是否把输入的数据回显到控制台设备"
// 参数<5>: [回显数据 SDT_TEXT], 参数说明: "回显的数据，如果忽略，则使用输入的字符，在“是否回显”为真时有意义"
// 参数<6>: 回车结束 SDT_BOOL, 参数说明: "是否以回车结束输入"
// 参数<7>: 最大接收长度 SDT_INT, 参数说明: "最大接收数据长度，“回车结束”为真时有意义"
CONSOLE_EXTERN_C void console_InPut_3_console(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    BOOL     arg3 = pArgInf[3].m_bool;
    BOOL     arg4 = pArgInf[4].m_bool;
    LPSTR    arg5 = pArgInf[5].m_pText;
    BOOL     arg6 = pArgInf[6].m_bool;
    INT      arg7 = pArgInf[7].m_int;

}

