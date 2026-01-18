#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).读字段值, 命令说明: "读取指定表或视图中指定字段的值，并写入第四个参数“字段值”。如果“查询条件”参数指定不当或未指定，有可能导致产生多个满足查询条件的记录，此时将取第一条记录的指定字段值。执行成功返回“真”，失败返回“假”。"
// 参数<1>: 表名称 SDT_TEXT, 参数说明: NULL
// 参数<2>: 字段名称或位置 _SDT_ALL, 参数说明: "参数值可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。"
// 参数<3>: [查询条件 SDT_TEXT], 参数说明: NULL
// 参数<4>: &字段值 _SDT_ALL, 参数说明: "其中将存放取得的字段值。如果必要，将自动进行适当的数据类型转换。"
SQLITEDB_EXTERN_C void sqlitedb_GetFieldValue_54_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    PVOID    arg2 = pArgInf[2].m_pByte;
    LPSTR    arg3 = pArgInf[3].m_pText;
    PVOID    arg4 = pArgInf[4].m_pByte;

}

