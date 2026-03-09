#include "..\include_spec_header.h"

// 调用格式: SDT_INT 取子程序地址, 命令说明: "返回指定子程序的内存代码地址（在内联汇编代码中可以在执行本命令后立即检查EAX寄存器获得）"
// 参数<1>: 欲获取其地址的子程序 SDT_SUB_PTR, 参数说明: NULL
SPEC_EXTERN_C void spec_GetSubAddress_2_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    DWORD    arg1 = pArgInf[0].m_dwSubCodeAdr;

}

