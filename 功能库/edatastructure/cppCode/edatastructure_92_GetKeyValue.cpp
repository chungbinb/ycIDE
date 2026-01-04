#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (表).取键值, 命令说明: "如果对象中没有指定的键，则返回假。"
// 参数<1>: 键 SDT_BIN, 参数说明: NULL
// 参数<2>: &值 MAKELONG(0x01, 0), 参数说明: NULL
EDATASTRUCTURE_EXTERN_C void edatastructure_GetKeyValue_92_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    void**   arg2 = pArgInf[2].m_ppCompoundData;

}

