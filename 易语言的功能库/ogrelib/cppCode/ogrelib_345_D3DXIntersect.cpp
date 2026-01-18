#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 是否相交, 命令说明: "检测空间中的一条射线是否与指定的模型相交。如相交则返回相交信息数组，不相交返回空数组。"
// 参数<1>: 模型 MAKELONG(0x1A, 0), 参数说明: "要检测是否相交的模型。"
// 参数<2>: 射线发出点 MAKELONG(0x02, 0), 参数说明: "指定射线的原点坐标。"
// 参数<3>: 射线方向 MAKELONG(0x02, 0), 参数说明: "指定射线的发射方向。"
// 参数<4>: &三角形索引 SDT_INT, 参数说明: "如果相交，则为与射线发出点距离最近的三角形的索引。"
// 参数<5>: [&重心坐标U分量 SDT_FLOAT], 参数说明: "如果相交，则为与射线相交的三角形中的重心坐标U分量。"
// 参数<6>: [&重心坐标V分量 SDT_FLOAT], 参数说明: "如果相交，则为与射线相交的三角形中的重心坐标V分量。"
// 参数<7>: [&距离 SDT_FLOAT], 参数说明: "如果相交，则为从射线发出点到相交点的距离。"
// 参数<8>: [&所有相交信息 数组 MAKELONG(0x45, 0)], 参数说明: "如果相交，则为所有相交信息数组。"
OGRELIB_EXTERN_C void ogrelib_D3DXIntersect_345_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    void*    arg2 = pArgInf[1].m_pCompoundData;
    void*    arg3 = pArgInf[2].m_pCompoundData;
    PINT     arg4 = pArgInf[3].m_pInt;
    PFLOAT   arg5 = pArgInf[4].m_pFloat;
    PFLOAT   arg6 = pArgInf[5].m_pFloat;
    PFLOAT   arg7 = pArgInf[6].m_pFloat;
    void**   arg8 = pArgInf[7].m_ppAryData;

}

