#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 修改表, 命令说明: "修改一个表，注：只能做“修改类型”中指定的操作。成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 表名 SDT_TEXT, 参数说明: "本参数是将要修改表的表名，如果为中文表名请在表名两边加“`”，如：`姓名`"
// 参数<3>: 更改信息表 MAKELONG(0x02, 0), 参数说明: "本参数记录了新表的相关信息"
// 参数<4>: 字段信息 MAKELONG(0x01, 0), 参数说明: "本参数在修改类型为#增加字段或#修改字段时使用"
// 参数<5>: 修改类型 SDT_INT, 参数说明: "本参数是#增加字段，#修改字段#，#删除字段，#增加索引，#删除索引，#增加主键，#删除主键其中之一"
MYSQL_EXTERN_C void mysql_change_table_30_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    void*    arg3 = pArgInf[2].m_pCompoundData;
    void*    arg4 = pArgInf[3].m_pCompoundData;
    INT      arg5 = pArgInf[4].m_int;

}

