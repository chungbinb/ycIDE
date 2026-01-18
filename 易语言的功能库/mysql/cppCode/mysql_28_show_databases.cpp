#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 取库名列表, 命令说明: "取得数据库名称的列表。成功返回真，失败返回假。结果可以通过“取记录集”取得"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
MYSQL_EXTERN_C void mysql_show_databases_28_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

