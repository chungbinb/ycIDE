#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (顶点缓冲).取缓冲信息, 命令说明: "获级当前顶点缓冲的信息。"
// 参数<1>: &信息 MAKELONG(0x14, 0), 参数说明: "存放取后的信息。"
OGRELIB_EXTERN_C void ogrelib_GetDesc_80_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

