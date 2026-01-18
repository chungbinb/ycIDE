#include "..\include_ogrelib_header.h"

// 调用格式: MAKELONG(0x02, 0) (平面).求直线平面交点, 命令说明: "找出直线跟平面的交点。如果直线平行于平面，"
// 参数<1>: 直线起点向量 MAKELONG(0x02, 0), 参数说明: NULL
// 参数<2>: 直线终点向量 MAKELONG(0x02, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXPlaneIntersectLine_335_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

