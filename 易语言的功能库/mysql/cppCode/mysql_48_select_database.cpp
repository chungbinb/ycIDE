#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 选择库, 命令说明: "选择一个数据库为当前用户默认使用的数据库，成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 库名 SDT_TEXT, 参数说明: "本参数为要选择的数据库名"
MYSQL_EXTERN_C void mysql_select_database_48_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

