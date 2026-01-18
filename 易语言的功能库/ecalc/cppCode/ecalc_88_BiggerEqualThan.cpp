#include "..\include_ecalc_header.h"

// 调用格式: SDT_BOOL (大数).大于等于, 命令说明: "判断本对象是否大于等于指定对象。如大于等于，则返回真；否则，返回假"
// 参数<1>: 比较数据 MAKELONG(0x0A, 0), 参数说明: "本参数提供待和本大数数据比较的大数"
ECALC_EXTERN_C void ecalc_BiggerEqualThan_88_ecalc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

