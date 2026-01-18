#include "..\include_ethread_header.h"

// 调用格式: SDT_BOOL 启动线程, 命令说明: "创建并启动一条线程，可重复使用以创建多条线程，成功返回真，失败返回假"
// 参数<1>: 欲执行的子程序 _SDT_ALL, 参数说明: "本参数提供创建线程时欲启动的子程序，根据是否需要传递参数数据，该子程序必须没有或具有一个整数型参数，否则将出错"
// 参数<2>: [参数数据 SDT_INT], 参数说明: "本参数提供线程被启动时传递到欲启动子程序的整数数据。如果本参数未被省略，此时被启动子程序必须接收一个整数型参数，否则将出错"
// 参数<3>: [&线程句柄 SDT_INT], 参数说明: "如果提供了本参数，将向参数变量写入线程句柄（Windows下为HANDLE，Linux下为pthread_t），请在适当的时机关闭该句柄。如果不提供本参数接收线程句柄，内部将自动处理线程句柄。"
ETHREAD_EXTERN_C void ethread_CreateThread_0_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;
    INT      arg2 = pArgInf[1].m_int;
    PINT     arg3 = pArgInf[2].m_pInt;

}

