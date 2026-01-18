#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取设备特性, 命令说明: "取得渲染设备的技术特性。"
// 参数<1>: 设备特性 MAKELONG(0x26, 0), 参数说明: "要取得的设备特性。"
OGRELIB_EXTERN_C void ogrelib_GetDeviceCaps_172_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

