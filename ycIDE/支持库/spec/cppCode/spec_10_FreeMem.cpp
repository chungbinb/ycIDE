#include "..\include_spec_header.h"

// 调用格式: _SDT_NULL 释放内存, 命令说明: "释放由“申请内存”所获取的内存区域。内存区域被释放后，不允许再进行读写操作。"
// 参数<1>: 欲释放的内存地址 SDT_INT, 参数说明: "本参数应当是“申请内存”命令的返回值。"
SPEC_EXTERN_C void spec_FreeMem_10_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

