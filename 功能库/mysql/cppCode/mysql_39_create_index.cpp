#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 建立索引, 命令说明: "建立一个索引。成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 表名 SDT_TEXT, 参数说明: "本参数是将要建立索引的表，如果为中文表名请在表名两边加“`”，如：`姓名`"
// 参数<3>: 列名 SDT_TEXT, 参数说明: "本参数是将要建立索引使用的列（字段）名"
// 参数<4>: 索引名 SDT_TEXT, 参数说明: "本参数是将要建立的索引名"
MYSQL_EXTERN_C void mysql_create_index_39_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;

}

