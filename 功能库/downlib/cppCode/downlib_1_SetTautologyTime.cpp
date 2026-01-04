#include "..\include_downlib_header.h"

// 调用格式: _SDT_NULL 设重试时间, 命令说明: "设置重新尝试连接的时间"
// 参数<1>: 重试时间 SDT_INT, 参数说明: "尝试重新连接的时间"
DOWNLIB_EXTERN_C void downlib_SetTautologyTime_1_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

