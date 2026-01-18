#include "..\include_sqlite3_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).提交事务, 命令说明: NULL
// 参数<1>: [事务名称 SDT_TEXT], 参数说明: NULL
SQLITE3_EXTERN_C void sqlite3_CommitTransaction_45_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

