#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 字段名到序号, 命令说明: "通过字段名得到序号，如果成功返回真，结果变量为得到的序号，失败返回假，结果变量无效"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
// 参数<2>: 字段名 SDT_TEXT, 参数说明: "本参数是字段的名称"
// 参数<3>: &结果变量 SDT_INT, 参数说明: "本参数是存放返回的序号的变量"
MYSQL_EXTERN_C void mysql_field_index_16_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    PINT     arg3 = pArgInf[2].m_pInt;

}

