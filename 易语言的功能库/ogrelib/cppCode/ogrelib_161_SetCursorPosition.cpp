#include "..\include_ogrelib_header.h"

// 调用格式: _SDT_NULL (设备).置光标位置, 命令说明: "设置光标位置。"
// 参数<1>: X坐标 SDT_INT, 参数说明: NULL
// 参数<2>: Y坐标 SDT_INT, 参数说明: NULL
// 参数<3>: [更新选项 SDT_INT], 参数说明: "设置光标更新选项。0、调用交换页面命令时更新；1、立即更新。"
OGRELIB_EXTERN_C void ogrelib_SetCursorPosition_161_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

