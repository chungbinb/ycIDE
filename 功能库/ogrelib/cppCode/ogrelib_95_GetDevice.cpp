#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立体).取设备, 命令说明: "获得使用此资源的设备。"
// 参数<1>: &设备 MAKELONG(0x09, 0), 参数说明: "要获得的设备变量。"
OGRELIB_EXTERN_C void ogrelib_GetDevice_95_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

