#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 创建用户, 命令说明: "创建一个用户。成功返回真，失败返回假。具体规则请参见MYSQL相关手册"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 主机 SDT_TEXT, 参数说明: "本参数是与新用户相关联的主机名，如果为空文本则为所有主机"
// 参数<3>: 用户名 SDT_TEXT, 参数说明: "本参数是新用户名"
// 参数<4>: 密码 SDT_TEXT, 参数说明: "本参数是用户的密码"
// 参数<5>: 库名 SDT_TEXT, 参数说明: "本参数是新用户所操作的库，如果为空文本或“*”，则为所有库，如果为中文表名请在表名两边加“`”"
// 参数<6>: 表名 SDT_TEXT, 参数说明: "本参数是新用户所操作的表，当库名不为空文本或“*”时本参数可以为空文本或“*”即所有表，如果为中文表名请在表名两边加“`”，如：`姓名`"
// 参数<7>: 权限 SDT_INT, 参数说明: "本参数是新用户对库或表的操作权限。如果为零则为无权限"
MYSQL_EXTERN_C void mysql_create_user_34_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    LPSTR    arg5 = pArgInf[4].m_pText;
    LPSTR    arg6 = pArgInf[5].m_pText;
    INT      arg7 = pArgInf[6].m_int;

}

