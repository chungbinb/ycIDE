#include "..\include_shellex_header.h"

// 调用格式: SDT_BOOL 撤销热键, 命令说明: "撤销所注册的系统热键"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: "窗口句柄"
// 参数<2>: 热键标识 SDT_INT, 参数说明: "注册热键时所返回的热键标识"
SHELLEX_EXTERN_C void shellex_UnRegHotKey_11_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

