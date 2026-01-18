#include "..\include_eapi_header.h"

// 调用格式: SDT_BOOL 映射网络驱动器, 命令说明: "将指定的网络资源映射到本地驱动器。成功返回真；失败返回假。"
// 参数<1>: 网络资源路径 SDT_TEXT, 参数说明: "要映射到本地的网络资源路径。"
// 参数<2>: 驱动器 SDT_TEXT, 参数说明: "映射到本地的驱动器名。"
EAPI_EXTERN_C void eapi_NetAddConnection_79_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

