#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 取错误文本, 命令说明: "有错误发生返回真，并填充错误信息，没错误发生返回假"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: &错误信息 SDT_TEXT, 参数说明: "本参数为错误信息字符串"
MYSQL_EXTERN_C void mysql_error_info_41_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR*   arg2 = pArgInf[1].m_ppText;

}

