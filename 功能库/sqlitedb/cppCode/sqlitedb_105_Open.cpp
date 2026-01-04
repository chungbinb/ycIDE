#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite表).打开, 命令说明: "打开指定表。成功返回真，否则返回假。表成功打开后，记录集指针将指向首记录前。一个“Sqlite表”对象同一时刻只能打开一个表，打开后一个表意味着将关闭前一个已打开的表(如果有的话)。"
// 参数<1>: 表名称 SDT_TEXT, 参数说明: NULL
// 参数<2>: Sqlite数据库 MAKELONG(0x01, 0), 参数说明: "指定要打开哪个Sqlite数据库中的表。该数据库必须已经打开。"
SQLITEDB_EXTERN_C void sqlitedb_Open_105_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

