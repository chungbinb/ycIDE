#include "..\include_mysql_header.h"

// 调用格式: SDT_INT 取字段总数, 命令说明: "取得记录集的字段总数，返回字段总数"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
MYSQL_EXTERN_C void mysql_num_fields_14_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

