#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite记录集).置SQL语句, 命令说明: "设置本对象所使用的SQL语句。本方法等效于“Sqlite数据库.取记录集()”。"
// 参数<1>: SQL语句 SDT_TEXT, 参数说明: "SQL语句中可包含零到多个“SQL参数”，形如“?”“:n”“$v”等，其中n代表一个数值，v代表一个文本。所有“SQL参数”均需经过“绑定参数()”进行值绑定，未经绑定的“SQL参数”其值将视为null。"
// 参数<2>: Sqlite数据库 MAKELONG(0x01, 0), 参数说明: "指定对哪个Sqlite数据库进行查询，该数据库必须已经打开"
SQLITEDB_EXTERN_C void sqlitedb_SetSQL_184_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    void*    arg2 = pArgInf[2].m_pCompoundData;

}

