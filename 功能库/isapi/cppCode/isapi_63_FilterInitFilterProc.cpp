#include "..\include_isapi_header.h"

// 调用格式: _SDT_NULL (互联网筛选器).初始化互联网筛选器, 命令说明: "在筛选器DLL的公开导出函数 HttpFilterProc() 中必须首先调用本方法"
// 参数<1>: 筛选器上下文 SDT_INT, 参数说明: "必须是 HttpFilterProc() 函数的第一个参数"
// 参数<2>: 通知类型 SDT_INT, 参数说明: "必须是 HttpFilterProc() 函数的第二个参数"
// 参数<3>: 通知数据 SDT_INT, 参数说明: "必须是 HttpFilterProc() 函数的第三个参数"
ISAPI_EXTERN_C void isapi_FilterInitFilterProc_63_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

