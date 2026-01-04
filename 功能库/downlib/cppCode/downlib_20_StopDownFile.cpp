#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (下载对象).停止指定任务, 命令说明: "停止一个下载文件的任务,当前支持HTTP，FTP协议。成功返回真，失败返回假"
// 参数<1>: &任务数据 SDT_BIN, 参数说明: "本参数是用于保存下载任务的相关数据的"
// 参数<2>: 执行方式 SDT_INT, 参数说明: "本参数无意义"
DOWNLIB_EXTERN_C void downlib_StopDownFile_20_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE*  arg1 = pArgInf[1].m_ppBin;
    INT      arg2 = pArgInf[2].m_int;

}

