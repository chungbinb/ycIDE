#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT []取网络共享资源列表, 命令说明: "取局域网中指定计算机的共享资源列表。成功返回列表数组；失败返回空。"
// 参数<1>: [计算机 SDT_TEXT], 参数说明: "计算机名称，为空则取所有计算机的共享资源。"
// 参数<2>: [共享资源类型 SDT_INT], 参数说明: "共享资源类型，可以为以下常量值之一：0、#接口常量.所有共享资源；1、#接口常量.共享目录或文件；2、#接口常量.共享打印机。"
EAPI_EXTERN_C void eapi_GetShareResourceList_81_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    INT      arg2 = pArgInf[1].m_int;

}

