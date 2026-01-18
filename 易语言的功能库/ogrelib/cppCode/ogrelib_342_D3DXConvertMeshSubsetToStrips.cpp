#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 模型子集到连续带, 命令说明: "转换指定的模型子集到连续带。"
// 参数<1>: 模型 MAKELONG(0x1A, 0), 参数说明: "表示要转换到一个带的模型。"
// 参数<2>: 属性标识 SDT_INT, 参数说明: "表示要转换模型子集的属性标识。"
// 参数<3>: 索引缓冲创建选项 SDT_INT, 参数说明: "指定索引缓冲创建选项。"
// 参数<4>: 索引缓冲 MAKELONG(0x18, 0), 参数说明: "表示包含带的索引缓冲。"
// 参数<5>: &顶点个数 SDT_INT, 参数说明: "索引缓冲的顶点个数。"
// 参数<6>: &每个带三角形数量 数组 SDT_INT, 参数说明: "每个带包含的三角形数量的数组。"
// 参数<7>: &带数量 SDT_INT, 参数说明: "索引缓冲和相应的带长数组中包含的单独带的数量。"
OGRELIB_EXTERN_C void ogrelib_D3DXConvertMeshSubsetToStrips_342_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;
    void*    arg4 = pArgInf[3].m_pCompoundData;
    PINT     arg5 = pArgInf[4].m_pInt;
    void**   arg6 = pArgInf[5].m_ppAryData;
    PINT     arg7 = pArgInf[6].m_pInt;

}

