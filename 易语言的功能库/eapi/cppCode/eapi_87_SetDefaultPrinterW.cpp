#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 设置默认打印机, 命令说明: "设置系统的默认打印机。"
// 参数<1>: 打印机名称 SDT_TEXT, 参数说明: "要设置为默认打印机的名称。"
EAPI_EXTERN_C void eapi_SetDefaultPrinterW_87_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

