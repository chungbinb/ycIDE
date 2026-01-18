#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).进入监视, 命令说明: "进入与指定对象所引用的基本Java对象相关联的监视程序。成功返回“真”，失败返回“假”。\r\n    每个Java对象都有一个相关联的监视程序。如果当前线程已经拥有与指定对象相关联的监视程序，它将使指示该线程进入监视程序次数的监视程序计数器增1。如果与指定对象相关联的监视程序并非由某个线程所拥有，则当前线程将变为该监视程序的所有者，同时将该监视程序的计数器设置为1。如果另一个线程已拥有与指定对象关联的监视程序，则在监视程序被释放前当前线程将处于等待状态。监视程序被释放后，当前线程将尝试重新获得所有权。"
// 参数<1>: 对象标志符 SDT_INT, 参数说明: "可为常规Java对象或类对象。"
JAVALIB_EXTERN_C void javalib_MonitorEnter_63_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

