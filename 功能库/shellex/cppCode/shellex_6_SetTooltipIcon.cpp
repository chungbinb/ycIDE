#include "..\include_shellex_header.h"

// 调用格式: _SDT_NULL 置提示图标, 命令说明: "设置所有控件的提示标签的显示图标。"
// 参数<1>: 图标类型 SDT_INT, 参数说明: "提示标签的图标类型，0:无图标,1:信息,2:警告,3:错误"
// 参数<2>: 图标标题文本 _SDT_ALL, 参数说明: "提示标签的图标标题文本，如果为空文本，则表示无图标。"
SHELLEX_EXTERN_C void shellex_SetTooltipIcon_6_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    PVOID    arg2 = pArgInf[1].m_pByte;

}

