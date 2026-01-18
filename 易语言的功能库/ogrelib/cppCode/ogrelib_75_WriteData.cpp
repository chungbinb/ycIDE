#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (纹理).写入数据, 命令说明: "在当前资源中写入一块数据。"
// 参数<1>: 级别 SDT_INT, 参数说明: "要写入的级别。"
// 参数<2>: 区域 MAKELONG(0x01, 0), 参数说明: "要写入的区域。"
// 参数<3>: 数据 SDT_BIN, 参数说明: "要写入的数据,注：数据必须与区域匹配。"
OGRELIB_EXTERN_C void ogrelib_WriteData_75_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    LPBYTE   arg3 = pArgInf[3].m_pBin;

}

