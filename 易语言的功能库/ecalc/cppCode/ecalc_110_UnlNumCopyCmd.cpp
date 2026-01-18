#include "..\include_ecalc_header.h"

// 本命令被隐藏, 原始名字 = "大数复制构造", 本命令为复制构造函数
// 调用格式: _SDT_NULL (大数).大数复制构造, 命令说明: NULL
// 参数<1>: UL MAKELONG(0x0A, 0), 参数说明: "省略"
ECALC_EXTERN_C void ecalc_UnlNumCopyCmd_110_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

