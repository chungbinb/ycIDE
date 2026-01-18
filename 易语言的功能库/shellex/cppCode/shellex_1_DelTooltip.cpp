#include "..\include_shellex_header.h"

// 调用格式: _SDT_NULL 删除提示, 命令说明: "删除一个控件的提示标签。"
// 参数<1>: 控件句柄 SDT_INT, 参数说明: "需要删除提示标签的控件的句柄。"
SHELLEX_EXTERN_C void shellex_DelTooltip_1_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

