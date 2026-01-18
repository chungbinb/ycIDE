#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取显示模式, 命令说明: NULL
// 参数<1>: 交换链标识 SDT_INT, 参数说明: "指定一个交换链。"
// 参数<2>: 显示模式 MAKELONG(0x22, 0), 参数说明: "要取得的显示模式。"
OGRELIB_EXTERN_C void ogrelib_GetDisplayMode_175_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

