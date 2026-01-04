#include "..\include_eapi_header.h"

// 调用格式: SDT_DOUBLE 取屏幕DPI, 命令说明: "返回指定窗口句柄所处显示器的当前DPI(缩放比例). hWnd: 提供所欲检查窗口句柄,为NULL表示使用桌面窗口(主显示器)."
// 参数<1>: [窗口句柄 SDT_INT], 参数说明: "欲取所在窗口的软件窗口句柄，为空则取主屏幕。"
EAPI_EXTERN_C void eapi_GetMoniterDPI_89_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

