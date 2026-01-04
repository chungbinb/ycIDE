#include "..\include_mysql_header.h"

// 调用格式: _SDT_NULL 断开MySql, 命令说明: "断开一个MySql连接。断开之后原连接句柄不可再用。"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值。对于参数值为0的情况不执行任何操作（注意，本库3.0以前的版本在参数为0时执行断开最后一次连接，易语言5.0针对静态编译对此库进行改造后已不支持该功能）。"
MYSQL_EXTERN_C void mysql_mysql_close_2_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

