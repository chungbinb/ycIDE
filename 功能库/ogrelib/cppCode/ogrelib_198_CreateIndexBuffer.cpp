#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建索引缓冲, 命令说明: NULL
// 参数<1>: 长度 SDT_INT, 参数说明: "索引缓冲区的长度（以字节为单位）。"
// 参数<2>: 资源用途 MAKELONG(0x37, 0), 参数说明: "为“资源用途”常量值之一。"
// 参数<3>: 内存池类型 MAKELONG(0x39, 0), 参数说明: "为“内存池类型”常量值之一。"
// 参数<4>: 索引缓冲 MAKELONG(0x18, 0), 参数说明: "要创建的索引缓冲资源。"
OGRELIB_EXTERN_C void ogrelib_CreateIndexBuffer_198_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    void*    arg3 = pArgInf[3].m_pCompoundData;
    void*    arg4 = pArgInf[4].m_pCompoundData;

}

