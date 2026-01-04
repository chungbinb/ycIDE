#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 到指定行, 命令说明: "将当前记录指针移动到指定行，移动成功返回真，失败返回假"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
// 参数<2>: 记录位置 SDT_INT64, 参数说明: "本参数是准备移动到的新记录号，记录号值从 0 开始"
MYSQL_EXTERN_C void mysql_row_seek_11_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT64    arg2 = pArgInf[1].m_int64;

}

