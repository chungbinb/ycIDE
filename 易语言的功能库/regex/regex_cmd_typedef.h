#pragma once

#define __REGEX_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 regex_test_0_regex
#define REGEX_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__REGEX_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "regex_test_0_regex"
#define REGEX_NAME_STR(_index, _name) ______E_FNENAME(__REGEX_NAME(_index, _name))

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
#define REGEX_DEF(_MAKE) \
    _MAKE(  0, "匹配", Match, "用一个正则表达式来匹配一个文本。如果这个文本符合正则表达式指定的模式，返回真；否则返回假。比如：\r\n        正则表达式.创建 (“易语言4\\.0(模块|支持库)?”)\r\n        信息框 (正则表达式.匹配 (“易语言4.0支持库”), 0, )\r\n    将显示“真”", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_regex_global_var + 0)\
    _MAKE(  1, "构造函数", Constructor, "“正则表达式”类型的构造函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_regex_global_var + 0)\
    _MAKE(  2, "析构函数", Desstructor, "“正则表达式”类型的析构函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_regex_global_var + 0)\
    _MAKE(  3, "创建", Create, "根据指定的正则表达式文本创建一个正则表达式对象。本对象中的原有内容将被释放。成功返回真；否则返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_regex_global_var + 1)\
    _MAKE(  4, "是否为空", IsEmpty, "如果本对象的内容为空或尚未创建，返回真；否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_regex_global_var + 0)\
    _MAKE(  5, "取文本", GetText, "返回以文本形式表示的正则表达式。如果该对象尚未创建，则返回空文本", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_regex_global_var + 0)\
    _MAKE(  6, "取子表达式个数", GetSubExpCount, "返回该表达式中子表达式（用圆括号标记）的个数。比如“易语言4\\.0(模块|支持库)?”中包含1个子表达式：“(模块|支持库)”", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_regex_global_var + 0)\
    _MAKE(  7, "搜索", Search, "使用指定的正则表达式搜索指定文本中与该表达式匹配的子文本。注意，出于搜索效率考虑，被搜索的文本没有存放在返回的“搜索结果”中，所以在调用“搜索结果.取匹配文本”和“搜索结果.取子匹配文本”时，请注意给“被搜索的文本”传递的参数应该和对应的本命令的“欲被搜索的文本”参数相同。", -1, _CMD_OS(__OS_WIN), MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 3, g_argumentInfo_regex_global_var + 3)\
    _MAKE(  8, "构造函数", Constructor, "“搜索结果”类型的构造函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_regex_global_var + 0)\
    _MAKE(  9, "析构函数", Desstructor, "“搜索结果”类型的析构函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_regex_global_var + 0)\
    _MAKE( 10, "是否为空", IsEmpty, "如果本对象的内容为空，返回真；否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_regex_global_var + 0)\
    _MAKE( 11, "取匹配文本", GetMatchText, "取得与整个正则表达式匹配的子文本", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_regex_global_var + 6)\
    _MAKE( 12, "取子匹配文本", GetSubMatchText, "取得与正则表达式中某个子表达式匹配的子文本", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_HIGH, 0, 0, 3, g_argumentInfo_regex_global_var + 8)\
    _MAKE( 13, "复制构造函数", CopyConstructor, "“正则表达式”类型的复制构造函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_regex_global_var + 11)\
    _MAKE( 14, "复制构造函数", CopyConstructor, "“搜索结果”类型的复制构造函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_regex_global_var + 12)\
    _MAKE( 15, "替换", Replace, "使用指定的正则表达式搜索指定文本中与该表达式匹配的子文本，并按照指定的格式进行替换。返回替换后的文本", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_HIGH, 0, 0, 6, g_argumentInfo_regex_global_var + 13)\
    _MAKE( 16, "搜索全部", SearchAll, "使用指定的正则表达式搜索指定文本中与该表达式匹配的所有子文本。返回值包含所有的搜索结果的一维数组，数组原有内容将被销毁，维数也将根据需要做相应调整。本命令的内部是通过循环多次调用“搜索”，每次指定适当的参数，来实现搜索整个文本的。返回值数组的各个成员分别对应每次调用“搜索”的返回值。本命令可高效地一次性取得目标文本中所有的匹配子文本信息", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x02, 0), 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_regex_global_var + 3)\
    _MAKE( 17, "取分组名称索引", GetGroupNameIndex, "获取指定的正则表达式中的分组名称所在的分组索引,即(?<Name>)的name所在的分组索引", -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_regex_global_var + 19)

