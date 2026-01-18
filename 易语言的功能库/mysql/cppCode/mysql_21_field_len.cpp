#include "..\include_mysql_header.h"

// 调用格式: SDT_INT 取字段宽度, 命令说明: "取得指定字段的名称宽度，出错返回-1"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
// 参数<2>: 字段序号 SDT_INT, 参数说明: "本参数是您要取的宽度的字段的序号"
MYSQL_EXTERN_C void mysql_field_len_21_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;

}

