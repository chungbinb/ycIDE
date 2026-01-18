#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (模型).取顶点缓冲, 命令说明: "获取此模型用的顶点缓冲。"
// 参数<1>: 顶点缓冲 MAKELONG(0x13, 0), 参数说明: "存放顶点缓冲。"
OGRELIB_EXTERN_C void ogrelib_GetVertexBuffer_371_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

