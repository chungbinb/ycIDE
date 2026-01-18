#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 执行SQL语句, 命令说明: "执行一条SQL语句，执行成功返回真，失败返回假。如果执行成功可以通过“取记录集”命令得到记录集句柄。"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: Sql语句 SDT_TEXT, 参数说明: "本参数是设置查询数据库的SQL语句，如Select,Update,Insert,Delete等等语句。详细信息请参考相关SQL语言书籍"
MYSQL_EXTERN_C void mysql_mysql_query_3_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;

}

