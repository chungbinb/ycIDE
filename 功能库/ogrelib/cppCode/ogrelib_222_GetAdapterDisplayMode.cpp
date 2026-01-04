#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).取当前显示模式, 命令说明: "取适配器当前显示模式。"
// 参数<1>: [适配器序号 SDT_INT], 参数说明: "指定要取得模式的适配器的序号。默认值为主显示适配器。"
// 参数<2>: 显示模式 MAKELONG(0x22, 0), 参数说明: "要取得的当前显示模式。"
OGRELIB_EXTERN_C void ogrelib_GetAdapterDisplayMode_222_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

