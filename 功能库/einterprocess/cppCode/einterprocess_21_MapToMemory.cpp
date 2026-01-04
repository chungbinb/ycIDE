#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL (内存映射文件).映射到内存, 命令说明: "将一个创建完毕的或已打开的内存映射文件的全部或一部分映射到进程的内存空间中。成功返回真，失败返回假"
// 参数<1>: 起始位置 SDT_INT64, 参数说明: "映射到内存的文件起始位置。注意该值一定要是系统分配内存的分配粒度。windows的分配粒度是64k，所以该值一定要是64k的倍数，否则会执行出错。起始位置的值从0开始"
// 参数<2>: 大小 SDT_INT, 参数说明: "映射到内存的字节数量。该值等于0代表从起始位置开始文件其余部分全部映射到内存"
EINTERPROCESS_EXTERN_C void einterprocess_MapToMemory_21_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT64    arg1 = pArgInf[1].m_int64;
    INT      arg2 = pArgInf[2].m_int;

}

