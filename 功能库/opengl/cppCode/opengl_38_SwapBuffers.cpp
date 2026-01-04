#include "..\include_opengl_header.h"

// 调用格式: SDT_BOOL 交换缓存, 命令说明: "交换后台缓存与前台缓存。将后台缓存显示出来，对前台缓存进行操作。成功返回真，失败返回假。在双缓存模式下才用效。"
// 参数<1>: 设备句柄 SDT_INT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_SwapBuffers_38_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

