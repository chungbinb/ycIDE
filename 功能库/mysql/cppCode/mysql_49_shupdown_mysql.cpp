#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 关闭MySql, 命令说明: "让数据库服务器关闭。连接的用户必须有shutdown权限，成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
MYSQL_EXTERN_C void mysql_shupdown_mysql_49_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

