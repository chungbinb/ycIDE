#include "..\include_shellex_header.h"

// 调用格式: SDT_INT 执行子程序, 命令说明: "执行一个易语言的子程序。"
// 参数<1>: 指针地址 SDT_SUB_PTR, 参数说明: "指针的地址"
// 参数<2>: [参数 _SDT_ALL], 参数说明: "参数"
SHELLEX_EXTERN_C void shellex_CallPtr_13_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    DWORD    arg1 = pArgInf[0].m_dwSubCodeAdr;
    PVOID    arg2 = pArgInf[1].m_pByte;

}

