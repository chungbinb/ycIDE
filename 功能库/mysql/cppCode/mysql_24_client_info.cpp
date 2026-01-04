#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 取客户端版本, 命令说明: "取得MYSQL客户端的版本信息并设置结果变量，成功返回真，失败返回假"
// 参数<1>: &结果变量 SDT_TEXT, 参数说明: "本参数存放调用本命令后的结果"
MYSQL_EXTERN_C void mysql_client_info_24_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR*   arg1 = pArgInf[0].m_ppText;

}

