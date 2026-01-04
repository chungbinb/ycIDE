#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).置索引缓冲, 命令说明: "设置索引缓冲数据。"
// 参数<1>: 索引缓冲 MAKELONG(0x18, 0), 参数说明: "要设置的索引缓冲数据。"
OGRELIB_EXTERN_C void ogrelib_SetIndices_165_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

