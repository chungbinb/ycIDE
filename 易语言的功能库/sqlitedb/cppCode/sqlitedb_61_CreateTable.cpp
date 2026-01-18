#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).创建表, 命令说明: NULL
// 参数<1>: 表名称 SDT_TEXT, 参数说明: NULL
// 参数<2>: 字段信息 数组/非数组 MAKELONG(0x0A, 0), 参数说明: NULL
SQLITEDB_EXTERN_C void sqlitedb_CreateTable_61_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    void*    arg2 = pArgInf[2].m_pAryData;

}

