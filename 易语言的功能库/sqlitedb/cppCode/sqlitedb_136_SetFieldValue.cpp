#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite表).写字段值, 命令说明: "将第二个参数“字段值”数据写入当前记录中的指定字段。执行成功返回“真”，失败返回“假”。本方法通常在“编辑()”或“插入()”之后、“提交()”之前被调用，且仅当“提交()”调用后数据库才被实际修改。"
// 参数<1>: 字段名称或位置 _SDT_ALL, 参数说明: "参数值可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。"
// 参数<2>: 字段值 _SDT_ALL, 参数说明: "指定欲写入的值"
SQLITEDB_EXTERN_C void sqlitedb_SetFieldValue_136_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    PVOID    arg1 = pArgInf[1].m_pByte;
    PVOID    arg2 = pArgInf[2].m_pByte;

}

