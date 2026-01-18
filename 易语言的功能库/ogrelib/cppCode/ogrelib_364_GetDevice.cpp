#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (模型).取设备, 命令说明: "取和此模型关联的设备。"
// 参数<1>: 设备 MAKELONG(0x09, 0), 参数说明: "存放设备。"
OGRELIB_EXTERN_C void ogrelib_GetDevice_364_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

