#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).交换页面, 命令说明: "将后台缓冲区中的内容显示到前台，并刷新窗口。渲染完毕后，请注意一定不要忘记使用本命令显示渲染结果，否则不会有任何输出。"
// 参数<1>: [源矩形 MAKELONG(0x01, 0)], 参数说明: "将指定矩形区域的后台缓冲页面显示到前台。默认为将整个后台缓冲页面显示到前台。"
// 参数<2>: [目的矩形 MAKELONG(0x01, 0)], 参数说明: "目的页面的矩形区域。默认为整个客户区域。"
// 参数<3>: [目标窗口 SDT_INT], 参数说明: "指定显示的目标窗口，默认为设备参数中的设备窗口句柄。"
OGRELIB_EXTERN_C void ogrelib_Present_151_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    INT      arg3 = pArgInf[3].m_int;

}

