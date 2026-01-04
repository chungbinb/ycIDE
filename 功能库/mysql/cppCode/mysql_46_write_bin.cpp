#include "..\include_mysql_header.h"

// 调用格式: SDT_BOOL 写字节集字段, 命令说明: "写字节集字段。本命令只用于写字段类型为字节集的字段，如果条件参数为空(或空字符串)则增加一条新记录，如果条件参数为一个条件字符串则更新符合条件的记录。注意:一次写入数据的大小MYSQL做了限制，如果需要改动请参考相应MYSQL手册，对MYSQL服务器进行调整"
// 参数<1>: MySql句柄 SDT_INT, 参数说明: "本参数必须是本进程调用“连接MySql”命令得到的返回值"
// 参数<2>: 表名 SDT_TEXT, 参数说明: "本参数是您要更新数据的表的名称，如果为中文表名请在表名两边加“`”，如：`姓名`"
// 参数<3>: 字段名 SDT_TEXT, 参数说明: "本参数是待写入字段名称，注意此字段的字段类型必须是字节集，如果为中文字段名请在字段名两边加“`”"
// 参数<4>: [条件 SDT_TEXT], 参数说明: "本参数是查找符合条件的记录。如果为“”空字符串那么将添加一条记录。例如当字段name的内容“jack”时更新记录: name='jack'，如果为中文字段名请在字段名两边加“`”"
// 参数<5>: [字节集 SDT_BIN], 参数说明: "本参数是待写入的字节集：如果本参数被省略且对应字段可以为NULL，则将该字段值置为NULL；如果本参数未被省略且为空字节集，则将该字段值清空。"
MYSQL_EXTERN_C void mysql_write_bin_46_mysql(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    INT      arg1 = pArgInf[0].m_int;
    LPSTR    arg2 = pArgInf[1].m_pText;
    LPSTR    arg3 = pArgInf[2].m_pText;
    LPSTR    arg4 = pArgInf[3].m_pText;
    LPBYTE   arg5 = pArgInf[4].m_pBin;

}

