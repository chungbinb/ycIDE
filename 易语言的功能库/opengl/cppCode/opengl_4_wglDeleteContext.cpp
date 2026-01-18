#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 删除着色描述表, 命令说明: "删除当前的着色描述表，解除OPenGL命令对指定的设备的作用。成功返回真，失败返回假。"
// 参数<1>: 着色描述表号 SDT_INT, 参数说明: "需要删除的着色描述表。"
OPENGL_EXTERN_C void opengl_wglDeleteContext_4_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

