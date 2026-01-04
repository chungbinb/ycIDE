#include "..\include_ethread_header.h"

// 调用格式: SDT_BOOL (线程池).初始化, 命令说明: "初始化线程池, 返回是否初始化成功\r\n设计理念是: 你有一次使用到了这么多条线程, 那就会使用到第二次, 所以不会自动释放空闲的线程"
// 参数<1>: [最小线程数 SDT_INT], 参数说明: "线程池最小线程数, 小于等于0则默认为5, 创建的时候就分配的线程数"
// 参数<2>: [最大线程数 SDT_INT], 参数说明: "线程池最大线程数, 小于等于0则默认为10, 最大32767条线程, 一般用不到这么多线程, 如果用到这么多线程, 那应该考虑优化程序了"
ETHREAD_EXTERN_C void ethread_ThreadPool_Init_11_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

