#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 到下一行, 命令说明: "将当前记录指针向下移动一行，成功返回真，失败返回假"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
MYSQL_EXTERN_C void mysql_row_next_10_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

