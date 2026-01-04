#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取灯光, 命令说明: "获取由“设置灯光”方法所设置的灯光。"
// 参数<1>: 索引 SDT_INT, 参数说明: "本参数表示要获取灯光的索引。"
// 参数<2>: &灯光 MAKELONG(0x0B, 0), 参数说明: "本参数表示要获取的灯光。"
OGRELIB_EXTERN_C void ogrelib_GetLight_178_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void**   arg2 = pArgInf[2].m_ppCompoundData;

}

