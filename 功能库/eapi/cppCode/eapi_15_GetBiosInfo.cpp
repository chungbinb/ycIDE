#include "..\include_eapi_header.h"

// 调用格式: _SDT_NULL 取BIOS信息, 命令说明: "取BIOS信息。"
// 参数<1>: &BIOS信息 MAKELONG(0x06, 0), 参数说明: "待取得的系统信息。"
EAPI_EXTERN_C void eapi_GetBiosInfo_15_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[0].m_ppCompoundData;

}

