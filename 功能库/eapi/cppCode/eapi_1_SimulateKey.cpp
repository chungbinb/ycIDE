#include "..\include_eapi_header.h"

// 调用格式: _SDT_NULL 模拟按键, 命令说明: "模拟键盘的输入。"
// 参数<1>: 虚拟键值1 _SDT_ALL, 参数说明: NULL
// 参数<2>: [虚拟键值2 _SDT_ALL], 参数说明: NULL
// 参数<3>: [虚拟键值3 _SDT_ALL], 参数说明: NULL
EAPI_EXTERN_C void eapi_SimulateKey_1_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;
    PVOID    arg2 = pArgInf[1].m_pByte;
    PVOID    arg3 = pArgInf[2].m_pByte;

}

