#include "..\include_mysql_header.h"

// 本命令被隐藏, 原始名字 = "当前行号"
// 调用格式: SDT_INT64 当前行号, 命令说明: "获得记录集指针指向的行的位置"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
MYSQL_EXTERN_C void mysql_row_tell_8_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

