#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT 取远程网卡物理地址, 命令说明: "根据指定的IP地址取远程机器网卡的物理地址。。失败返回空。"
// 参数<1>: IP地址 SDT_TEXT, 参数说明: NULL
EAPI_EXTERN_C void eapi_GetRemoteMac_45_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

