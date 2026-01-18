#include "..\include_ecalc_header.h"

// 调用格式: MAKELONG(0x0A, 0) (大数).求余, 命令说明: "将本对象与指定对象做求余运算，并返回运算后的结果"
// 参数<1>: 计算数据 MAKELONG(0x0A, 0), 参数说明: "本参数提供待和本数据计算的大数数据"
ECALC_EXTERN_C void ecalc_Mod_97_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

