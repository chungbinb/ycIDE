#pragma once

#define __MYSQL_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 mysql_test_0_mysql
#define MYSQL_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__MYSQL_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "mysql_test_0_mysql"
#define MYSQL_NAME_STR(_index, _name) ______E_FNENAME(__MYSQL_NAME(_index, _name))

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
#define MYSQL_DEF(_MAKE) \
    _MAKE(  0, "设置MySql目录", path, "设置MySql的安装目录。如果返回假，您将不能使用本支持库提供的函数。并且这说明了没有在您提供的路径中找到MYSQL,请您检查安装路径，如果没有安装MYSQL请您到 http://www.mysql.com 下载", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 0)\
    _MAKE(  1, "连接MySql", mysql_connect, "连接到MySql服务器，返回一个MySql句柄。返回0表示连接失败，请检查该MYSQL服务器是否允许连接。有效的Mysql句柄使用完毕后需通过“断开Mysql”命令释放相关资源。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_mysql_global_var + 1)\
    _MAKE(  2, "断开MySql", mysql_close, "断开一个MySql连接。断开之后原连接句柄不可再用。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 6)\
    _MAKE(  3, "执行SQL语句", mysql_query, "执行一条SQL语句，执行成功返回真，失败返回假。如果执行成功可以通过“取记录集”命令得到记录集句柄。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_mysql_global_var + 7)\
    _MAKE(  4, "取记录集", store_result, "得到查询后的记录的集合(简称：记录集)。返回记录集句柄。当使用完记录集后要通过“释放记录集”命令来释放记录集", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 7)\
    _MAKE(  5, "释放记录集", free_result, "释放记录集之后，原记录集句柄不可再用。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 9)\
    _MAKE(  6, "读字段值", fetch_text, "读记录集中当前行的字段内容，字段索引从0开始。读取成功返回真，失败返回假。如果读取成功将把字段中的数据写入结果变量", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_mysql_global_var + 10)\
    _MAKE(  7, "取记录集行数", num_rows, "获取记录集的行数", 1, _CMD_OS(__OS_WIN), SDT_INT64, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 9)\
    _MAKE(  8, "当前行号", row_tell, "获得记录集指针指向的行的位置", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_INT64, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 9)\
    _MAKE(  9, "到上一行", row_up, "将当前记录指针向上移动一行，返回移动后的位置", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_INT64, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 9)\
    _MAKE( 10, "到下一行", row_next, "将当前记录指针向下移动一行，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 9)\
    _MAKE( 11, "到指定行", row_seek, "将当前记录指针移动到指定行，移动成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 13)\
    _MAKE( 12, "到首行", row_head, "将当前记录指针移动到记录集首记录，移动成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 9)\
    _MAKE( 13, "到尾行", row_cauda, "将当前记录指针移动到记录集尾记录，移动成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 9)\
    _MAKE( 14, "取字段总数", num_fields, "取得记录集的字段总数，返回字段总数", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 9)\
    _MAKE( 15, "序号到字段名", field_name, "通过序号得到字段名，如果成功返回真，结果变量为得到的字段名，失败返回假，结果变量无效", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_mysql_global_var + 15)\
    _MAKE( 16, "字段名到序号", field_index, "通过字段名得到序号，如果成功返回真，结果变量为得到的序号，失败返回假，结果变量无效", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_mysql_global_var + 18)\
    _MAKE( 17, "查找记录", mysql_select, "查找满足条件的记录，返回记录集句柄。注意执行这条语句后不需要再执行“取记录集”命令，如果为中文字段名请在字段名两边加“`”，如：`姓名`", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_mysql_global_var + 21)\
    _MAKE( 18, "增加记录", insert, "写入信息到新记录的指定字段，执行成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_mysql_global_var + 26)\
    _MAKE( 19, "更新记录", update, "更新指定字段的数据，更新成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_mysql_global_var + 29)\
    _MAKE( 20, "删除记录", delete, "删除一条记录，删除成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_mysql_global_var + 33)\
    _MAKE( 21, "取字段宽度", field_len, "取得指定字段的名称宽度，出错返回-1", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 36)\
    _MAKE( 22, "取字段属性", field_type, "取得字段的属性，返回与字段类型常量向对应的值，出错返回-1", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 38)\
    _MAKE( 23, "取服务器版本", server_info, "取得MYSQL服务器的版本信息并设置结果变量，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 40)\
    _MAKE( 24, "取客户端版本", client_info, "取得MYSQL客户端的版本信息并设置结果变量，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 42)\
    _MAKE( 25, "创建库", create_db, "创建一个数据库。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 43)\
    _MAKE( 26, "删除库", delete_db, "删除一个数据库。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 45)\
    _MAKE( 27, "查找库", search_db, "查找一个数据库。找到返回真，没找到返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 47)\
    _MAKE( 28, "取库名列表", show_databases, "取得数据库名称的列表。成功返回真，失败返回假。结果可以通过“取记录集”取得", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 7)\
    _MAKE( 29, "创建表", create_table, "创建一个表。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_mysql_global_var + 49)\
    _MAKE( 30, "修改表", change_table, "修改一个表，注：只能做“修改类型”中指定的操作。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_mysql_global_var + 52)\
    _MAKE( 31, "删除表", drop_table, "删除一个表。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 57)\
    _MAKE( 32, "查找表", search_table, "查找一个表，找到返回真，没找到返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 59)\
    _MAKE( 33, "取表名列表", show_table, "取得当前数据库中所有表的名称列表。成功返回真，失败返回假。结果可以通过“取记录集”取得", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 7)\
    _MAKE( 34, "创建用户", create_user, "创建一个用户。成功返回真，失败返回假。具体规则请参见MYSQL相关手册", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_mysql_global_var + 61)\
    _MAKE( 35, "删除用户", delete_user, "删除一个用户。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_mysql_global_var + 68)\
    _MAKE( 36, "查找用户", search_user, "查找一个用户。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_mysql_global_var + 71)\
    _MAKE( 37, "取用户列表", show_user, "取得用户名列表。成功返回真，失败返回假。结果可以通过“取记录集”取得。注：记录集字段0为主机名，1是用户名，2是密码等，详情请参考mysql.user表", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 7)\
    _MAKE( 38, "修改用户", change_user, "修改一个用户。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_mysql_global_var + 74)\
    _MAKE( 39, "建立索引", create_index, "建立一个索引。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_mysql_global_var + 80)\
    _MAKE( 40, "删除索引", drop_index, "删除一个索引。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_mysql_global_var + 84)\
    _MAKE( 41, "取错误文本", error_info, "有错误发生返回真，并填充错误信息，没错误发生返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 87)\
    _MAKE( 42, "开始事务", begin_trans, "开始一件事务。成功返回真，失败返回假。注意 事务不可以用于MyISAM数据表类型，但是返回值为真。关于数据表类型设置请参考相关MYSQL资料", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 7)\
    _MAKE( 43, "保存事务", commit_trans, "保存一件事务。成功返回真，失败返回假。注意 事务不可以用于MyISAM数据表类型，但是返回值为真。关于数据表类型设置请参考相关MYSQL资料", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 7)\
    _MAKE( 44, "回滚事务", rollback_trans, "回滚一件事务。成功返回真，失败返回假。注意 事务不可以用于MyISAM数据表类型，但是返回值为真。关于数据表类型设置请参考相关MYSQL资料", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 7)\
    _MAKE( 45, "到MYSQL文本", mysql_type, "从易语言中的数据类型转换到可以被MYSQL识别的文本。可以将易语言中的时间，逻辑型到MYSQL中相应数据类型，转换成功返回MYSQL可以识别的文本，失败返回空文本", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 89)\
    _MAKE( 46, "写字节集字段", write_bin, "写字节集字段。本命令只用于写字段类型为字节集的字段，如果条件参数为空(或空字符串)则增加一条新记录，如果条件参数为一个条件字符串则更新符合条件的记录。注意:一次写入数据的大小MYSQL做了限制，如果需要改动请参考相应MYSQL手册，对MYSQL服务器进行调整", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_mysql_global_var + 91)\
    _MAKE( 47, "字段是否为空", field_is_null, "判断记录集中当前行的某个字段是否为空，不为空返回真，为空返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 96)\
    _MAKE( 48, "选择库", select_database, "选择一个数据库为当前用户默认使用的数据库，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 98)\
    _MAKE( 49, "关闭MySql", shupdown_mysql, "让数据库服务器关闭。连接的用户必须有shutdown权限，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 7)\
    _MAKE( 50, "返回服务器状态", mysql_status, "返回一个服务器状态字符串", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 100)\
    _MAKE( 51, "取影响行数", affected_rows, "返回一个长整数，表示刚才某个非查询SQL语句（update,delete,insert）执行后所影响到的记录数，即：返回上次UPDATE更改的行数，上次DELETE删除的行数，或上次INSERT语句插入的行数。返回值大于0表明受影响或检索的行数；返回值为0表示UPDATE语句未更新记录，在查询中没有与WHERE匹配的行，或未执行查询；返回值为-1表示未取到实际影响行数。", 1, _CMD_OS(__OS_WIN), SDT_INT64, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_mysql_global_var + 7)\
    _MAKE( 52, "输出SQL", SQL语句序号, "SQL语句的序号", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_mysql_global_var + 102)

