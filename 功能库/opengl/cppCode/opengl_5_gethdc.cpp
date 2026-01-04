#include "..\include_opengl_header.h"

// 调用格式: SDT_INT 取任意设备句柄, 命令说明: "可以取得任意一个窗口组件的设备句柄，成功返回着色描述表号，失败返回0。"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_gethdc_5_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

