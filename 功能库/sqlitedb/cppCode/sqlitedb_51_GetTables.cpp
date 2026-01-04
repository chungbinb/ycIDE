#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_TEXT (Sqlite数据库).[]取所有表, 命令说明: NULL
// 参数<1>: 是否包含表 SDT_BOOL, 参数说明: NULL
// 参数<2>: 是否包含视图 SDT_BOOL, 参数说明: NULL
// 参数<3>: 是否包含临时表或视图 SDT_BOOL, 参数说明: NULL
SQLITEDB_EXTERN_C void sqlitedb_GetTables_51_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL     arg1 = pArgInf[1].m_bool;
    BOOL     arg2 = pArgInf[2].m_bool;
    BOOL     arg3 = pArgInf[3].m_bool;

}

