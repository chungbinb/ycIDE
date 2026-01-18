#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).设置灯光, 命令说明: "在渲染场景中放置灯光,注必须调用“启用灯光”方法才能使这个灯光发生作用。"
// 参数<1>: 索引 SDT_INT, 参数说明: "本参数表示此灯光在场景中的索引。"
// 参数<2>: 灯光 MAKELONG(0x0B, 0), 参数说明: "要设置的灯光，为“灯光”数据类型。"
OGRELIB_EXTERN_C void ogrelib_SetLight_145_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

