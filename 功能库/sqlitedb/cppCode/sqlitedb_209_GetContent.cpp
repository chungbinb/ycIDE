#include "..\include_sqlitedb_header.h"

// 调用格式: SDT_TEXT (Sqlite记录集).[]取所有记录, 命令说明: "返回一个二维文本数组，其中存放了记录集中所有记录（含所有字段）内容；如果记录集中没有记录或操作失败，将返回空数组。如果记录集中有非文本型字段，将自动转换为文本型（可能丢失部分数据）。提示：使用“Sqlite表.读字段值()”或“Sqlite记录集.读字段值()”可正确地处理非文本类型字段。注意：本方法执行后，当前记录指针将指向首记录前（相当于重新打开记录集）。"
// 参数<1>: 是否包含表头 SDT_BOOL, 参数说明: "是否在返回值数组的第一行包含字段名称。如果本记录集中没有任何记录，即使本参数为“真”，也将返回空数组。"
SQLITEDB_EXTERN_C void sqlitedb_GetContent_209_sqlitedb(PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    BOOL     arg1 = pArgInf[1].m_bool;

}

