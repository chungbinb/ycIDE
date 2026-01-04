#include "..\include_ogrelib_header.h"

// 调用格式: SDT_BOOL (基础纹理).置过滤类型, 命令说明: "设置生成多纹理子级别用的过滤类型。"
// 参数<1>: 过滤类型 MAKELONG(0x2F, 0), 参数说明: "要设置的过滤类型，值可以为“纹理过滤类型”常量数据类型中的值。"
OGRELIB_EXTERN_C void ogrelib_SetAutoGenFilterType_23_ogrelib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

