#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 取CPU信息, 命令说明: "取CPU信息。"
// 参数<1>: &CPU信息 MAKELONG(0x03, 0), 参数说明: "待取得的CPU信息。"
EAPI_EXTERN_C void eapi_GetCpuInfo_17_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[0].m_ppCompoundData;

}

