#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).填充颜色, 命令说明: "允许应用程序用指定的颜色填充一个页面的指定区域。"
// 参数<1>: 页面 MAKELONG(0x08, 0), 参数说明: "要填充颜色的页面。"
// 参数<2>: 颜色 SDT_INT, 参数说明: "指定要填充的颜色。"
// 参数<3>: [矩形区域 MAKELONG(0x01, 0)], 参数说明: "要填充的矩形区域。置空则填充整个页面。"
OGRELIB_EXTERN_C void ogrelib_ColorFill_195_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;
    void*    arg3 = pArgInf[3].m_pCompoundData;

}

