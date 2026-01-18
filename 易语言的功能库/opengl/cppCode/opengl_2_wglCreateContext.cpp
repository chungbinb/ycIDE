#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 创建着色描述表, 命令说明: "为设备创建着色描述表，成功返回着色描述表号，失败返回0。在不用的时候必须删除着色描述表。"
// 参数<1>: 设备句柄 SDT_INT, 参数说明: "需要创建着色描述表的设备句柄。"
OPENGL_EXTERN_C void opengl_wglCreateContext_2_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

