#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_BOOL (Sqlite数据库).打开, 命令说明: "打开或新建数据库。要打开或新建的数据库由第一个参数“数据库文件”指定（一个Sqlite数据库就是一个文件）；如果指定的数据库文件不存在，则由第二个参数“是否允许创建”决定是否以该文件名创建新数据库。如果参数“数据库文件”被省略或为空文本，且参数“是否允许创建”为“真”，则在“内存中”创建并打开数据库。执行成功返回“真”，失败返回“假”。"
// 参数<1>: [数据库文件 SDT_TEXT], 参数说明: "欲打开的数据库文件，请使用完整的路径和文件名。"
// 参数<2>: [是否允许创建 SDT_BOOL], 参数说明: "在指定的数据库文件不存在的情况下，如果本属性为“真”，则创建该数据库，否则直接返回“假”。本参数如果被省略，默认为“假”。"
SQLITEDB_EXTERN_C void sqlitedb_Open_30_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPSTR    arg1 = pArgInf[1].m_pText;
    BOOL     arg2 = pArgInf[2].m_bool;

}

