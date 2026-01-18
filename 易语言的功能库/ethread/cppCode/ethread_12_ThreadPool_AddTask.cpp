#include "..\include_ethread_header.h"

// 调用格式: SDT_BOOL (线程池).添加任务, 命令说明: "添加线程任务, 回调函数有多少个参数就填写多少个参数, 返回是否添加成功"
// 参数<1>: 欲执行的子程序 _SDT_ALL, 参数说明: "本参数提供创建线程时欲启动的子程序, 只支持传递整数型的子程序地址或者子程序指针类型, 其他类型直接调用失败"
// 参数<2>: [线程参数 _SDT_ALL], 参数说明: "传递到线程里的参数, 支持11个易语言的基础数据类型, 遇到空参数或者不是基础数据类型则视为结束传参"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
ETHREAD_EXTERN_C void ethread_ThreadPool_AddTask_12_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;
    PVOID    arg2 = pArgInf[2].m_pByte;

}

