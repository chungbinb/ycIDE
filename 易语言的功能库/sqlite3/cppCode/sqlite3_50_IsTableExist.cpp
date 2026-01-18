#include "..\include_sqlite3_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).表是否存在, 命令说明: "判断数据库是否已有指定的表或视图，包含临时表和视图。"
// 参数<1>: 表名 SDT_TEXT, 参数说明: NULL
SQLITE3_EXTERN_C void sqlite3_IsTableExist_50_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

