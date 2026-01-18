#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取视见区, 命令说明: "取剪裁平面系数。成功返回系数，失败返回-1。"
// 参数<1>: 视见区 MAKELONG(0x07, 0), 参数说明: "要取得的视见区。"
OGRELIB_EXTERN_C void ogrelib_GetViewport_186_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

