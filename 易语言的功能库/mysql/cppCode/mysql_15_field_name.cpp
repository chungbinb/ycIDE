#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 序号到字段名, 命令说明: "通过序号得到字段名，如果成功返回真，结果变量为得到的字段名，失败返回假，结果变量无效"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
// 参数<2>: 序号 SDT_INT, 参数说明: "本参数是字段的序号"
// 参数<3>: &结果变量 SDT_TEXT, 参数说明: "本参数是存放返回的字段名的变量"
MYSQL_EXTERN_C void mysql_field_name_15_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    INT      arg2 = pArgInf[1].m_int;
    LPSTR*   arg3 = pArgInf[2].m_ppText;

}

