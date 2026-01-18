#include "..\include_ethread_header.h"

// 调用格式: SDT_INT 等待线程Ex, 命令说明: "等待线程, 直到线程结束或者超时才返回, 返回等待的值\r\n        返回0表示线程执行完毕\r\n        返回258则表示等待超时, 线程还在运行中\r\n        返回-1表示调用失败"
// 参数<1>: 线程句柄 SDT_INT, 参数说明: "可通过\"启动线程\" 或者 \"启动线程Ex\" 或者 \"打开线程\" 获得的线程句柄"
// 参数<2>: [等待时间 SDT_INT], 参数说明: "指定等待时间, 单位为毫秒, 如果为负数, 表示不限定等待时间, 如果为0则立马返回, 默认为0"
// 参数<3>: [处理事件 SDT_BOOL], 参数说明: "是否处理窗口的消息事件, 如果是窗口线程调用, 可以设置为真, 这样窗口就不会卡死, 默认为假"
ETHREAD_EXTERN_C void ethread_WaitThreadEx_22_ethread(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    BOOL     arg3 = pArgInf[2].m_bool;

}

