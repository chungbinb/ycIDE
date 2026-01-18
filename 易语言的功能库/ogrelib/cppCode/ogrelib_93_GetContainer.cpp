#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (立体).取父容器, 命令说明: "获取这个立体的父容器。"
// 参数<1>: &父容器 MAKELONG(0x11, 0), 参数说明: "返回的父容器。"
OGRELIB_EXTERN_C void ogrelib_GetContainer_93_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppCompoundData;

}

