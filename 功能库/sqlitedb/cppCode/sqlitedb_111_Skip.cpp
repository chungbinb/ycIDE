#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite表).跳过, 命令说明: "将当前记录指针向前或者向后移动数条记录。执行成功返回真，失败返回假。"
// 参数<1>: [欲跳过的记录数 SDT_INT], 参数说明: "参数值如果为负数，则向前移动，否则向后移动。如果本参数被省略，默认值为 1 ，即向后移动一条记录。"
SQLITEDB_EXTERN_C void sqlitedb_Skip_111_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

