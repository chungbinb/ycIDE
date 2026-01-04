#include "..\include_esslayer_header.h"

// 调用格式: SDT_TEXT (保密服务器).取客户IP, 命令说明: "取客户端IP地址，成功返回客户端IP和端口的文本形式（形如“192.168.0.8:1234”），失败返回空字符串。注意：在连接已经断开的情况下，只有在服务程序的“处理函数”中第一时间（即收到断开通知时）调用本方法，才能获取用户IP；而且，由于连接已经断开，该IP已没有实际意义，仅用于标识。"
// 参数<1>: 客户句柄 SDT_INT, 参数说明: "客户的句柄值"
ESSLAYER_EXTERN_C void esslayer_GetClientIPAddr_7_esslayer(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

