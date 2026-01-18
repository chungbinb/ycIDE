#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立体).读出数据, 命令说明: "从当前资源中读出一块数据。"
// 参数<1>: 立体区域 MAKELONG(0x23, 0), 参数说明: "要读的立体区域。"
// 参数<2>: &数据 SDT_BIN, 参数说明: "返回的数据。"
OGRELIB_EXTERN_C void ogrelib_ReadData_99_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    LPBYTE*  arg2 = pArgInf[2].m_ppBin;

}

