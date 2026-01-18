#include "..\include_shellex_header.h"

// 调用格式: SDT_INT 取指针地址, 命令说明: "返回一个结构的指针地址。"
// 参数<1>: 参数 _SDT_ALL, 参数说明: "要取得地址的一个参数"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
SHELLEX_EXTERN_C void shellex_GetAddr_12_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;

}

