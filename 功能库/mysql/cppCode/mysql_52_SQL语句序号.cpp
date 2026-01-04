#include "..\include_mysql_header.h"

// 本命令被隐藏, 原始名字 = "输出SQL"
// 调用格式: SDT_BOOL 输出SQL, 命令说明: "SQL语句的序号"
// 参数<1>: SQL语句序号 SDT_INT, 参数说明: "SQL语句的序号"
// 参数<2>: &返回SQL语句 SDT_TEXT, 参数说明: "返回SQL语句"
MYSQL_EXTERN_C void mysql_SQL语句序号_52_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR*   arg2 = pArgInf[1].m_ppText;

}

