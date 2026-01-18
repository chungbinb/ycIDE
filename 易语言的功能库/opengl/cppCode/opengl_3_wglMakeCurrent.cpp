#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 置当前着色描述表, 命令说明: "设置当前的着色描述表，成功以后所有OPenGL命令都作用在指定的设备上。成功返回真，失败返回假。在不使用着色描述表的时候要释放当前着色描述表。"
// 参数<1>: 设备句柄 SDT_INT, 参数说明: "需要设置着色描述表的设备句柄。0表示释放当前着色描述表。"
// 参数<2>: 着色描述表号 SDT_INT, 参数说明: "需要设置的着色描述表，0表示释放当前着色描述表。"
OPENGL_EXTERN_C void opengl_wglMakeCurrent_3_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

