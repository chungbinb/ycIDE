#pragma once

#define __SQLITEDB_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 sqlitedb_test_0_sqlitedb
#define SQLITEDB_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__SQLITEDB_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "sqlitedb_test_0_sqlitedb"
#define SQLITEDB_NAME_STR(_index, _name) ______E_FNENAME(__SQLITEDB_NAME(_index, _name))

// 这个宏定义了所有的命令, 以后需要命令名数组, 声明命令等, 都可以使用这个宏
// 下面为_MAKE宏参数列表, 除了第一个成员外, 其他都是 CMD_INFO 这个结构的成员
//  0 _index            = 命令索引, 会根据这个索引生成命令名, 这个索引也能让你更直观的看到这个命令是在命令数组里的下标
//  1 _szName           = 命令的中文名, 需要加双引号, 这个会显示在易语言支持库命令列表里
//  2 _szEgName         = 命令的英文名, 不能加双引号, 这个会显示在易语言支持库命令列表英文名字里, 会根据这个英文名字生成c++实现功能函数名, 这个也是静态库导出的符号名
//  3 _szExplain        = 命令详细解释, 需要加双引号, 这个会显示在易语言支持库命令列表的详细解释里
//  4 _shtCategory      = 全局命令的所属类别, 从1开始, 对象成员命令的此值为-1
//  5 _wState           = 标记, CT_开头常量, _CMD_OS(__OS_WIN) 表示支持win系统, _CMD_OS(OS_ALL) 表示支持所有系统, 如果返回数组或者变长参数, 需要或上对应标志位
//  6 _dtRetValType     = 返回值类型, 使用前注意转换HIWORD为0的内部数据类型值到程序中使用的数据类型值
//  7 _wReserved        = 保留字段, 填0
//  8 _shtUserLevel     = 命令的用户学习难度级别, LVL_SIMPLE=初级命令, LVL_SECONDARY=中级命令, LVL_HIGH=高级命令
//  9 _shtBitmapIndex   = 指定图像索引, 从1开始, 0表示无, 显示在支持库列表里的图标
// 10 _shtBitmapCount   = 图像数目(用作动画)
// 11 _nArgCount        = 命令的参数数目
// 12 _pBeginArgInfo    = 参数起始地址
#define SQLITEDB_DEF(_MAKE) \
    _MAKE(  0, "全局命令一", QuanJuMingLingYi, NULL, 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(  1, "无法识别的名字_1", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(  2, "无法识别的名字_2", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(  3, "无法识别的名字_3", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(  4, "无法识别的名字_4", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(  5, "无法识别的名字_5", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(  6, "无法识别的名字_6", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(  7, "无法识别的名字_7", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(  8, "无法识别的名字_8", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(  9, "无法识别的名字_9", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 10, "无法识别的名字_10", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 11, "无法识别的名字_11", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 12, "无法识别的名字_12", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 13, "无法识别的名字_13", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 14, "无法识别的名字_14", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 15, "无法识别的名字_15", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 16, "无法识别的名字_16", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 17, "无法识别的名字_17", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 18, "无法识别的名字_18", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 19, "无法识别的名字_19", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 20, "构造函数", Constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 21, "复制构造函数", CopyConstructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 22, "析构函数", Destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 23, "无法识别的名字_23", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 24, "无法识别的名字_24", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 25, "无法识别的名字_25", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 26, "无法识别的名字_26", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 27, "无法识别的名字_27", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 28, "无法识别的名字_28", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 29, "是否已打开", IsOpen, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 30, "打开", Open, "打开或新建数据库。要打开或新建的数据库由第一个参数“数据库文件”指定（一个Sqlite数据库就是一个文件）；如果指定的数据库文件不存在，则由第二个参数“是否允许创建”决定是否以该文件名创建新数据库。如果参数“数据库文件”被省略或为空文本，且参数“是否允许创建”为“真”，则在“内存中”创建并打开数据库。执行成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 1)\
    _MAKE( 31, "关闭", Close, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 32, "执行SQL语句", ExecuteSQL, "执行指定的一条或多条SQL语句。执行成功返回真，失败返回假。本方法不能处理带“SQL参数”的SQL语句。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 3)\
    _MAKE( 33, "取记录集", GetDataset, "返回“Sqlite记录集”对象。本方法等效于“Sqlite记录集.置SQL语句()”。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x03, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 4)\
    _MAKE( 34, "无法识别的名字_34", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 35, "无法识别的名字_35", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 36, "置最大等待时间", SetBusyTimeout, "设置数据库被锁定时的最大等待时间，如果数据库超过最大等待时间后还没有被解锁，则中止尝试执行当前操作。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 5)\
    _MAKE( 37, "取错误码", GetLastErrorCode, "取前面最近一次操作的执行结果的错误代码。如果错误代码为0表示前一次操作成功。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 38, "取错误文本", GetLastErrorText, "取前面最近一次操作的执行结果的错误信息文本。如果前一次操作成功或本方法调用失败，将返回空文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 39, "取最新插入ID", GetLastInsertRowId, "取前面最近一次插入的记录的记录ID。记录ID从1开始。如果执行失败将返回0。", -1, _CMD_OS(__OS_WIN), SDT_INT64, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 40, "创建SQL函数todo", CreateSqlFunction, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 6)\
    _MAKE( 41, "无法识别的名字_41", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 42, "无法识别的名字_42", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 43, "无法识别的名字_43", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 44, "开始事务", BeginTransaction, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 7)\
    _MAKE( 45, "提交事务", CommitTransaction, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 7)\
    _MAKE( 46, "回滚事务", RollbackTransaction, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 7)\
    _MAKE( 47, "结束事务", EndTransaction, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 7)\
    _MAKE( 48, "无法识别的名字_48", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 49, "无法识别的名字_49", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 50, "表是否存在", IsTableExist, "判断数据库是否已有指定的表或视图，包含临时表和视图。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 8)\
    _MAKE( 51, "取所有表", GetTables, NULL, -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_sqlitedb_global_var + 9)\
    _MAKE( 52, "取表内容", GetTableContent, "返回一个二维文本数组，其中存放了指定表的所有记录（含所有字段）内容；表中没有记录或操作失败，将返回空数组。如果表中有非文本型字段，将自动转换为文本型（可能丢失部分数据）。提示：使用“Sqlite表.读字段值()”或“Sqlite记录集.读字段值()”可正确地处理非文本类型字段。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 12)\
    _MAKE( 53, "取表定义", GetTableSql, "返回创建指定表时所用的SQL语句（CREATE TABLE ...）。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 12)\
    _MAKE( 54, "读字段值", GetFieldValue, "读取指定表或视图中指定字段的值，并写入第四个参数“字段值”。如果“查询条件”参数指定不当或未指定，有可能导致产生多个满足查询条件的记录，此时将取第一条记录的指定字段值。执行成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_sqlitedb_global_var + 14)\
    _MAKE( 55, "无法识别的名字_55", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 56, "无法识别的名字_56", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 57, "无法识别的名字_57", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 58, "无法识别的名字_58", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 59, "无法识别的名字_59", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 60, "无法识别的名字_60", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 61, "创建表", CreateTable, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 18)\
    _MAKE( 62, "修改表(暂时不提供本函数)", XiuGaiBiao_ZanShiBuTiGongBenHanShu_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 6)\
    _MAKE( 63, "删除表", DropTable, "将指定表连同其中的所有记录全部删除。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 20)\
    _MAKE( 64, "清空表", ClearTable, "删除指定表中所有记录。本方法不删除表自身，也不改变表结构。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 21)\
    _MAKE( 65, "无法识别的名字_65", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 66, "无法识别的名字_66", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 67, "无法识别的名字_67", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 68, "无法识别的名字_68", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 69, "无法识别的名字_69", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 70, "无法识别的名字_70", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 71, "无法识别的名字_71", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 72, "无法识别的名字_72", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 73, "无法识别的名字_73", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 74, "无法识别的名字_74", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 75, "无法识别的名字_75", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 76, "无法识别的名字_76", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 77, "无法识别的名字_77", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 78, "无法识别的名字_78", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 79, "无法识别的名字_79", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 80, "收缩数据库", ShrinkDB, "收缩数据所占用的磁盘空间。经过大批量记录增删之后，数据库文件可能会变的较大，调用此方法可释放一部分缓冲区域，以减少文件尺寸。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 81, "无法识别的名字_81", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 82, "无法识别的名字_82", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 83, "无法识别的名字_83", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 84, "无法识别的名字_84", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 85, "无法识别的名字_85", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 86, "无法识别的名字_86", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 87, "无法识别的名字_87", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 88, "无法识别的名字_88", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 89, "无法识别的名字_89", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 90, "无法识别的名字_90", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 91, "无法识别的名字_91", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 92, "无法识别的名字_92", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 93, "无法识别的名字_93", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 94, "无法识别的名字_94", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 95, "无法识别的名字_95", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 96, "无法识别的名字_96", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 97, "无法识别的名字_97", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 98, "无法识别的名字_98", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE( 99, "无法识别的名字_99", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(100, "构造函数", Constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(101, "复制构造函数", CopyConstructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(102, "析构函数", Destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(103, "无法识别的名字_103", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(104, "是否已打开", IsOpen, "判断表是否处理打开状态", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(105, "打开", Open, "打开指定表。成功返回真，否则返回假。表成功打开后，记录集指针将指向首记录前。一个“Sqlite表”对象同一时刻只能打开一个表，打开后一个表意味着将关闭前一个已打开的表(如果有的话)。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 22)\
    _MAKE(106, "关闭", Close, "关闭当前打开的表。如果用户未调用本方法关闭已打开的表，支持库会在适当的时机自动关闭该表。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(107, "到首记录", First, "移动到表的第一条记录，执行成功返回真，否则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(108, "到尾记录", Last, "移动到表的最后一条记录，执行成功返回真，否则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(109, "到下一记录", Next, "移动到下一条记录，执行成功返回真，否则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(110, "到上一记录", Prior, "移动到上一条记录，执行成功返回真，否则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(111, "跳过", Skip, "将当前记录指针向前或者向后移动数条记录。执行成功返回真，失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 24)\
    _MAKE(112, "跳到", Goto, "改变当前记录指针到指定的记录号。成功返回真，失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 25)\
    _MAKE(113, "首记录前", Bof, "如果当前记录指针已在首记录的前面，返回真，否则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(114, "尾记录后", eof, "如果当前记录指针已在尾记录的后面，返回真，否则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(115, "无法识别的名字_115", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(116, "无法识别的名字_116", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(117, "无法识别的名字_117", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(118, "无法识别的名字_118", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(119, "无法识别的名字_119", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(120, "编辑", Edit, "进入“编辑”状态，准备修改当前记录。在调用本方法后，可使用“写字段值()”修改当前记录中的各字段值；只有调用“提交()”后才会实际执行修改操作。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(121, "插入", Insert, "进入“插入”状态，准备向表中插入一条记录。在调用本方法后，可使用“写字段值()”填充要插入记录中的各字段值；只有调用“提交()”后才会实际执行插入操作。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(122, "删除", Delete, "删除当前记录。删除成功后，记录指针指向着记录前。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(123, "提交", Post, "将“编辑()”或“插入()”后对记录所作的修改（通过“写字段值()”）提交到数据库，并退出“编辑”或“插入”状态。执行成功返回真，失败返回假。如果不执行此方法，数据库不会被实际修改。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(124, "取消", Cancel, "取消“编辑()”或“插入()”后对记录所作的修改（通过“写字段值()”），并退出“编辑”或“插入”状态。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(125, "刷新", Refresh, "重新从数据库中读取最新数据。一般在其它程序或线程修改了数据库后需调用本方法。注意本方法不改变当前记录号。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(126, "查找", Locate, "本命令从当前记录开始（包括当前记录）寻找符合给定条件的记录。如成功找到返回真，并且将当前记录指针移至所找到的记录。如出错或未找到则返回假，当前记录指针位置保持不变。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 26)\
    _MAKE(127, "无法识别的名字_127", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(128, "无法识别的名字_128", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(129, "无法识别的名字_129", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(130, "无法识别的名字_130", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(131, "无法识别的名字_131", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(132, "无法识别的名字_132", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(133, "无法识别的名字_133", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(134, "无法识别的名字_134", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(135, "读字段值", GetFieldValue, "读取当前记录中指定字段的值，并写入第二个参数“字段值”中。执行成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 27)\
    _MAKE(136, "写字段值", SetFieldValue, "将第二个参数“字段值”数据写入当前记录中的指定字段。执行成功返回“真”，失败返回“假”。本方法通常在“编辑()”或“插入()”之后、“提交()”之前被调用，且仅当“提交()”调用后数据库才被实际修改。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 29)\
    _MAKE(137, "读字段文本值", GetFieldTextValue, "读取当前记录中指定字段的值，并转换为文本型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 31)\
    _MAKE(138, "读字段整数值", GetFieldIntValue, "读取当前记录中指定字段的值，并转换为整数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 33)\
    _MAKE(139, "读字段小数值", GetFieldFloatValue, "读取当前记录中指定字段的值，并转换为小数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 35)\
    _MAKE(140, "读字段双精度小数值", GetFieldDoubleValue, "读取当前记录中指定字段的值，并转换为双精度小数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 37)\
    _MAKE(141, "读字段字节集值", GetFieldBinValue, "读取当前记录中指定字段的值，并转换为字节集型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 39)\
    _MAKE(142, "读字段逻辑值", GetFieldBoolValue, "读取当前记录中指定字段的值，并转换为逻辑型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 41)\
    _MAKE(143, "读字段日期时间值", GetFieldDatetimeValue, "读取当前记录中指定字段的值，并转换为日期时间型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 43)\
    _MAKE(144, "读字段长整数值", GetFieldInt64Value, "读取当前记录中指定字段的值，并转换为长整数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_INT64, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 45)\
    _MAKE(145, "读字段短整数值", GetFieldShortValue, "读取当前记录中指定字段的值，并转换为短整数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_SHORT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 47)\
    _MAKE(146, "读字段字节值", GetFieldByteValue, "读取当前记录中指定字段的值，并转换为字节型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_BYTE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 49)\
    _MAKE(147, "无法识别的名字_147", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(148, "取表名", GetTableName, "返回当前已打开的表名。如果表已被关闭，将返回空文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(149, "取记录号", GetRecordNO, "返回当前记录的记录号，第一条记录为1，第二条记录为2，依次类推。如果当前记录指针在首记录前，返回0 ；如果在尾记录后，返回值为记录总数加一。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(150, "取记录个数", GetRecordCount, "返回表中的记录个数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(151, "取所有记录", GetContent, "返回一个二维文本数组，其中存放了表中的所有记录（含所有字段）内容；表中没有记录或操作失败，将返回空数组。如果表中有非文本型字段，将自动转换为文本型（可能丢失部分数据）。本方法与“Sqlite数据库.取表内容()”等效。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 51)\
    _MAKE(152, "取字段个数", GetFieldCount, "取表中的字段个数", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(153, "取所有字段", GetFields, "返回一个一维文本型数组，其中包含了当前表中的所有字段名称。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(154, "无法识别的名字_154", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(155, "无法识别的名字_155", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(156, "无法识别的名字_156", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(157, "无法识别的名字_157", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(158, "无法识别的名字_158", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(159, "无法识别的名字_159", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(160, "无法识别的名字_160", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(161, "无法识别的名字_161", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(162, "无法识别的名字_162", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(163, "无法识别的名字_163", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(164, "无法识别的名字_164", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(165, "无法识别的名字_165", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(166, "无法识别的名字_166", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(167, "无法识别的名字_167", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(168, "无法识别的名字_168", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(169, "无法识别的名字_169", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(170, "无法识别的名字_170", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(171, "无法识别的名字_171", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(172, "无法识别的名字_172", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(173, "无法识别的名字_173", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(174, "无法识别的名字_174", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(175, "无法识别的名字_175", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(176, "无法识别的名字_176", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(177, "无法识别的名字_177", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(178, "无法识别的名字_178", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(179, "无法识别的名字_179", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(180, "构造函数", Constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(181, "复制构造函数", CopyConstructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(182, "析构函数", Destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(183, "无法识别的名字_183", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(184, "置SQL语句", SetSQL, "设置本对象所使用的SQL语句。本方法等效于“Sqlite数据库.取记录集()”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 52)\
    _MAKE(185, "取参数个数", GetParameterCount, "返回当前SQL语句（由“置SQL语句()”或“Sqlite数据库.取记录集()”指定）中所包含的“SQL参数”个数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(186, "取所有参数", GetParameters, "返回所有由“SQL参数”的参数名称组成的一维文本数组。如果某“SQL参数”没有名称，则对应的数组成员为空文本。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(187, "绑定参数", BindParameter, "对当前SQL语句（由“置SQL语句()”或“Sqlite数据库.取记录集()”所指定）中所包含的某个“SQL参数”进行值绑定。所有“SQL参数”均需进行值绑定，未经绑定的“SQL参数”其值将视为null。如果对同一“SQL参数”进行多次绑定，则最后一次绑定的值将覆盖前面所绑定的值。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 54)\
    _MAKE(188, "无法识别的名字_188", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(189, "是否已打开", IsOpen, "判断记录集是否已经打开。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(190, "打开", Open, "打开或重新打开记录集。对于查询类(select)SQL语句，将当前记录指针指向第一条记录之前；对于执行类(update)SQL语句，执行该语句一次。此方法可以被多次重复调用。读取记录集数据之前，必须“打开()”记录集。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(191, "关闭", Close, "关闭记录集。关闭后的记录集不再可用，所有内部数据全部丢失。如果用户没有调用本方法关闭记录集，支持库会在适当的时机自动关闭记录集。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(192, "读字段值", GetFieldValue, "读取当前记录中指定字段的值，并写入第二个参数“字段值”中。执行成功返回“真”，失败返回“假”。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 56)\
    _MAKE(193, "无法识别的名字_193", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(194, "无法识别的名字_194", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(195, "无法识别的名字_195", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(196, "无法识别的名字_196", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(197, "无法识别的名字_197", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(198, "到下一记录", Next, "移动到下一条记录", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(199, "无法识别的名字_199", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(200, "首记录前", Bof, "判断当前记录指针是否指向第一条记录之前。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(201, "尾记录后", Eof, "判断当前记录指针是否指向最后一条记录之后。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(202, "无法识别的名字_202", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(203, "无法识别的名字_203", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(204, "无法识别的名字_204", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(205, "无法识别的名字_205", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(206, "无法识别的名字_206", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(207, "无法识别的名字_207", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(208, "取记录个数", GetRecordCount, NULL, -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(209, "取所有记录", GetContent, "返回一个二维文本数组，其中存放了记录集中所有记录（含所有字段）内容；如果记录集中没有记录或操作失败，将返回空数组。如果记录集中有非文本型字段，将自动转换为文本型（可能丢失部分数据）。提示：使用“Sqlite表.读字段值()”或“Sqlite记录集.读字段值()”可正确地处理非文本类型字段。注意：本方法执行后，当前记录指针将指向首记录前（相当于重新打开记录集）。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sqlitedb_global_var + 58)\
    _MAKE(210, "取字段个数", GetFieldCount, NULL, -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(211, "取所有字段", GetFields, "返回所有字段名称组成的一维文本数组", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(212, "无法识别的名字_212", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(213, "无法识别的名字_213", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(214, "无法识别的名字_214", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(215, "读字段文本值", GetFieldTextValue, "读取当前记录中指定字段的值，并转换为文本型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 59)\
    _MAKE(216, "读字段整数值", GetFieldIntValue, "读取当前记录中指定字段的值，并转换为整数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 61)\
    _MAKE(217, "读字段小数值", GetFieldFloatValue, "读取当前记录中指定字段的值，并转换为小数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_FLOAT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 63)\
    _MAKE(218, "读字段双精度小数值", GetFieldDoubleValue, "读取当前记录中指定字段的值，并转换为双精度小数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_DOUBLE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 65)\
    _MAKE(219, "读字段字节集值", GetFieldBinValue, "读取当前记录中指定字段的值，并转换为字节集型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 67)\
    _MAKE(220, "读字段逻辑值", GetFieldBoolValue, "读取当前记录中指定字段的值，并转换为逻辑型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 69)\
    _MAKE(221, "读字段日期时间值", GetFieldDatetimeValue, "读取当前记录中指定字段的值，并转换为日期时间型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 71)\
    _MAKE(222, "读字段长整数值", GetFieldInt64Value, "读取当前记录中指定字段的值，并转换为长整数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_INT64, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 73)\
    _MAKE(223, "读字段短整数值", GetFieldShortValue, "读取当前记录中指定字段的值，并转换为短整数型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_SHORT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 75)\
    _MAKE(224, "读字段字节值", GetFieldByteValue, "读取当前记录中指定字段的值，并转换为字节型数据后返回。", -1, _CMD_OS(__OS_WIN), SDT_BYTE, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sqlitedb_global_var + 77)\
    _MAKE(225, "无法识别的名字_225", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(226, "无法识别的名字_226", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(227, "无法识别的名字_227", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(228, "无法识别的名字_228", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)\
    _MAKE(229, "无法识别的名字_229", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sqlitedb_global_var + 0)

