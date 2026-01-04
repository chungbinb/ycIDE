#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 端口检测, 命令说明: NULL
// 参数<1>: 端口 SDT_SHORT, 参数说明: "检测的端口。"
// 参数<2>: [ip地址 SDT_TEXT], 参数说明: "置空则检测本机端口。"
EAPI_EXTERN_C void eapi_CheckPort_54_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    SHORT    arg1 = pArgInf[0].m_short;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

