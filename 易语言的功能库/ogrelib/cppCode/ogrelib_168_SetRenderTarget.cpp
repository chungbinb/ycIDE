#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置渲染对象, 命令说明: "给设备设置一个新的颜色缓冲页面。"
// 参数<1>: 渲染对象索引 SDT_INT, 参数说明: NULL
// 参数<2>: 渲染对象页面 MAKELONG(0x08, 0), 参数说明: "新的渲染对象页面。如为空，则相应序号的渲染对象将被禁用。"
OGRELIB_EXTERN_C void ogrelib_SetRenderTarget_168_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

