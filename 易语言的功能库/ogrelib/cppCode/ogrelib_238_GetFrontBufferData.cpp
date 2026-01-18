#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (交换链).复制前台缓冲页面, 命令说明: "将交换链的前台缓冲页面复制到程序指定的系统内存缓冲中。"
// 参数<1>: 目标页面 MAKELONG(0x08, 0), 参数说明: "交换链前台缓冲页面的副本。在窗口模式中，目标页面的大小为桌面大小；在全屏模式中，目标页面的大小为屏幕大小。"
OGRELIB_EXTERN_C void ogrelib_GetFrontBufferData_238_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

