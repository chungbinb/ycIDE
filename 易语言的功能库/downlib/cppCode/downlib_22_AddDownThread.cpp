#include "..\include_downlib_header.h"

// 调用格式: SDT_BOOL (下载对象).增加线程, 命令说明: "增加一个下载线程,当前支持HTTP，FTP，MMS协议。如果最大块的长度小于接收大小的2倍时将不能开启新线程注意：下载线程原则上可以无限增加，但是请您根据您的实际情况谨慎使用。不要过多或过与频繁的增加线程，否则不但不能提高效率，相反会影响系统的稳定"
// 无参数
DOWNLIB_EXTERN_C void downlib_AddDownThread_22_downlib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

