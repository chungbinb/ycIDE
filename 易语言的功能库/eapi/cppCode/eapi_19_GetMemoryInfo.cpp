#include "..\include_eapi_header.h"

// 调用格式: SDT_INT 取内存容量信息, 命令说明: "取内存容量信息，单位为M（兆）。"
// 参数<1>: 内存类型 SDT_INT, 参数说明: "可以被省略。要取得的内存类型，可以为以下常量值之一：0、#接口常量.总物理内存；1、#接口常量.可用物理内存；2、#接口常量.总虚拟内存；3、#接口常量.可用虚拟内存；4、#接口常量.总页面文件；5、#接口常量.可用页面文件。"
EAPI_EXTERN_C void eapi_GetMemoryInfo_19_eapi(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

