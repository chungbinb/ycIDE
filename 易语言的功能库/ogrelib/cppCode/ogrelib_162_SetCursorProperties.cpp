#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置光标属性, 命令说明: NULL
// 参数<1>: 热区偏移X坐标 SDT_INT, 参数说明: "热区相对于左上角的偏移量。"
// 参数<2>: 热区偏移Y坐标 SDT_INT, 参数说明: "热区相对于左上角的偏移量。"
// 参数<3>: 光标位图页面 MAKELONG(0x08, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_SetCursorProperties_162_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    void*    arg3 = pArgInf[3].m_pCompoundData;

}

