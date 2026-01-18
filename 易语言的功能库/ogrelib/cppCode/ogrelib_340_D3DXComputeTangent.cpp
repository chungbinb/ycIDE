#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 计算正切向量, 命令说明: "为纹理阶段中给定的纹理坐标计算正切向量。"
// 参数<1>: 模型 MAKELONG(0x1A, 0), 参数说明: "给定的模型。"
// 参数<2>: 纹理阶段索引 SDT_INT, 参数说明: "表现纹理阶段的索引。"
// 参数<3>: 正切数据索引 SDT_INT, 参数说明: "提供使用正切数据的索引。"
// 参数<4>: 副法线索引 SDT_INT, 参数说明: "提供使用副法线数据的索引。"
// 参数<5>: 环绕标志 SDT_INT, 参数说明: "参数为0表示不环绕，为1表示在U和V方向上环绕。"
// 参数<6>: 邻接信息 数组 SDT_INT, 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_D3DXComputeTangent_340_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    INT      arg4 = pArgInf[3].m_int;
    INT      arg5 = pArgInf[4].m_int;
    void*    arg6 = pArgInf[5].m_pAryData;

}

