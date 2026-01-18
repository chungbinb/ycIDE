#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立体纹理).写入数据, 命令说明: "在当前资源中写入一块数据。"
// 参数<1>: 级别 SDT_INT, 参数说明: "要写入数据的级别。"
// 参数<2>: 立体区域 MAKELONG(0x23, 0), 参数说明: "要写入的立体区域。"
// 参数<3>: 数据 SDT_BIN, 参数说明: "要写入的立体区域的数据，注：数据和区域应该是对应的。"
OGRELIB_EXTERN_C void ogrelib_WriteData_119_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    LPBYTE   arg3 = pArgInf[3].m_pBin;

}

