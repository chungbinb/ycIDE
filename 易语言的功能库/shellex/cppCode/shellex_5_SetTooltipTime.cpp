#include "..\include_shellex_header.h"

// 调用格式: _SDT_NULL 置提示时间, 命令说明: "设置所有控件的提示标签的显示时间。"
// 参数<1>: 显示时间 SDT_INT, 参数说明: "提示标签的显示时间，-1表示一直显示。"
SHELLEX_EXTERN_C void shellex_SetTooltipTime_5_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

