#include "..\include_mysql_header.h"

// 调用格式: SDT_INT 取字段属性, 命令说明: "取得字段的属性，返回与字段类型常量向对应的值，出错返回-1"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
// 参数<2>: 字段名或字段序号 _SDT_ALL, 参数说明: "本参数是您要取得属性的字段的名称或序号"
MYSQL_EXTERN_C void mysql_field_type_22_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    PVOID    arg2 = pArgInf[1].m_pByte;

}

