#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL 清空模型, 命令说明: "清空一个模型，为简化做准备。"
// 参数<1>: 模型 MAKELONG(0x1A, 0), 参数说明: "指定将要清空的模型。"
// 参数<2>: 输入邻接信息 数组 SDT_INT, 参数说明: "要被清空的模型的邻接信息。"
// 参数<3>: 清空后的模型 MAKELONG(0x1A, 0), 参数说明: "返回清空后的模型。"
// 参数<4>: &输出邻接信息 数组 SDT_INT, 参数说明: "清空后的模型的邻接信息。"
OGRELIB_EXTERN_C void ogrelib_D3DXCleanMesh_336_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[0].m_pCompoundData;
    void*    arg2 = pArgInf[1].m_pAryData;
    void*    arg3 = pArgInf[2].m_pCompoundData;
    void**   arg4 = pArgInf[3].m_ppAryData;

}

