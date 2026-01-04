#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建附加交换链, 命令说明: "通过调用本命令，应用程序可以为设备创建附加的交换链。应用程序可以为每个视区创建一个交换链并将每个交换链与某个特定窗口相关联。应用程序在每个交换链的后缓存中渲染图像，然后分别显示它们。"
// 参数<1>: 设备参数 MAKELONG(0x1B, 0), 参数说明: "新交换链的描述参数。"
// 参数<2>: 附加交换链 MAKELONG(0x21, 0), 参数说明: NULL
OGRELIB_EXTERN_C void ogrelib_CreateAdditionalSwapChain_196_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

