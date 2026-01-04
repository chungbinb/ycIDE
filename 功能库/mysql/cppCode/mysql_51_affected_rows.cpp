#include "..\include_mysql_header.h"

// 调用格式: SDT_INT64 取影响行数, 命令说明: "返回一个长整数，表示刚才某个非查询SQL语句（update,delete,insert）执行后所影响到的记录数，即：返回上次UPDATE更改的行数，上次DELETE删除的行数，或上次INSERT语句插入的行数。返回值大于0表明受影响或检索的行数；返回值为0表示UPDATE语句未更新记录，在查询中没有与WHERE匹配的行，或未执行查询；返回值为-1表示未取到实际影响行数。"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
MYSQL_EXTERN_C void mysql_affected_rows_51_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

