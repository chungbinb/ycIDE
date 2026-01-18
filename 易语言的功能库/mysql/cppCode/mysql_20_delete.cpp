#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 删除记录, 命令说明: "删除一条记录，删除成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 表名 SDT_TEXT, 参数说明: "本参数是您要进行删除操作的表的名称，如果为中文表名请在表名两边加“`”，如：`姓名`"
// 参数<3>: 条件 SDT_TEXT, 参数说明: "本参数是删除符合条件的记录。如果为“”(空字符串)将删除所有记录。例如当字段name的内容为“jack”时删除记录: name='jack',如果为中文字段名请在字段名两边加“`”,如：`姓名`"
MYSQL_EXTERN_C void mysql_delete_20_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;

}

