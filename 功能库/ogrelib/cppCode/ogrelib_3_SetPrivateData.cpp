#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (资源).置私有数据, 命令说明: "关联一块数据到本资源上。"
// 参数<1>: 全局唯一标识符 MAKELONG(0x0D, 0), 参数说明: "此私有数据的全局唯一标识符。"
// 参数<2>: 数据 SDT_BIN, 参数说明: "想要关联到本资源上的数据。"
// 参数<3>: 标志 SDT_INT, 参数说明: "保留。"
OGRELIB_EXTERN_C void ogrelib_SetPrivateData_3_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    LPBYTE   arg2 = pArgInf[2].m_pBin;
    INT      arg3 = pArgInf[3].m_int;

}

