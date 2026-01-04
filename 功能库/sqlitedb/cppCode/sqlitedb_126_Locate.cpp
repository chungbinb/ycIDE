#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite表).查找, 命令说明: "本命令从当前记录开始（包括当前记录）寻找符合给定条件的记录。如成功找到返回真，并且将当前记录指针移至所找到的记录。如出错或未找到则返回假，当前记录指针位置保持不变。"
// 参数<1>: 查找条件 SDT_TEXT, 参数说明: "查找条件应该类似于“字段名称=字段值”或“字段1名称=字段值1 AND 字段2名称=字段值2”等形式，也可以使用Sqlite所支持的其它SQL语法。此查找条件将作为SQL select语句中where子句的一部分。"
SQLITEDB_EXTERN_C void sqlitedb_Locate_126_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;

}

