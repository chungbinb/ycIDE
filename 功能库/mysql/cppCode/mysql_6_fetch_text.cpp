#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 读字段值, 命令说明: "读记录集中当前行的字段内容，字段索引从0开始。读取成功返回真，失败返回假。如果读取成功将把字段中的数据写入结果变量"
// 参数<1>: 记录集句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“取记录集”命令得到的返回值"
// 参数<2>: 字段 _SDT_ALL, 参数说明: "本参数指定您想要读取的字段，可以是字段名称或是字段序号"
// 参数<3>: &结果变量 _SDT_ALL, 参数说明: "本参数是存放返回结果的变量，如果字段类型为#MYSQL日期型或#MYSQL时间型或#MYSQL年份型时系统会转换为易语言中的日期时间型，如：字段为#MYSQL时间型，值为“23:33:34”，通过本命令得到的日期时间型变量的值为“2000-1-1 23:33:34”，如果字段为空时返回0或空字符串或空字节集。可以通过“字段是否为空”命令确定"
MYSQL_EXTERN_C void mysql_fetch_text_6_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    PVOID    arg2 = pArgInf[1].m_pByte;
    PVOID    arg3 = pArgInf[2].m_pByte;

}

