#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).启用灯光, 命令说明: "启用已经放置好的一个灯光。"
// 参数<1>: 索引 SDT_INT, 参数说明: "本参数表示此灯光在场景中的索引，是“设置灯光”参数中的那个索引。"
// 参数<2>: 启用或禁用 SDT_BOOL, 参数说明: "本参数表示启用或禁用索引中指的灯光。"
OGRELIB_EXTERN_C void ogrelib_Enablelight_146_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    BOOL     arg2 = pArgInf[2].m_bool;

}

