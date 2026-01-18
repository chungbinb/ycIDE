#include "..\include_sqlite3_header.h"

// 本命令被隐藏, 原始名字 = "创建SQL函数todo"
// 调用格式: SDT_BOOL (Sqlite数据库).创建SQL函数todo, 命令说明: NULL
// 参数<1>: UL SDT_INT, 参数说明: NULL
SQLITE3_EXTERN_C void sqlite3_CreateSqlFunction_40_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

