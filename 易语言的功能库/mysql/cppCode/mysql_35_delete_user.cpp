#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 删除用户, 命令说明: "删除一个用户。成功返回真，失败返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 用户名 SDT_TEXT, 参数说明: "本参数是将要删除用户的名字"
// 参数<3>: 主机 SDT_TEXT, 参数说明: "本参数是与用户名相关联的主机名"
MYSQL_EXTERN_C void mysql_delete_user_35_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;

}

