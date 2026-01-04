#include "..\include_shellex_header.h"

// 调用格式: _SDT_NULL 置提示底色, 命令说明: "设置所有控件的提示标签的背景颜色。"
// 参数<1>: 背景底色 SDT_INT, 参数说明: "新的提示标签的背景颜色，-1表示系统背景颜色。"
SHELLEX_EXTERN_C void shellex_SetTooltipBColor_3_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

