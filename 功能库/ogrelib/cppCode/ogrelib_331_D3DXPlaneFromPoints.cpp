#include "..\include_ogrelib_header.h"

// 调用格式: _SDT_NULL (平面).创建平面扩展, 命令说明: "根据三个点创建一个平面"
// 参数<1>: 空间点坐标 MAKELONG(0x02, 0), 参数说明: NULL
// 参数<2>: 空间点坐标 MAKELONG(0x02, 0), 参数说明: NULL
// 参数<3>: 空间点坐标 MAKELONG(0x02, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXPlaneFromPoints_331_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    void*    arg3 = pArgInf[3].m_pCompoundData;

}

