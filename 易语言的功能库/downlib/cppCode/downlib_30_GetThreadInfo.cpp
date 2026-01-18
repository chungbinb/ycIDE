#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (下载对象).取线程信息, 命令说明: "获得线程信息，通常用于多个对象使用相同的日志或交互回调函数时，在日志或交互回调函数中确定线程"
// 参数<1>: &线程信息数组 数组 SDT_INT, 参数说明: "本参数是用于保存线程信息数组"
DOWNLIB_EXTERN_C void downlib_GetThreadInfo_30_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void**   arg1 = pArgInf[1].m_ppAryData;

}

