#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立方体纹理).写入数据, 命令说明: "在当前资源中写入一块数据。"
// 参数<1>: 哪一面 MAKELONG(0x3B, 0), 参数说明: "本参数表示要取立方体哪一面。"
// 参数<2>: 级别 SDT_INT, 参数说明: "要写入的级别。"
// 参数<3>: 区域 MAKELONG(0x01, 0), 参数说明: "要写入的区域。"
// 参数<4>: 数据 SDT_BIN, 参数说明: "要写入的数据,注：数据必须与区域匹配。"
OGRELIB_EXTERN_C void ogrelib_WriteData_46_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    INT      arg2 = pArgInf[2].m_int;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    LPBYTE   arg4 = pArgInf[4].m_pBin;

}

