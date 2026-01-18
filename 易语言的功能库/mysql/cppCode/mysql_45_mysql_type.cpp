#include "..\include_mysql_header.h"

// 调用格式: SDT_TEXT 到MYSQL文本, 命令说明: "从易语言中的数据类型转换到可以被MYSQL识别的文本。可以将易语言中的时间，逻辑型到MYSQL中相应数据类型，转换成功返回MYSQL可以识别的文本，失败返回空文本"
// 参数<1>: 易语言类型数据 _SDT_ALL, 参数说明: "本参数是易语言中时间日期型或逻辑型的数据"
// 参数<2>: 时间类型 SDT_INT, 参数说明: "本参数是要转换到MYSQL的时间与日期类的类型常量，如#MYSQL日期，#MYSQL时间， #MYSQL日期与时间型，#MYSQL年份型等。注意:当“易语言类型数据”为日期时间型时，本参数有效"
MYSQL_EXTERN_C void mysql_mysql_type_45_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[0].m_pByte;
    INT      arg2 = pArgInf[1].m_int;

}

