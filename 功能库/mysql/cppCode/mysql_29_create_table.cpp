#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 创建表, 命令说明: "创建一个表。成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 表名 SDT_TEXT, 参数说明: "本参数是将要创建的表的名称，如果为中文表名请在表名两边加“`”，如：`姓名表`"
// 参数<3>: 字段信息表 数组 MAKELONG(0x01, 0), 参数说明: "本参数是由字段信息类型组成的数组，这个数组中最多有一个类型为#字段附加类型的元素"
MYSQL_EXTERN_C void mysql_create_table_29_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    void*    arg3 = pArgInf[2].m_pAryData;

}

