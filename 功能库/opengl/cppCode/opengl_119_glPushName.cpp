#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 名字入栈, 命令说明: "设置接下来的图元的名称。并把指定名称压入栈顶,不能超出最大名字栈长度。每个图元的名字与当时出现在名字栈中所有的名字有关。因此一个图元可以拥有多个名字。连续重复使用本命令即可达到命名多级名字的效果。必须在选择模式下使用。否则无效。"
// 参数<1>: 名字 SDT_INT, 参数说明: "压入栈顶的名字。"
OPENGL_EXTERN_C void opengl_glPushName_119_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

