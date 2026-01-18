#include "..\include_mysql_header.h"

// 本命令被隐藏, 原始名字 = "设置MySql目录"
// 调用格式: SDT_BOOL 设置MySql目录, 命令说明: "设置MySql的安装目录。如果返回假，您将不能使用本支持库提供的函数。并且这说明了没有在您提供的路径中找到MYSQL,请您检查安装路径，如果没有安装MYSQL请您到 http://www.mysql.com 下载"
// 参数<1>: MySql的安装目录 SDT_TEXT, 参数说明: "本参数的作用是设置MYSQL的安装目录的。请输入您安装mysql的目录，如: c:\\mysql"
MYSQL_EXTERN_C void mysql_path_0_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[0].m_pText;

}

