#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 终止进程, 命令说明: "模拟键盘的输入。"
// 参数<1>: 进程标识 _SDT_ALL, 参数说明: "要终止进程的进程ID、进程名称或窗口标题。"
EAPI_EXTERN_C void eapi_SimulateKey_11_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;

}

