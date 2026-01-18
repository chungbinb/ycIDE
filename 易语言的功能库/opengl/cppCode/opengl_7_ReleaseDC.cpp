#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 释放设备句柄, 命令说明: "释放由调用“取任意设备句柄”命令获取的指定设备场景，执行成功为1，失败为0。"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: NULL
// 参数<2>: 设备句柄 SDT_INT, 参数说明: "需要释放的设备句柄。"
OPENGL_EXTERN_C void opengl_ReleaseDC_7_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

