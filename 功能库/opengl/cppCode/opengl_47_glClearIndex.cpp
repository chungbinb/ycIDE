#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 清除颜色索引, 命令说明: "设置颜色索引的默认值。不执行本命令前颜色索引号为0。"
// 参数<1>: 颜色索引号 SDT_FLOAT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glClearIndex_47_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[0].m_float;

}

