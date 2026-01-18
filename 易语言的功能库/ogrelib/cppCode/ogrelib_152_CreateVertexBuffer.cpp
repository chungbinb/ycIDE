#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (设备).创建顶点缓冲, 命令说明: "创建顶点缓冲。"
// 参数<1>: 缓冲区大小 SDT_INT, 参数说明: "将指定矩形区域的后台缓冲页面显示到前台。默认为将整个后台缓冲页面显示到前台。"
// 参数<2>: 用途 MAKELONG(0x37, 0), 参数说明: "为“资源用途”常量值之一。"
// 参数<3>: 灵活顶点格式 SDT_INT, 参数说明: NULL
// 参数<4>: 内存池类型 MAKELONG(0x39, 0), 参数说明: "为“内存池类型”常量值之一。"
// 参数<5>: 顶点缓冲 MAKELONG(0x13, 0), 参数说明: "要创建的顶点缓冲。"
OGRELIB_EXTERN_C void ogrelib_CreateVertexBuffer_152_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;
    INT      arg3 = pArgInf[3].m_int;
    void*    arg4 = pArgInf[4].m_pCompoundData;
    void*    arg5 = pArgInf[5].m_pCompoundData;

}

