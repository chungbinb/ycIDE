#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).执行SQL语句, 命令说明: "执行指定的一条或多条SQL语句。执行成功返回真，失败返回假。本方法不能处理带“SQL参数”的SQL语句。"
// 参数<1>: 欲执行的SQL语句 SDT_TEXT, 参数说明: "多个SQL语句之间请以半角分号“;”隔开，最后一个SQL语句后面可以不加“;”。"
SQLITEDB_EXTERN_C void sqlitedb_ExecuteSQL_32_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

