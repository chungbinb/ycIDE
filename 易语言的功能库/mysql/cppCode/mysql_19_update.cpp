#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 更新记录, 命令说明: "更新指定字段的数据，更新成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 表名 SDT_TEXT, 参数说明: "本参数是您要更新数据的表的名称，如果为中文表名请在表名两边加“`”，如：`姓名`"
// 参数<3>: 赋值语句 SDT_TEXT, 参数说明: "本参数是您要执行的赋值语句。例如为字段name与email分别赋予值“sun”、“sun@126.com”: name='sun',email='sun@126.com'，如果为中文字段名请在字段名两边加“`”，如：`姓名`"
// 参数<4>: 条件 SDT_TEXT, 参数说明: "本参数是查找符合条件的记录。如果为“”空字符串那么将更新所有记录。例如当字段name的内容“jack”时更新记录: name='jack'"
MYSQL_EXTERN_C void mysql_update_19_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;

}

