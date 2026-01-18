#include "..\include_isapi_header.h"

// 调用格式: SDT_INT (互联网筛选器).申请内存, 命令说明: "执行成功返回申请到的内存地址，否则返回假。申请到的内存会在连接结束时被自动释放。"
// 参数<1>: 欲申请的内存长度 SDT_INT, 参数说明: NULL
ISAPI_EXTERN_C void isapi_FilterAllocMem_70_isapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

