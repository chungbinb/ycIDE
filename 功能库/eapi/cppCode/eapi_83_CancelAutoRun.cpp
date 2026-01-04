#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 取消自动运行, 命令说明: "取消程序的自动运行"
// 参数<1>: 程序名 SDT_TEXT, 参数说明: "设置自动运行时的程序名称。"
EAPI_EXTERN_C void eapi_CancelAutoRun_83_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

