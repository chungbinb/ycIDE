#include "..\include_sqlite3_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).收缩数据库, 命令说明: "收缩数据所占用的磁盘空间。经过大批量记录增删之后，数据库文件可能会变的较大，调用此方法可释放一部分缓冲区域，以减少文件尺寸。"
// 无参数
SQLITE3_EXTERN_C void sqlite3_ShrinkDB_80_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{

}

