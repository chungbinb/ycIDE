#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (下载对象).增加新任务, 命令说明: "增加一个下载文件的任务,当前支持HTTP，FTP，MMS，RTSP协议。成功返回真，失败返回假。注意：默认线程原则上没有限制。但使用时应根据具体情况设置。否则不但起不到提高速度的作用还会影响系统的稳定"
// 参数<1>: 配置信息 MAKELONG(0x04, 0), 参数说明: "配置任务相关信息"
DOWNLIB_EXTERN_C void downlib_AddDownFile_19_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    void*    arg1 = pArgInf[1].m_pCompoundData;

}

