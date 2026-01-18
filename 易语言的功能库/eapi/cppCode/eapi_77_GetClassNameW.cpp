#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT 取窗口类名, 命令说明: "取指定窗口的类名。成功返回窗口类名，失败返回空。"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: "要接收所有鼠标输入的窗口的句柄。"
EAPI_EXTERN_C void eapi_GetClassNameW_77_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

