#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (索引缓冲).取索引信息, 命令说明: "获级当前索引缓冲的信息。"
// 参数<1>: &信息 MAKELONG(0x19, 0), 参数说明: "存放取后的信息。"
OGRELIB_EXTERN_C void ogrelib_GetDesc_51_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

