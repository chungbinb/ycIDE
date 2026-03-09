#include "..\include_spec_header.h"

// 调用格式: _SDT_NULL 延迟, 命令说明: "延迟一段时间，然后继续执行后面的程序代码。与易语言核心支持库中的“延时()”命令不同的是，本命令在等待期间允许用户进行其它操作（如点击按钮等）。"
// 参数<1>: 等待时间 SDT_INT, 参数说明: "单位为毫秒。提示：1000毫秒等于1秒。"
SPEC_EXTERN_C void spec_Delay_7_spec(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

