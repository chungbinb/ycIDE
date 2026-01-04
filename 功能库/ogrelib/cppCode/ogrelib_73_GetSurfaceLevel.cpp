#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (纹理).取级别页面, 命令说明: "获取指定级别所对应的页面。"
// 参数<1>: 级别 SDT_INT, 参数说明: "要取的级别。"
// 参数<2>: &页面 MAKELONG(0x08, 0), 参数说明: "存放的页面。"
OGRELIB_EXTERN_C void ogrelib_GetSurfaceLevel_73_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void**   arg2 = pArgInf[2].m_ppCompoundData;

}

