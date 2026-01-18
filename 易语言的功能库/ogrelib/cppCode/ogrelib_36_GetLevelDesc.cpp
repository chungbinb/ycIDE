#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立方体纹理).取级别信息, 命令说明: "获级指定级别的纹理信息。"
// 参数<1>: 级别 SDT_INT, 参数说明: "要取的级别。"
// 参数<2>: &信息 MAKELONG(0x1D, 0), 参数说明: "存放取后的信息。"
OGRELIB_EXTERN_C void ogrelib_GetLevelDesc_36_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void**   arg2 = pArgInf[2].m_ppCompoundData;

}

