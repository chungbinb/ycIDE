#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取材质, 命令说明: "获取当前材质。"
// 参数<1>: &材质 MAKELONG(0x0C, 0), 参数说明: "本参数表示要获取的材质。"
OGRELIB_EXTERN_C void ogrelib_GetMaterial_180_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

