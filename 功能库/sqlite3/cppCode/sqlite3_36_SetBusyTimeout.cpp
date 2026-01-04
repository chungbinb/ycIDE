#include "..\include_sqlite3_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).置最大等待时间, 命令说明: "设置数据库被锁定时的最大等待时间，如果数据库超过最大等待时间后还没有被解锁，则中止尝试执行当前操作。"
// 参数<1>: 最大等待时间(单位: 秒) SDT_INT, 参数说明: NULL
SQLITE3_EXTERN_C void sqlite3_SetBusyTimeout_36_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[1].m_int;

}

