#include "..\include_ecalc_header.h"

// 本命令被隐藏, 原始名字 = "复数复制构造", 本命令为复制构造函数
// 调用格式: _SDT_NULL (复数运算).复数复制构造, 命令说明: NULL
// 参数<1>: UL MAKELONG(0x01, 0), 参数说明: NULL
ECALC_EXTERN_C void ecalc_ComplexCopyCmd_111_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

