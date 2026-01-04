#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite记录集).打开, 命令说明: "打开或重新打开记录集。对于查询类(select)SQL语句，将当前记录指针指向第一条记录之前；对于执行类(update)SQL语句，执行该语句一次。此方法可以被多次重复调用。读取记录集数据之前，必须“打开()”记录集。"
// 无参数
SQLITEDB_EXTERN_C void sqlitedb_Open_190_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

