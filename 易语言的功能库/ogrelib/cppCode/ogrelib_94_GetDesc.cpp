#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立体).取信息, 命令说明: "获级当前体积信息。"
// 参数<1>: &立体信息 MAKELONG(0x24, 0), 参数说明: "返回立体的信息。"
OGRELIB_EXTERN_C void ogrelib_GetDesc_94_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

