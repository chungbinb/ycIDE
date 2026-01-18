#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 计算顶点的法向量, 命令说明: "有时.x文件不包含顶点的法向量，这时，如果使用光照，则需要手动计算顶点的法向量。"
// 参数<1>: 模型 MAKELONG(0x1A, 0), 参数说明: "规格化的模型。"
// 参数<2>: [邻接信息 数组 SDT_INT], 参数说明: "如果提供了邻接信息，则重复的顶点会被忽略；如果没有邻接信息，重复的顶点也会被重复计算。另外一点更加重要，需要计算法向量的模型对象的顶点格式必须包含“法线”标志。"
OGRELIB_EXTERN_C void ogrelib_D3DXComputeNormals_339_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    void*    arg2 = pArgInf[1].m_pAryData;

}

