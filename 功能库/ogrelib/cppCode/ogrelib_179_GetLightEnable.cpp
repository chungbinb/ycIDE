#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取灯光状态, 命令说明: "取得灯光的开启状态。真为开启、假为关闭。"
// 参数<1>: 灯光序号 SDT_INT, 参数说明: "要取得状态的灯光序号。"
OGRELIB_EXTERN_C void ogrelib_GetLightEnable_179_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

