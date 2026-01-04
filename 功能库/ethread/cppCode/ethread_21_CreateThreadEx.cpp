#include "..\include_ethread_header.h"

// 调用格式: SDT_INT 启动线程Ex, 命令说明: "创建并启动一条线程, 可重复使用以创建多条线程, 成功返回线程ID, 失败返回0"
// 参数<1>: 欲执行的子程序 _SDT_ALL, 参数说明: "本参数提供创建线程时欲启动的子程序, 只支持传递整数型的子程序地址或者子程序指针类型, 其他类型直接调用失败"
// 参数<2>: [创建标志 SDT_INT], 参数说明: "请参考 #启动线程_ 开头常量, 默认为 #启动线程_默认, 多个标志请使用 位或(值1, 值2, 值3, ...)"
// 参数<3>: [&线程句柄 SDT_INT], 参数说明: "如果提供了本参数，将向参数变量写入线程句柄（Windows下为HANDLE，Linux下为pthread_t），请在适当的时机关闭该句柄。如果不提供本参数接收线程句柄，内部将自动处理线程句柄。"
// 参数<4>: [线程参数 _SDT_ALL], 参数说明: "传递到线程里的参数, 支持11个易语言的基础数据类型, 遇到空参数或者不是基础数据类型则视为结束传参"
// 变长参数, 命令参数表中最后一个参数可以被重复添加
ETHREAD_EXTERN_C void ethread_CreateThreadEx_21_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;
    INT      arg2 = pArgInf[1].m_int;
    PINT     arg3 = pArgInf[2].m_pInt;
    PVOID    arg4 = pArgInf[3].m_pByte;

}

