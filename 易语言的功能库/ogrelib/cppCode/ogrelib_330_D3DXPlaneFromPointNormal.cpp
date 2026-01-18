#include "..\include_ogrelib_header.h"

// 调用格式: _SDT_NULL (平面).创建平面, 命令说明: "根据一个点和一条平面的法线创建一个平面"
// 参数<1>: 空间点坐标 MAKELONG(0x02, 0), 参数说明: NULL
// 参数<2>: 平面法线 MAKELONG(0x02, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXPlaneFromPointNormal_330_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

