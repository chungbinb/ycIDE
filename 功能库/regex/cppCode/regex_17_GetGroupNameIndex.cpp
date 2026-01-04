#include "..\include_regex_header.h"

// 调用格式: SDT_INT (正则表达式).[]取分组名称索引, 命令说明: "获取指定的正则表达式中的分组名称所在的分组索引,即(?<Name>)的name所在的分组索引"
// 参数<1>: 分组名称 SDT_TEXT, 参数说明: "本参数指定要获取的分组名称,分组命名形式为(?<name>.*?)"
REGEX_EXTERN_C void regex_GetGroupNameIndex_17_regex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

