#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立体纹理).取级别信息, 命令说明: "获级指定级别的纹理信息。"
// 参数<1>: 级别 SDT_INT, 参数说明: "立体的信息的级别。"
// 参数<2>: &立体信息 MAKELONG(0x24, 0), 参数说明: "返回立体的信息。"
OGRELIB_EXTERN_C void ogrelib_GetLevelDesc_108_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void**   arg2 = pArgInf[2].m_ppCompoundData;

}

