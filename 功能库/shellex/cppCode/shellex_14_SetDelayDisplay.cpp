#include "..\include_shellex_header.h"

// 调用格式: _SDT_NULL 置提示延迟, 命令说明: "设置提示框延迟显示的时间 也是就鼠标放上去多长时间显示 "
// 参数<1>: 延迟时间 SDT_INT, 参数说明: "欲显示时鼠标需悬停的时间"
SHELLEX_EXTERN_C void shellex_SetDelayDisplay_14_shellex(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

