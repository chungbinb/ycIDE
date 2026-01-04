#include "..\include_shellex_header.h"

// 调用格式: _SDT_NULL 置提示文本, 命令说明: "设置指定控件的提示文本。"
// 参数<1>: 控件句柄 SDT_INT, 参数说明: "需要修改提示标签的控件的句柄。"
// 参数<2>: 提示文本 _SDT_ALL, 参数说明: "新的提示标签的文本。"
SHELLEX_EXTERN_C void shellex_SetTooltipText_2_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    PVOID    arg2 = pArgInf[1].m_pByte;

}

