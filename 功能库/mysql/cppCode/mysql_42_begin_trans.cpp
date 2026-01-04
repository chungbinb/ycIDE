#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 开始事务, 命令说明: "开始一件事务。成功返回真，失败返回假。注意 事务不可以用于MyISAM数据表类型，但是返回值为真。关于数据表类型设置请参考相关MYSQL资料"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
MYSQL_EXTERN_C void mysql_begin_trans_42_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

