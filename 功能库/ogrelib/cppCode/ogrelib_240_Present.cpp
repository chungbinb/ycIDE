#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (交换链).交换页面, 命令说明: "显示位于前台缓冲页面之后的那个后台缓冲页面。"
// 参数<1>: [源矩形 MAKELONG(0x01, 0)], 参数说明: "源矩形。为空则显示整个源页面。"
// 参数<2>: [目的矩形 MAKELONG(0x01, 0)], 参数说明: "目的矩形。为空则整个客户区域被填充。"
// 参数<3>: [目标窗口 SDT_INT], 参数说明: "指定显示的目标窗口，置空则为设备参数中的设备窗口句柄。"
OGRELIB_EXTERN_C void ogrelib_Present_240_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    INT      arg3 = pArgInf[3].m_int;

}

