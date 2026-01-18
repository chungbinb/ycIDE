#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置纹理调色板, 命令说明: "设置当前的纹理调色板。"
// 参数<1>: 调色板号 SDT_INT, 参数说明: "将要设置为当前纹理调色板的调色板号码。"
OGRELIB_EXTERN_C void ogrelib_SetCurrentTexturePalette_160_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

