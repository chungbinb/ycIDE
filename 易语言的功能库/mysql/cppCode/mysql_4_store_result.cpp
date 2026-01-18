#include "..\include_mysql_header.h"

// 调用格式: SDT_INT 取记录集, 命令说明: "得到查询后的记录的集合(简称：记录集)。返回记录集句柄。当使用完记录集后要通过“释放记录集”命令来释放记录集"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
MYSQL_EXTERN_C void mysql_store_result_4_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

