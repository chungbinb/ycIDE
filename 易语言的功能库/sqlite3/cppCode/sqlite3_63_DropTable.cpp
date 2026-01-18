#include "..\include_sqlite3_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).删除表, 命令说明: "将指定表连同其中的所有记录全部删除。"
// 参数<1>: 要删除的表名 SDT_TEXT, 参数说明: NULL
SQLITE3_EXTERN_C void sqlite3_DropTable_63_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

