#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (交换链).取设备参数, 命令说明: "取交换链相关的设备参数。"
// 参数<1>: 设备参数 MAKELONG(0x1B, 0), 参数说明: "要取得的设备参数。"
OGRELIB_EXTERN_C void ogrelib_GetPresentParameters_239_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

