#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 增加记录, 命令说明: "写入信息到新记录的指定字段，执行成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 表名 SDT_TEXT, 参数说明: "本参数是您要写入新记录的表的名称，如果为中文表名请在表名两边加“`”，如：`姓名`"
// 参数<3>: 赋值语句 SDT_TEXT, 参数说明: "本参数是您要执行的赋值语句。例如增加一条记录并且为字段name与email分别赋予值“jack”、“jack@126.com”: name='jack',email='jack@126.com'，如果为中文字段名请在字段名两边加“`”，如：`姓名`"
MYSQL_EXTERN_C void mysql_insert_18_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;

}

