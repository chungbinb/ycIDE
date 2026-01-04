#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置Z模板缓冲页面, 命令说明: NULL
// 参数<1>: [Z模板缓冲页面 MAKELONG(0x08, 0)], 参数说明: "Z模板缓冲页面。为空则关闭Z模板缓冲操作并释放上一个Z模板缓冲。"
OGRELIB_EXTERN_C void ogrelib_SetDepthStencilSurface_163_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

