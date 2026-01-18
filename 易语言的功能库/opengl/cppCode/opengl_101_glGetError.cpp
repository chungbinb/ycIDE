#include "..\include_opengl_header.h"

// 调用格式: SDT_TEXT 取错误信息文本, 命令说明: "如果执行某OPenGL命令失败，在其后执行本命令可以取回错误信息文本。\t\t如果该OPenGL命令执行成功，执行本命令将返回空文本。\r\n\t\t如果参数值超出整数的范围，返回“无效整数”\r\n\t\t如果参数值超出可选的范围，返回“无效数值”\r\n\t\t如果OPenGL命令出现在错误的位置，返回“无效操作”\r\n\t\t如果在堆栈已满时继续入栈，返回“堆栈溢出”\r\n\t\t如果在堆栈已空时继续出栈，返回“堆栈下溢”"
// 无参数
OPENGL_EXTERN_C void opengl_glGetError_101_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

