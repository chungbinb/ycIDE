#include "..\include_esslayer_header.h"

// 调用格式: SDT_BOOL (保密服务器).启动, 命令说明: "启动保密服务器，绑定到指定端口，成功真，失败返回假。在启动之前必须设置相应的参数，否则启动失败"
// 参数<1>: 端口号 SDT_INT, 参数说明: "服务器的端口号"
// 参数<2>: 处理函数 SDT_SUB_PTR, 参数说明: "本参数作为服务程序的事件触发方法，如果有默认事件产生，系统将自动调用的该函数。例如客户端发送数据到服务程序，或者是有客户端连接到服务程序，或者有客户端主动或意外断开与服务程序的连接，系统都会自动执行该方法。该函数没有参数"
ESSLAYER_EXTERN_C void esslayer_StartServer_1_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;
    DWORD    arg2 = pArgInf[2].m_dwSubCodeAdr;

}

