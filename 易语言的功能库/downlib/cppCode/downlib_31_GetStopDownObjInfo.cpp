#include "..\include_downlib_header.h"

// 调用格式: SDT_INT (下载对象).取任务数据项, 命令说明: "获得通过“停止指定任务”命令返回的任务信息中的各类数据。失败返回-1"
// 参数<1>: 任务数据 SDT_BIN, 参数说明: "本参数是通过“停止任务”命令所获得的任务数据"
// 参数<2>: 数据类型 SDT_INT, 参数说明: "需要获得的任务信息的类型。如果”类型参数“作为指定具体断点序号使用，那么它从1开始不大于断点个数,1 待下载文件总长度，2 已下载总长度,3 断点个数,4 指定断点开始下载位置,类型参数为序号,5 指定断点待下载长度,类型参数为序号,6 指定断点已下载长度,类型参数为序号"
// 参数<3>: 类型参数 SDT_INT, 参数说明: "根据”数据类型“参数的不同而不同。当需要查询的信息类型没有指定本参数的意义时，本参数无意义"
DOWNLIB_EXTERN_C void downlib_GetStopDownObjInfo_31_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE   arg1 = pArgInf[1].m_pBin;
    INT      arg2 = pArgInf[2].m_int;
    INT      arg3 = pArgInf[3].m_int;

}

