#include "..\include_sqlite3_header.h"

// 调用格式: SDT_TEXT (Sqlite数据库).取表定义, 命令说明: "返回创建指定表时所用的SQL语句（CREATE TABLE ...）。"
// 参数<1>: 表名称 SDT_TEXT, 参数说明: NULL
SQLITE3_EXTERN_C void sqlite3_GetTableSql_53_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

