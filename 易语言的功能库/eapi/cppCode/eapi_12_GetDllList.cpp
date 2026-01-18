#include "..\include_eapi_header.h"

// 调用格式: SDT_TEXT []取正在使用DLL列表, 命令说明: "取所有正在使用的DLL列表。成功返回DLL列表数组；失败返回空。"
// 参数<1>: [进程标识符 SDT_INT], 参数说明: "要终止进程的进程ID、进程名称或窗口标题。置-1则取所有正在使用DLL列表。"
EAPI_EXTERN_C void eapi_GetDllList_12_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

