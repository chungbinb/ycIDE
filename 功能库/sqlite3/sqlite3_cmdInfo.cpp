#if !defined(__E_STATIC_LIB)
#include "include_sqlite3_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_sqlite3_global_var[] = 
{
    // 1=参数名称, 2=参数详细解释, 3=指定图像索引,从1开始,0表示无, 4=图像数目(用作动画)
    // 5=参数类型, 6=参数默认值,   7=参数标志 AS_ 开头常量
    // AS_HAS_DEFAULT_VALUE         有默认值,倒数第二个参数是默认值
    // AS_DEFAULT_VALUE_IS_EMPTY    默认值为空,有可空标志
    // AS_RECEIVE_VAR               只能传递变量,相当于传引用,传递过来的肯定不是数组
    // AS_RECEIVE_VAR_ARRAY         传递过来的肯定是数组变量引用
    // AS_RECEIVE_VAR_OR_ARRAY      传递变量或者数组变量引用
    // AS_RECEIVE_ARRAY_DATA        传递数组
    // AS_RECEIVE_ALL_TYPE_DATA     传递数组/非数组
    // AS_RECEIVE_VAR_OR_OTHER      可以传递 变量/数据/返回值数据

    /*000*/ {"参数一", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*001*/ {"数据库文件", "欲打开的数据库文件，请使用完整的路径和文件名。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*002*/ {"是否允许创建", "在指定的数据库文件不存在的情况下，如果本属性为“真”，则创建该数据库，否则直接返回“假”。本参数如果被省略，默认为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*003*/ {"欲执行的SQL语句", "多个SQL语句之间请以半角分号“;”隔开，最后一个SQL语句后面可以不加“;”。", 0, 0, SDT_TEXT, 0, NULL},

    /*004*/ {"SQL语句", "SQL语句中可包含零到多个“SQL参数”，形如“?”“:n”“$v”等，其中n代表一个数值，v代表一个文本。所有“SQL参数”均需经过“Sqlite记录集.绑定参数()”进行值绑定，未经绑定的“SQL参数”其值将视为null。", 0, 0, SDT_TEXT, 0, NULL},

    /*005*/ {"最大等待时间(单位: 秒)", NULL, 0, 0, SDT_INT, 0, NULL},

    /*006*/ {NULL, NULL, 0, 0, SDT_INT, 0, NULL},

    /*007*/ {"事务名称", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*008*/ {"表名", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*009*/ {"是否包含表", NULL, 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},
    /*010*/ {"是否包含视图", NULL, 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},
    /*011*/ {"是否包含临时表或视图", NULL, 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},

    /*012*/ {"表名称", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*013*/ {"是否包含表头", "是否在返回值数组的第一行包含字段名称。如果指定表中没有任何记录，即使本参数为“真”，也将返回空数组。", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},

    /*014*/ {"表名称", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*015*/ {"字段名称或位置", "参数值可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*016*/ {"查询条件", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*017*/ {"字段值", "其中将存放取得的字段值。如果必要，将自动进行适当的数据类型转换。", 0, 0, _SDT_ALL, 0, AS_RECEIVE_VAR},

    /*018*/ {"表名称", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*019*/ {"字段信息", NULL, 0, 0, MAKELONG(0x0A, 0), 0, AS_RECEIVE_ALL_TYPE_DATA},

    /*020*/ {"要删除的表名", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*021*/ {"要清空的表名", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*022*/ {"表名称", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*023*/ {"Sqlite数据库", "指定要打开哪个Sqlite数据库中的表。该数据库必须已经打开。", 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*024*/ {"欲跳过的记录数", "参数值如果为负数，则向前移动，否则向后移动。如果本参数被省略，默认值为 1 ，即向后移动一条记录。", 0, 0, SDT_INT, 1, AS_DEFAULT_VALUE_IS_EMPTY},

    /*025*/ {"欲跳到的记录号", "参数值所指定的记录号从 1 开始，即首记录的记录号为 1 ，依此类推。", 0, 0, SDT_INT, 0, NULL},

    /*026*/ {"查找条件", "查找条件应该类似于“字段名称=字段值”或“字段1名称=字段值1 AND 字段2名称=字段值2”等形式，也可以使用Sqlite所支持的其它SQL语法。此查找条件将作为SQL select语句中where子句的一部分。", 0, 0, SDT_TEXT, 0, NULL},

    /*027*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*028*/ {"字段值", "其中将存放取得的字段值。如果必要，将自动进行适当的数据类型转换。", 0, 0, _SDT_ALL, 0, AS_RECEIVE_VAR},

    /*029*/ {"字段名称或位置", "参数值可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*030*/ {"字段值", "指定欲写入的值", 0, 0, _SDT_ALL, 0, NULL},

    /*031*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*032*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*033*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*034*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*035*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*036*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*037*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*038*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*039*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*040*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*041*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*042*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*043*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*044*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*045*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*046*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*047*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*048*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*049*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*050*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*051*/ {"是否包含表头", "是否在返回值数组的第一行包含字段名称。如果指定表中没有任何记录，即使本参数为“真”，也将返回空数组。", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},

    /*052*/ {"SQL语句", "SQL语句中可包含零到多个“SQL参数”，形如“?”“:n”“$v”等，其中n代表一个数值，v代表一个文本。所有“SQL参数”均需经过“绑定参数()”进行值绑定，未经绑定的“SQL参数”其值将视为null。", 0, 0, SDT_TEXT, 0, NULL},
    /*053*/ {"Sqlite数据库", "指定对哪个Sqlite数据库进行查询，该数据库必须已经打开", 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*054*/ {"参数名称或索引", "使用参数名称时注意，“:”“$”也是参数名称的一部分；使用参数索引时注意，索引从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*055*/ {"要绑定的值", "如果省略本参数，默认为null。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*056*/ {"字段名称或位置", "参数值可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*057*/ {"字段值", "其中将存放取得的字段值。如果必要，将自动进行适当的数据类型转换。", 0, 0, _SDT_ALL, 0, AS_RECEIVE_VAR},

    /*058*/ {"是否包含表头", "是否在返回值数组的第一行包含字段名称。如果本记录集中没有任何记录，即使本参数为“真”，也将返回空数组。", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},

    /*059*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*060*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*061*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*062*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*063*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*064*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*065*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*066*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*067*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*068*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*069*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*070*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*071*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*072*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*073*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*074*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*075*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*076*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*077*/ {"字段名称或位置", "指定欲读取的字段，可以为一个字段名称文本或者一个字段位置数值，字段位置数值从1开始。", 0, 0, _SDT_ALL, 0, NULL},
    /*078*/ {"执行结果", "如果提供本参数，其中将存放本方法执行结果——成功为“真”，失败为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_sqlite3_global_var) / sizeof(g_argumentInfo_sqlite3_global_var[0]);
#endif


#define SQLITE3_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_sqlite3_global_var[] = 
{
    SQLITE3_DEF(SQLITE3_DEF_CMDINFO)
};

int g_cmdInfo_sqlite3_global_var_count = sizeof(g_cmdInfo_sqlite3_global_var) / sizeof(g_cmdInfo_sqlite3_global_var[0]);

#endif

