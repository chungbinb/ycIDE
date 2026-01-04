#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取深度缓冲页面, 命令说明: "取得设备拥有的深度缓冲页面。"
// 参数<1>: 深度缓冲页面 MAKELONG(0x08, 0), 参数说明: "要取得的深度缓冲页面。"
OGRELIB_EXTERN_C void ogrelib_GetDepthStencilSurface_174_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

