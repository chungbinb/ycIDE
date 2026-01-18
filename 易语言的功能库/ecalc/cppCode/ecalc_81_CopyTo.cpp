#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (大数).复制到, 命令说明: "复制当前数据到另一个目标存储大数的对象中，目标大数以前所存储的数据将被舍弃"
// 参数<1>: 目标大数 MAKELONG(0x0A, 0), 参数说明: "要复制到的目标大数"
ECALC_EXTERN_C void ecalc_CopyTo_81_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

