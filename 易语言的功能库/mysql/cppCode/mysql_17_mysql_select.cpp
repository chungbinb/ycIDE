#include "..\include_mysql_header.h"

// 调用格式: SDT_INT 查找记录, 命令说明: "查找满足条件的记录，返回记录集句柄。注意执行这条语句后不需要再执行“取记录集”命令，如果为中文字段名请在字段名两边加“`”，如：`姓名`"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 表名 SDT_TEXT, 参数说明: "本参数是您要查询的表的名称，如果为中文表名请在表名两边加“`”，如：`姓名`"
// 参数<3>: 字段名 SDT_TEXT, 参数说明: "本参数是您要查询的表中字段的名称，可以使用逗号连接多个字段，如果为*号则表示查询所有字段。例子1 单个字段 name  例子2 多个字段 name,email,id  例子3 所有字段 *"
// 参数<4>: 查找条件 SDT_TEXT, 参数说明: "本参数是您要查询的条件。例子1 如果您要查找name的值为jack的记录 name='jack' 例子2 如果您要查找name的值为jack并且id的值为2的记录 name='jack' and id=2 例子3 如果您要查找name的值为jack或者id的值为2的记录 name='jack' or id=2"
// 参数<5>: 排序条件 SDT_TEXT, 参数说明: "本参数是排序的条件，ASC为升序，DESC为降序。例如要对name字段进行降序排序: name DESC "
MYSQL_EXTERN_C void mysql_mysql_select_17_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    LPSTR    arg5 = pArgInf[4].m_pText;

}

