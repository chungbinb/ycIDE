#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (下载对象).取任务数据, 命令说明: "获得任务数据，在指定的时间内执行本命令来取得任务数据。如果下载任务出现异常，可以通过“继续下载任务”来继续任务"
// 参数<1>: &任务数据 SDT_BIN, 参数说明: "本参数是用于保存下载任务的相关数据的"
DOWNLIB_EXTERN_C void downlib_GetThreadSize_27_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE*  arg1 = pArgInf[1].m_ppBin;

}

