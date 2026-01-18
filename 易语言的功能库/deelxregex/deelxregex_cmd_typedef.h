#pragma once

#define __DEELXREGEX_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 deelxregex_test_0_deelxregex
#define DEELXREGEX_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__DEELXREGEX_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "deelxregex_test_0_deelxregex"
#define DEELXREGEX_NAME_STR(_index, _name) ______E_FNENAME(__DEELXREGEX_NAME(_index, _name))

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
#define DEELXREGEX_DEF(_MAKE) \
    _MAKE(  0, "构造", Create, "创建正则对象", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE(  1, "析构", Release, "释放正则对象", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE(  2, "创建", Create, "根据正则表达式语法，对正则表达式文本进行编译。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 3, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE(  3, "构造", Create, "创建正则表达式对象", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE(  4, "析构", Release, "释放正则表达式对象", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE(  5, "查找匹配", Match, "通过 “搜索结果” 对象，可以得知是否匹配成功。如果成功，通过 “搜索结果” 对象可以获取捕获信息。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 3)\
    _MAKE(  6, "是否为空", IsEmpty, "如果本对象的内容为空，返回真；否则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE(  7, "获取开始位置", GetStart, "匹配成功后，获取所匹配到的子字符串的开始位置。如果匹配失败，则返回负值。注意：本支持库内部采用Unicode编码，返回的位置也是相对于Unicode字符串的。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE(  8, "获取结束位置", GetEnd, "匹配成功后，获取所匹配到的子字符串的结束位置。如果匹配失败，则返回负值。注意：本支持库内部采用Unicode编码，返回的位置也是相对于Unicode字符串的。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE(  9, "获取分组开始位置", GetGroupStart, "返回指定分组捕获的字符串的开始位置。如果指定分组未捕获，则返回负值。注意：本支持库内部采用Unicode编码，返回的位置也是相对于Unicode字符串的。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 5)\
    _MAKE( 10, "获取分组结束位置", GetGroupEnd, "返回指定分组捕获的字符串的结束位置。如果指定分组未捕获，则返回负值。注意：本支持库内部采用Unicode编码，返回的位置也是相对于Unicode字符串的。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 5)\
    _MAKE( 11, "取最大分组编号", MaxGroupNumber, "获取正则表达式最大捕获组编号。返回最大分组编号。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE( 12, "绝对匹配", MatchExact, "通过 “搜索结果” 对象，可以得知是否匹配成功。如果成功，通过 “搜索结果” 对象可以获取捕获信息。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 3)\
    _MAKE( 13, "获取命名分组编号", GetNamedGroupNumber, "通过命名分组名，返回命名分组编号。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 6)\
    _MAKE( 14, "对象匹配", PrepareMatch, "经过初始化的查找匹配上下文对象，用来在 “对象匹配” 中使用。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 7)\
    _MAKE( 15, "替换", Replace, "进行文本替换操作。返回替换后的文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_HIGH, 0, 0, 4, g_argumentInfo_deelxregex_global_var + 9)\
    _MAKE( 16, "释放字符串", ReleaseString, "释放字符串", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 13)\
    _MAKE( 17, "取表达式文本", GetRegExText, "返回以文本形式表示的正则表达式。如果该对象尚未创建，则返回空文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE( 18, "取结果文本", GetResultText, "取搜索结果的文本", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 14)\
    _MAKE( 19, "取子表达式个数", GetSubExpCount, "返回该表达式中子表达式（用圆括号标记）的个数。（本结果和取最大分组编号相同,只不过是为了更兼容原支持库增加的）", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE( 20, "取匹配文本", GetMatchText, "取得与整个正则表达式匹配的子文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE( 21, "取子匹配文本", GetSubMatchText, "取得与正则表达式中某个子表达式匹配的子文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 16)\
    _MAKE( 22, "是否匹配", IsMatched, "是否匹配成功。返回非零值表示匹配成功，返回 0 表示匹配失败。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE( 23, "搜索", Search, "使用指定的正则表达式搜索指定文本中与该表达式匹配的子文本。本命令和“查找匹配”一样，为兼容原支持库增加。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 17)\
    _MAKE( 24, "搜索下一个", SearchNext, "本命令和“搜索”相似，只是自动偏移开始查找匹配的位置。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE( 25, "搜索全部", SearchAll, "使用指定的正则表达式搜索指定文本中与该表达式匹配的所有子文本。返回值包含所有的搜索结果的一维数组，数组原有内容将被销毁，维数也将根据需要做相应调整。本命令的内部是通过循环多次调用“搜索”，每次指定适当的参数，来实现搜索整个文本的。返回值数组的各个成员分别对应每次调用“搜索”的返回值。本命令可高效地一次性取得目标文本中所有的匹配子文本信息。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 19)\
    _MAKE( 26, "创建W", CreateW, "根据正则表达式语法，对正则表达式文本进行编译。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 21)\
    _MAKE( 27, "查找匹配W", MatchW, "通过 “搜索结果” 对象，可以得知是否匹配成功。如果成功，通过 “搜索结果” 对象可以获取捕获信息。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 23)\
    _MAKE( 28, "搜索W", SearchW, "使用指定的正则表达式搜索指定文本中与该表达式匹配的子文本。本命令和“查找匹配”一样，为兼容原支持库增加。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 25)\
    _MAKE( 29, "搜索全部W", SearchAllW, "使用指定的正则表达式搜索指定文本中与该表达式匹配的所有子文本。返回值包含所有的搜索结果的一维数组，数组原有内容将被销毁，维数也将根据需要做相应调整。本命令的内部是通过循环多次调用“搜索”，每次指定适当的参数，来实现搜索整个文本的。返回值数组的各个成员分别对应每次调用“搜索”的返回值。本命令可高效地一次性取得目标文本中所有的匹配子文本信息。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 27)\
    _MAKE( 30, "替换W", ReplaceW, "进行文本替换操作。返回替换后的文本(Unicode格式文本，字节集类型)。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_HIGH, 0, 0, 4, g_argumentInfo_deelxregex_global_var + 29)\
    _MAKE( 31, "取表达式文本W", GetRegExTextW, "返回以文本形式表示的正则表达式(Unicode格式文本，字节集类型)。如果该对象尚未创建，则返回空文本。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE( 32, "取结果文本W", GetResultTextW, "取搜索结果的文本(Unicode格式文本，字节集类型)。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 33)\
    _MAKE( 33, "取匹配文本W", GetMatchTextW, "取得与整个正则表达式匹配的子文本(Unicode格式文本，字节集类型)。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE( 34, "取子匹配文本W", GetSubMatchTextW, "取得与正则表达式中某个子表达式匹配的子文本(Unicode格式文本，字节集类型)。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 16)\
    _MAKE( 35, "绝对匹配W", MatchExactW, "通过 “搜索结果” 对象，可以得知是否匹配成功。如果成功，通过 “搜索结果” 对象可以获取捕获信息。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 23)\
    _MAKE( 36, "测试匹配", Test, "测试表达式与匹配文本是否完全匹配，成功返回真，失败返回假。该方法常用于判断用户输入数据的合法性，比如检验Email的合法性。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 3)\
    _MAKE( 37, "测试匹配W", TestW, "测试表达式与匹配文本是否完全匹配，成功返回真，失败返回假。该方法常用于判断用户输入数据的合法性，比如检验Email的合法性。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_deelxregex_global_var + 23)\
    _MAKE( 38, "分割", Split, "使用指定的正则表达式将指定文本进行分割。返回分割后的一维文本数组。如果失败则返回一个空数组，即没有任何成员的数组。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 35)\
    _MAKE( 39, "分割W", SplitW, "使用指定的正则表达式将指定文本进行分割。返回分割后的一维字节集数组。如果失败则返回一个空数组，即没有任何成员的数组。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_BIN, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 37)\
    _MAKE( 40, "取所有匹配文本", GetAllMatchText, "取得与整个正则表达式匹配的文本。返回匹配到的一维文本数组，本命令不处理正则表达式中括号里的子文本，和“分割”命令相似，但结果不同。如果失败则返回一个空数组，即没有任何成员的数组。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 39)\
    _MAKE( 41, "取所有匹配文本W", GetAllMatchTextW, "取得与整个正则表达式匹配的文本。返回匹配到的一维字节集数组，本命令不处理正则表达式中括号里的子文本，和“分割”命令相似，但结果不同。如果失败则返回一个空数组，即没有任何成员的数组。", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_BIN, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_deelxregex_global_var + 41)\
    _MAKE( 42, "复制构造", Copy, "复制构造正则表达式对象", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)\
    _MAKE( 43, "复制构造", Copy, "复制构造搜索结果对象", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_deelxregex_global_var + 0)

