#include "..\include_eapi_header.h"

// 调用格式: _SDT_NULL 圆角化窗口, 命令说明: " 圆角化置顶窗口"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: "欲修改的窗口句柄"
// 参数<2>: [圆角宽度 SDT_INT], 参数说明: NULL
// 参数<3>: [圆角高度 SDT_INT], 参数说明: NULL
EAPI_EXTERN_C void eapi_RoundedWindow_95_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    INT      arg3 = pArgInf[2].m_int;

}

