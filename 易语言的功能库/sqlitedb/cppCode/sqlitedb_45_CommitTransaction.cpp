#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).提交事务, 命令说明: NULL
// 参数<1>: [事务名称 SDT_TEXT], 参数说明: NULL
SQLITEDB_EXTERN_C void sqlitedb_CommitTransaction_45_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

