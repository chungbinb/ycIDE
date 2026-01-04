#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_FLOAT (Sqlite记录集).读字段小数值, 命令说明: "读取当前记录中指定字段的值，并转换为小数型数据后返回。"
// 参数<1>: 字段名称或位置 _SDT_ALL, 参数说明: "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。"
// 参数<2>: [&执行结果 SDT_BOOL], 参数说明: "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。"
SQLITEDB_EXTERN_C void sqlitedb_GetFieldFloatValue_217_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;
    BOOL*    arg2 = pArgInf[2].m_pBool;

}

