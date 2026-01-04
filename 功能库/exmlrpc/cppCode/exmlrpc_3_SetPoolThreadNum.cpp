#include "..\include_exmlrpc_header.h"

// 调用格式: SDT_BOOL (远程服务).置线程池大小, 命令说明: "设置每一个线程池内的线程数量的最大值和最小值，该项设置必须在服务器启动之前设置。服务系统拥有四个线程池，一个接收线程池一个发送线程池还有两个处理线程池，该方法是设置每一个线程池内的允许线程数量。由于线程池内的线程数量是可以随环境的变化而增加或减少的，所以该方法也就是设置线程池内的线程数量的变化区间，系统会根据该设置取最优化的值。如果不进行该项设置，系统将采用默认的设置，即最小线程数量为5，最大线程数量为20。线程数量的最小值不能小于1。设置成功返回真，失败返回假"
// 参数<1>: 最小线程数 SDT_INT, 参数说明: "线程池内的线程数量的最小值"
// 参数<2>: 最大线程数 SDT_INT, 参数说明: "线程池内的线程数量的最大值"
EXMLRPC_EXTERN_C void exmlrpc_SetPoolThreadNum_3_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    INT      arg2 = pArgInf[2].m_int;

}

