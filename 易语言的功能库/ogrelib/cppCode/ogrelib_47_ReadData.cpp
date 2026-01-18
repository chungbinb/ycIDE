#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立方体纹理).读出数据, 命令说明: "从当前资源中读出一块数据。"
// 参数<1>: 哪一面 MAKELONG(0x3B, 0), 参数说明: "本参数表示要取立方体哪一面。"
// 参数<2>: 级别 SDT_INT, 参数说明: "要读的级别。"
// 参数<3>: 区域 MAKELONG(0x01, 0), 参数说明: "要读的区域。"
// 参数<4>: &数据 SDT_BIN, 参数说明: "读入数据存放的地方。"
OGRELIB_EXTERN_C void ogrelib_ReadData_47_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    LPBYTE*  arg4 = pArgInf[4].m_ppBin;

}

