#include "..\include_einterprocess_header.h"

// 调用格式: SDT_BOOL (邮槽客户机).创建, 命令说明: "邮槽客户机只能向邮槽中写入数据，不能从中读出数据。创建成功返回真，否则返回假"
// 参数<1>: 服务器名称 SDT_TEXT, 参数说明: "欲连接的邮槽服务器名称，也可以是局域网的域名，这时表示客户机要连接这个局域网域内的所有计算机上名称为参数2的邮槽。该参数也可以是“.”，表示连接本机上的名称为参数2的邮槽。该参数也可以是“*”，表示连接系统主域内的所有计算机上的名称为参数2的邮槽"
// 参数<2>: 邮槽名称 SDT_TEXT, 参数说明: "欲连接服务器上邮槽的名称"
EINTERPROCESS_EXTERN_C void einterprocess_CreateMailslotClient_14_einterprocess(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    LPSTR    arg2 = pArgInf[2].m_pText;

}

