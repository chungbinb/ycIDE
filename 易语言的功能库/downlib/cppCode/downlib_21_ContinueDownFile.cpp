#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (下载对象).继续下载任务, 命令说明: "继续一个下载文件的任务,当前支持HTTP，FTP，MMS，RTSP协议。成功返回真，失败返回假。注意：继续下载时默认线程数不是根据任务参数中的默认线程数进行设置的，而是根据任务信息中的没有完成的断点信息的数量进行设置的。断点的数量则是根据任务自本次运行以前开启的线程的总数减去已经完成下载任务的线程数获得的"
// 参数<1>: 配置信息 MAKELONG(0x04, 0), 参数说明: "配置任务相关信息"
// 参数<2>: 任务数据 SDT_BIN, 参数说明: "本参数是通过“停止任务”命令所获得的任务数据"
DOWNLIB_EXTERN_C void downlib_ContinueDownFile_21_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;
    LPBYTE   arg2 = pArgInf[2].m_pBin;

}

