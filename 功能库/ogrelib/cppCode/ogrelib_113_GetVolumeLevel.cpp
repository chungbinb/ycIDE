#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立体纹理).取级别立体, 命令说明: "获取指定级别所对应的立体。"
// 参数<1>: 级别 SDT_INT, 参数说明: "立体对应的级别。"
// 参数<2>: &立体 MAKELONG(0x25, 0), 参数说明: "返回的立体。"
OGRELIB_EXTERN_C void ogrelib_GetVolumeLevel_113_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void**   arg2 = pArgInf[2].m_ppCompoundData;

}

