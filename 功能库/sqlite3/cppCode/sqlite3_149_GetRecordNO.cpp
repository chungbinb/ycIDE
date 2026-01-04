#include "..\include_sqlite3_header.h"

// 调用格式: SDT_INT (Sqlite表).取记录号, 命令说明: "返回当前记录的记录号，第一条记录为1，第二条记录为2，依次类推。如果当前记录指针在首记录前，返回0 ；如果在尾记录后，返回值为记录总数加一。"
// 无参数
SQLITE3_EXTERN_C void sqlite3_GetRecordNO_149_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

