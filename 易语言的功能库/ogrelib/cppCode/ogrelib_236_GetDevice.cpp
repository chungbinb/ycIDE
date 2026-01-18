#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (交换链).取设备, 命令说明: "取与交换链相关联的设备。"
// 参数<1>: 设备 MAKELONG(0x09, 0), 参数说明: "要取得的设备。"
OGRELIB_EXTERN_C void ogrelib_GetDevice_236_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

