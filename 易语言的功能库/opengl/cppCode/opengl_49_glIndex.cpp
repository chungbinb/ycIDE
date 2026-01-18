#include "..\include_opengl_header.h"

// 调用格式: _SDT_NULL 设置颜色索引, 命令说明: "设置接下来的图元的颜色。能在“开始画”和“停止画”之间执行本语句。只有在颜色索引模式下有效。"
// 参数<1>: 颜色索引号 SDT_FLOAT, 参数说明: NULL
OPENGL_EXTERN_C void opengl_glIndex_49_opengl(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    FLOAT    arg1 = pArgInf[0].m_float;

}

