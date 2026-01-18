#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 取消网络驱动器映射, 命令说明: "取消指定的网络驱动器映射。成功返回真；"
// 参数<1>: 驱动器 SDT_TEXT, 参数说明: "映射到本地的驱动器名。"
EAPI_EXTERN_C void eapi_CancelNetConnection_82_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

