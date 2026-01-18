#include "..\include_mysql_header.h"

// 调用格式: _SDT_NULL 释放记录集, 命令说明: "释放记录集之后，原记录集句柄不可再用。"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
MYSQL_EXTERN_C void mysql_free_result_5_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

