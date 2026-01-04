#include "..\include_sqlite3_header.h"

// 调用格式: SDT_BOOL (Sqlite记录集).绑定参数, 命令说明: "对当前SQL语句（由“置SQL语句()”或“Sqlite数据库.取记录集()”所指定）中所包含的某个“SQL参数”进行值绑定。所有“SQL参数”均需进行值绑定，未经绑定的“SQL参数”其值将视为null。如果对同一“SQL参数”进行多次绑定，则最后一次绑定的值将覆盖前面所绑定的值。"
// 参数<1>: 参数名称或索引 _SDT_ALL, 参数说明: "使用参数名称时注意，“:”“$”也是参数名称的一部分；使用参数索引时注意，索引从1开始。"
// 参数<2>: [要绑定的值 _SDT_ALL], 参数说明: "如果省略本参数，默认为null。"
SQLITE3_EXTERN_C void sqlite3_BindParameter_187_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;
    PVOID    arg2 = pArgInf[2].m_pByte;

}

