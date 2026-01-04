#include "..\include_edatastructure_header.h"

// 调用格式: SDT_BOOL (表).加入键值对, 命令说明: "将“键”和“值”两者配对加入到对象中。如果已存在相同的“键”，则新“值”将覆盖旧“值”。执行成功返回真，失败返回假。"
// 参数<1>: 键 SDT_BIN, 参数说明: NULL
// 参数<2>: 值 MAKELONG(0x01, 0), 参数说明: NULL
EDATASTRUCTURE_EXTERN_C void edatastructure_AddKeyValue_91_edatastructure(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

