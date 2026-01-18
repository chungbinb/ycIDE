#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT 取本机网卡名, 命令说明: "取得本地计算机的网卡信息列表。失败返回空"
// 参数<1>: [网卡序号 SDT_INT], 参数说明: "可以被省略。当本机有多块网卡时，指定要对其操作的网卡序号（从1开始）。"
EAPI_EXTERN_C void eapi_GetLocalAdapterName_43_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

