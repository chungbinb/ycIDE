#include "..\include_shellex_header.h"

// 调用格式: SDT_BOOL 置提示字体, 命令说明: "设置所有控件的提示标签的字体。"
// 参数<1>: 窗口句柄 SDT_INT, 参数说明: "需要修改提示标签的窗口句柄。"
// 参数<2>: 提示字体 DTP_FONT, 参数说明: "需要设置的提示字体，注意角度属性无效。"
SHELLEX_EXTERN_C void shellex_SetTooltipFont_7_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    void*    arg2 = pArgInf[1].m_pCompoundData;

}

