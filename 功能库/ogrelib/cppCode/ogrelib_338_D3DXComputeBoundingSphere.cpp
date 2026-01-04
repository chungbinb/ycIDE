#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 计算模型边界球, 命令说明: "计算模型边界球。使用顶点数组作为输入计算边界球，可以使用各种顶点格式。"
// 参数<1>: 第一个顶点地址 MAKELONG(0x02, 0), 参数说明: "顶点数组的地址，顶点的第一个向量需要是顶点的位置坐标。"
// 参数<2>: 顶点个数 SDT_INT, 参数说明: "指定顶点的个数。"
// 参数<3>: 顶点大小 SDT_INT, 参数说明: "顶点大小（以字节为单位）。因顶点中有很多附加数据，如法向量、纹理坐标等，计算边界范围不需要这些数据，所以，需要知道跳过多少数据才能找到下一个顶点的坐标。"
// 参数<4>: 中心坐标 MAKELONG(0x02, 0), 参数说明: "返回边界球的中心坐标。"
// 参数<5>: &半径 SDT_FLOAT, 参数说明: "返回边界球的半径。"
OGRELIB_EXTERN_C void ogrelib_D3DXComputeBoundingSphere_338_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    void*    arg4 = pArgInf[3].m_pCompoundData;
    PFLOAT   arg5 = pArgInf[4].m_pFloat;

}

