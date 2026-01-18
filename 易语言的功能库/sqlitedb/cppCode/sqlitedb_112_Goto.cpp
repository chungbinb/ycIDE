#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite表).跳到, 命令说明: "改变当前记录指针到指定的记录号。成功返回真，失败返回假。"
// 参数<1>: 欲跳到的记录号 SDT_INT, 参数说明: "参数值所指定的记录号从 1 开始，即首记录的记录号为 1 ，依此类推。"
SQLITEDB_EXTERN_C void sqlitedb_Goto_112_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

