#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 删除IE工具条按钮, 命令说明: "向IE工具栏添加自己的按钮。"
// 参数<1>: 按钮文字 _SDT_ALL, 参数说明: NULL
EAPI_EXTERN_C void eapi_DeleteButtonFromIE_36_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;

}

