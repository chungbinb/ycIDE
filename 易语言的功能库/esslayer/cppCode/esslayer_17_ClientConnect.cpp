#include "..\include_esslayer_header.h"

// 调用格式: SDT_BOOL (保密客户端).连接, 命令说明: "连接到保密服务器，成功真，失败返回假"
// 参数<1>: 服务器地址 SDT_TEXT, 参数说明: "本参数是欲连接的服务器的IP地址，也可以是服务器的域名或者是内网的计算机名"
// 参数<2>: 端口 SDT_INT, 参数说明: "待连接服务器的端口"
// 参数<3>: 处理函数 SDT_SUB_PTR, 参数说明: "本参数作为客户程序的事件触发方法，如果有默认事件产生，系统将自动调用的该函数。例如服务器发送数据到客户端，或者是服务器主动或意外断开与客户端的连接，系统都会自动执行该方法。此处理函数可以没有参数，或有一个整数型参数，见下一个参数的说明。"
// 参数<4>: [处理函数参数 SDT_INT], 参数说明: "如果提供了本参数，则处理函数必须接收一个整数型参数，此参数值将被传递到处理函数中。"
ESSLAYER_EXTERN_C void esslayer_ClientConnect_17_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    INT      arg2 = pArgInf[2].m_int;
    DWORD    arg3 = pArgInf[3].m_dwSubCodeAdr;
    INT      arg4 = pArgInf[4].m_int;

}

