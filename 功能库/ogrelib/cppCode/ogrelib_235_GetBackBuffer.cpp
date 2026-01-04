#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (交换链).取后台缓冲页面, 命令说明: "从设备交换链中取得后台缓冲页面。"
// 参数<1>: 后台缓冲序号 SDT_INT, 参数说明: "要取得的后台缓冲序号。序号范围从0 - (后台缓冲数量 - 1)。为0返回第一个后台页面（不是前台页面），不能通过该命令取得前台页面。"
// 参数<2>: 后台缓冲页面 MAKELONG(0x08, 0), 参数说明: "要取得的后台缓冲页面。"
OGRELIB_EXTERN_C void ogrelib_GetBackBuffer_235_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

