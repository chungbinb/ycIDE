#include "..\include_exmlrpc_header.h"

// 调用格式: _SDT_NULL (远程服务).停止, 命令说明: "停止正在工作的服务程序。由于要释放大量的系统资源，该方法执行过程中会有一点延时，应尽量避免在该方法执行过程中，强制关闭主程序。为了正确使用该方法，建议把该方法放在程序的主线程中，同步执行该方法和关闭主程序"
// 无参数
EXMLRPC_EXTERN_C void exmlrpc_StopServer_2_exmlrpc(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

