#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT []取网络工作组列表, 命令说明: "取局域网中指定网络类型的工作组列表。成功返回列表数组；失败返回空。"
// 参数<1>: [网络类型 SDT_TEXT], 参数说明: "网络类型名，为空则取所有工作组。"
EAPI_EXTERN_C void eapi_GetGroupList_50_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

