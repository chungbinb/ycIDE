#include "..\include_mysql_header.h"

// 调用格式: SDT_INT 连接MySql, 命令说明: "连接到MySql服务器，返回一个MySql句柄。返回0表示连接失败，请检查该MYSQL服务器是否允许连接。有效的Mysql句柄使用完毕后需通过“断开Mysql”命令释放相关资源。"
// 参数<1>: [MySql服务器地址 SDT_TEXT], 参数说明: "本参数的作用是设置MYSQL服务器的地址。请输入您想要连接的MySql服务器的地址，如果为空或字符串\"localhost\"，则是到本地主机的一个连接"
// 参数<2>: [用户名 SDT_TEXT], 参数说明: "本参数的作用是设置您的用户名，如果为空，为当前用户"
// 参数<3>: [密码 SDT_TEXT], 参数说明: "本参数的作用是设置您的密码，如果为空，且USER表中设置了“空字段可用”属性则可以匿名登陆"
// 参数<4>: [数据库名 SDT_TEXT], 参数说明: "本参数的作用是设置您想要访问的数据库，如果为空则为默认数据库"
// 参数<5>: [端口 SDT_INT], 参数说明: "本参数的作用是设置您想要连接的MySql服务器的端口，如果为空则为默认端口"
MYSQL_EXTERN_C void mysql_mysql_connect_1_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    INT      arg5 = pArgInf[4].m_int;

}

