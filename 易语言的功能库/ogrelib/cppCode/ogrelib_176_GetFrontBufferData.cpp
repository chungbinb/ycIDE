#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).复制前台缓冲页面, 命令说明: "将设备的前台缓冲页面复制到程序指定的系统内存缓冲中。"
// 参数<1>: 交换链标识 SDT_INT, 参数说明: "指定一个交换链。"
// 参数<2>: 目标页面 MAKELONG(0x08, 0), 参数说明: "设备前台缓冲页面的副本。在窗口模式中，目标页面的大小为桌面大小；在全屏模式中，目标页面的大小为屏幕大小。"
OGRELIB_EXTERN_C void ogrelib_GetFrontBufferData_176_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

