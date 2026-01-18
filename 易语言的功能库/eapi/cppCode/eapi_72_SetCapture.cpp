#include "..\include_eapi_header.h"

// 调用格式: SDT_INT 捕获鼠标, 命令说明: "将鼠标捕获设置到指定的窗口。在鼠标按钮按下的时候，这个窗口会为当前应用程序或整个系统接收所有鼠标输入。返回值为之前拥有鼠标捕获的窗口的句柄，如没有这样的窗口则返回空。"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: "要接收所有鼠标输入的窗口的句柄。"
EAPI_EXTERN_C void eapi_SetCapture_72_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

