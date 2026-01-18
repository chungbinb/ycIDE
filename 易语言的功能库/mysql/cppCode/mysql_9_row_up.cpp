#include "..\include_mysql_header.h"

// 本命令被隐藏, 原始名字 = "到上一行"
// 调用格式: SDT_INT64 到上一行, 命令说明: "将当前记录指针向上移动一行，返回移动后的位置"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
MYSQL_EXTERN_C void mysql_row_up_9_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;

}

