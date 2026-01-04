#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 窗口置顶, 命令说明: " 置顶置顶窗口"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: "欲修改的窗口句柄"
// 参数<2>: [是否总在最前 SDT_BOOL], 参数说明: "默认为假"
EAPI_EXTERN_C void eapi_SetForegroundWindow_96_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    BOOL     arg2 = pArgInf[1].m_bool;

}

