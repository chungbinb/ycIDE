#include "..\include_mysql_header.h"

// 本命令被隐藏, 原始名字 = "返回服务器状态"
// 调用格式: SDT_BOOL 返回服务器状态, 命令说明: "返回一个服务器状态字符串"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: &状态信息 SDT_TEXT, 参数说明: "本参数为状态信息字符串"
MYSQL_EXTERN_C void mysql_mysql_status_50_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR*   arg2 = pArgInf[1].m_ppText;

}

