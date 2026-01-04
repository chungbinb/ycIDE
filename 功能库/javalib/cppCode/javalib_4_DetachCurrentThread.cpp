#include "..\include_javalib_header.h"

// 本命令被隐藏, 原始名字 = "断开"
// 调用格式: SDT_BOOL (Java虚拟机).断开, 命令说明: "断开当前线程与Java虚拟机之间的连接，释放该线程占用的所有Java监视程序，通知所有等待该线程终止的Java线程。主线程（即创建Java虚拟机的线程）不能使用本方法断开与虚拟机之间的连接，而必须调用“销毁()”方法来卸载整个虚拟机。成功返回“真”，失败返回“假”。"
// 无参数
JAVALIB_EXTERN_C void javalib_DetachCurrentThread_4_javalib(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

