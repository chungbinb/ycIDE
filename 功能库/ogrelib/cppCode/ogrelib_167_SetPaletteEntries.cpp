#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置调色板单元, 命令说明: NULL
// 参数<1>: 调色板序号 SDT_INT, 参数说明: NULL
// 参数<2>: 调色板单元 MAKELONG(0x1F, 0), 参数说明: "要设置的调色板单元。"
OGRELIB_EXTERN_C void ogrelib_SetPaletteEntries_167_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

