#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT []取网络计算机列表, 命令说明: "取局域网中指定工作组的计算机列表。成功返回列表数组；失败返回空。"
// 参数<1>: [工作组名称 SDT_TEXT], 参数说明: "工作组名称，为空则取所有计算机。"
EAPI_EXTERN_C void eapi_GetComputerList_51_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

