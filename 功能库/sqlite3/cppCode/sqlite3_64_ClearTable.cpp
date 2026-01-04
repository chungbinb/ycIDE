#include "..\include_sqlite3_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).清空表, 命令说明: "删除指定表中所有记录。本方法不删除表自身，也不改变表结构。"
// 参数<1>: 要清空的表名 SDT_TEXT, 参数说明: NULL
SQLITE3_EXTERN_C void sqlite3_ClearTable_64_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

