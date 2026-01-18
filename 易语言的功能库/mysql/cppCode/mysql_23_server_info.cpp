#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 取服务器版本, 命令说明: "取得MYSQL服务器的版本信息并设置结果变量，成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: &结果变量 SDT_TEXT, 参数说明: "本参数存放调用本命令后的结果"
MYSQL_EXTERN_C void mysql_server_info_23_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR*   arg2 = pArgInf[1].m_ppText;

}

