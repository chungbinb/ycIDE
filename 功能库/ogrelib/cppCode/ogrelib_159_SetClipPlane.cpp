#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置剪裁平面系数, 命令说明: "设置剪裁平面系数。"
// 参数<1>: 序号 SDT_INT, 参数说明: NULL
// 参数<2>: 系数 SDT_FLOAT, 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_SetClipPlane_159_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    FLOAT    arg2 = pArgInf[2].m_float;

}

