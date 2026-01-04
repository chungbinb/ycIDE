#include "..\include_ethread_header.h"

// 调用格式: SDT_BOOL 等待线程, 命令说明: "等待，直到指定线程结束或等待超时才返回。返回真表示等待成功。注意，在Linux下暂不支持超时等待。"
// 参数<1>: 线程句柄 SDT_INT, 参数说明: "可通过“启动线程”的第三个参数获取线程句柄。"
// 参数<2>: [等待时间 SDT_INT], 参数说明: "指定等待时间，单位为毫秒。如果为0或负数，或被省略，表示不限定等待时间。"
ETHREAD_EXTERN_C void ethread_WaitThread_5_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

