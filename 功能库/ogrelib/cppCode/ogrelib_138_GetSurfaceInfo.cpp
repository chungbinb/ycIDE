#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (三维页面).取页面信息, 命令说明: "取页面的描述信息（包括页面宽度、高度、格式、内存分配类型等信息）。"
// 参数<1>: 锁定类型 SDT_INT, 参数说明: "指定锁定的类型。"
// 参数<2>: [矩形区域 MAKELONG(0x01, 0)], 参数说明: "指定要锁定的页面矩形区域。为空则锁定整个页面。"
OGRELIB_EXTERN_C void ogrelib_GetSurfaceInfo_138_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

