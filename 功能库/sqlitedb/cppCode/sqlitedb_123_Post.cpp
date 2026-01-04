#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite表).提交, 命令说明: "将“编辑()”或“插入()”后对记录所作的修改（通过“写字段值()”）提交到数据库，并退出“编辑”或“插入”状态。执行成功返回真，失败返回假。如果不执行此方法，数据库不会被实际修改。"
// 无参数
SQLITEDB_EXTERN_C void sqlitedb_Post_123_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

