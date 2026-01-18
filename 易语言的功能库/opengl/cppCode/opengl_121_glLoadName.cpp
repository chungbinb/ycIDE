#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 置栈顶名字, 命令说明: "设置接下来的图元的名称。如果名字栈为空，将返回假。必须在选择模式下使用。否则无效。"
// 参数<1>: 名字 SDT_INT, 参数说明: "栈顶的名字。"
OPENGL_EXTERN_C void opengl_glLoadName_121_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

