#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 字段是否为空, 命令说明: "判断记录集中当前行的某个字段是否为空，不为空返回真，为空返回假"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
// 参数<2>: 字段 _SDT_ALL, 参数说明: "本参数指定您想要读取的字段，可以是字段名称或是字段序号"
MYSQL_EXTERN_C void mysql_field_is_null_47_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    PVOID    arg2 = pArgInf[1].m_pByte;

}

