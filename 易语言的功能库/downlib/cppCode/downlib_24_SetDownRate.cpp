#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (下载对象).限制速度, 命令说明: "限制下载速度。"
// 参数<1>: 速度 SDT_INT, 参数说明: "限制后的速度，单位为 字节/秒"
DOWNLIB_EXTERN_C void downlib_SetDownRate_24_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

