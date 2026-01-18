#include "..\include_sqlite3_header.h"

// 调用格式: SDT_BOOL (Sqlite表).读字段值, 命令说明: "读取当前记录中指定字段的值，并写入第二个参数“字段值”中。执行成功返回“真”，失败返回“假”。"
// 参数<1>: 字段名称或位置 _SDT_ALL, 参数说明: "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。"
// 参数<2>: &字段值 _SDT_ALL, 参数说明: "其中将存放取得的字段值。如果必要，将自动进行适当的数据类型转换。"
SQLITE3_EXTERN_C void sqlite3_GetFieldValue_135_sqlite3(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;
    PVOID    arg2 = pArgInf[2].m_pByte;

}

