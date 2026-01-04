#include "..\include_sqlite3_header.h"

// 调用格式: MAKELONG(0x03, 0) (Sqlite数据库).取记录集, 命令说明: "返回“Sqlite记录集”对象。本方法等效于“Sqlite记录集.置SQL语句()”。"
// 参数<1>: SQL语句 SDT_TEXT, 参数说明: "SQL语句中可包含零到多个“SQL参数”，形如“?”“:n”“$v”等，其中n代表一个数值，v代表一个文本。所有“SQL参数”均需经过“Sqlite记录集.绑定参数()”进行值绑定，未经绑定的“SQL参数”其值将视为null。"
SQLITE3_EXTERN_C void sqlite3_GetDataset_33_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

