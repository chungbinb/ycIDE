#include "..\include_javalib_header.h"

// 调用格式: SDT_BOOL (Java本地接口).退出监视, 命令说明: "当前线程必须是与指定对象所引用的基本Java对象相关联的监视程序的所有者。线程将使指示进入监视程序次数的计数器减1。如果计数器的值变为0，当前线程释放监视程序。成功返回“真”，失败返回“假”。"
// 参数<1>: 对象标志符 SDT_INT, 参数说明: "可为常规Java对象或类对象。"
JAVALIB_EXTERN_C void javalib_MonitorExit_64_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

