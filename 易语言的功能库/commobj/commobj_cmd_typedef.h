#pragma once

#define __COMMOBJ_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 commobj_test_0_commobj
#define COMMOBJ_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__COMMOBJ_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "commobj_test_0_commobj"
#define COMMOBJ_NAME_STR(_index, _name) ______E_FNENAME(__COMMOBJ_NAME(_index, _name))

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
#define COMMOBJ_DEF(_MAKE) \
    _MAKE(  0, "无法识别的名字_0", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(  1, "无法识别的名字_1", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(  2, "无法识别的名字_2", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(  3, "无法识别的名字_3", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(  4, "无法识别的名字_4", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(  5, "无法识别的名字_5", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(  6, "无法识别的名字_6", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(  7, "无法识别的名字_7", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(  8, "无法识别的名字_8", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(  9, "无法识别的名字_9", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 10, "无法识别的名字_10", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 11, "无法识别的名字_11", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 12, "无法识别的名字_12", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 13, "无法识别的名字_13", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 14, "无法识别的名字_14", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 15, "无法识别的名字_15", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 16, "无法识别的名字_16", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 17, "无法识别的名字_17", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 18, "无法识别的名字_18", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 19, "无法识别的名字_19", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 20, "构造函数", Constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 21, "复制构造函数", CopyConstructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 22, "析构函数", Destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 23, "取长度", GetLength, "返回本对象文本中的字符个数（每个汉字视为两个字符）。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 24, "取文本", GetText, "取本对象中的全部或部分文本。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 25, "置文本", SetText, "将参数数据转换为文本并设置为本对象文本。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_commobj_global_var + 2)\
    _MAKE( 26, "清除文本", EmptyText, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 27, "添加", Append, "将参数转换成文本并添加到本对象文本的末尾。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_commobj_global_var + 3)\
    _MAKE( 28, "插入", Insert, "将参数数据转换为文本后插入本对象文本中的指定位置之前。", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 4)\
    _MAKE( 29, "删除字符", DeleteChars, "删除文本中从指定字符开始的指定个数的字符（包括起始删除位置处的字符）", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 6)\
    _MAKE( 30, "无法识别的名字_30", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 31, "无法识别的名字_31", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 32, "无法识别的名字_32", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 33, "无法识别的名字_33", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 34, "寻找文本", Find, "寻找指定文本在本对象文本中从“起始寻找位置”开始第一次出现时的位置索引（>0），未找到则返回-1。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_commobj_global_var + 8)\
    _MAKE( 35, "倒找文本", FindReverse, "在本对象文本中从“起始寻找位置”开始向文本开头方向寻找指定文本第一次出现时的位置索引（相对于文本头），未找到则返回-1。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_commobj_global_var + 11)\
    _MAKE( 36, "替换文本", ReplaceTextW, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_commobj_global_var + 14)\
    _MAKE( 37, "替换子文本", ReplaceSubText, "在本对象文本中查找指定文本，并将其替换为其它文本。返回替换成功次数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_commobj_global_var + 17)\
    _MAKE( 38, "分割文本", SplitText, NULL, -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 22)\
    _MAKE( 39, "无法识别的名字_39", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 40, "无法识别的名字_40", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 41, "无法识别的名字_41", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 42, "无法识别的名字_42", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 43, "无法识别的名字_43", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 44, "无法识别的名字_44", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 45, "到大写", UpperCase, "将本对象文本中的全部或部分英文字母变为大写字母。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 24)\
    _MAKE( 46, "到小写", LowerCase, "将本对象文本中的全部或部分英文字母变为小写字母。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 26)\
    _MAKE( 47, "到全角", QJCase, "将本对象文本中的所有半角字符变为全角。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 48, "到半角", QJCase, "将本对象文本中的所有全角字符变为半角。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 49, "删首空", TrimLeft, "删除本对象文本中首部所有全角或半角空格。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 50, "删尾空", TrimRight, "删除本对象文本中尾部所有全角或半角空格。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 51, "删首尾空", Trim, "删除本对象文本中首部和尾部所有全角或半角空格。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 52, "删全部空", TrimAll, "删除本对象文本中所有全角或半角空格（包括首部、尾部及中间）。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 53, "无法识别的名字_53", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 54, "无法识别的名字_54", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 55, "无法识别的名字_55", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 56, "无法识别的名字_56", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 57, "无法识别的名字_57", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 58, "无法识别的名字_58", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 59, "无法识别的名字_59", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 60, "无法识别的名字_60", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 61, "无法识别的名字_61", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 62, "无法识别的名字_62", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 63, "无法识别的名字_63", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 64, "无法识别的名字_64", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 65, "无法识别的名字_65", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 66, "从文件读", ReadFromFile, "从指定的文本文件中读取其所有文本到本对象中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_commobj_global_var + 28)\
    _MAKE( 67, "写到文件", WriteToFile, "将本对象中的所有数据写入指定的文件中。如果该文件不存在，则自动创建；如果该文件已经存在，其原有内容将被直接覆盖！", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_commobj_global_var + 28)\
    _MAKE( 68, "无法识别的名字_68", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 69, "无法识别的名字_69", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 70, "取缓冲区", GetBuffer, "返回指定大小的缓冲区内存首地址。如果不能获取足够大小的缓冲区，将返回0。可以向该缓冲区直接写入文本数据，但事后必须调用“释放缓冲区()”以调整本对象内部状态。向缓冲区写入数据时请注意不要写到缓冲区之外。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_commobj_global_var + 29)\
    _MAKE( 71, "释放缓冲区", ReleaseBuffer, "本方法只是调整内部状态，并不修改内部文本。另请参考“取缓冲区()”中的说明。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 72, "置可用内存", SetMemLength, "设置本对象内部使用的最小可用内存和最大可用内存。使用本方法可以优化内存使用，但因为内部已经有适当的初始值，普通用户不必调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_commobj_global_var + 30)\
    _MAKE( 73, "置内存增量", SetMemGrowSize, "设置本对象内部使用的内存增量值。所谓“内存增量”，即每次需要申请内存时在所需内存的基础上增加的内存数量（多申请出来的内存可供后续操作使用，从而减少了申请内存的次数，提高了执行效率）。正确使用本方法可以优化内存使用并提高执行效率，但因为内部已经有适当的初始值，普通用户不必调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_commobj_global_var + 32)\
    _MAKE( 74, "无法识别的名字_74", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 75, "无法识别的名字_75", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 76, "无法识别的名字_76", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 77, "无法识别的名字_77", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 78, "无法识别的名字_78", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 79, "无法识别的名字_79", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 80, "构造函数", Constructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 81, "复制构造函数", CopyConstructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 82, "析构函数", Destructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 83, "取长度", GetLength, "单位为字节(byte)", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 84, "取字节集", GetData, "取本对象中的全部或部分字节集。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 33)\
    _MAKE( 85, "置字节集", SetData, NULL, -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_commobj_global_var + 2)\
    _MAKE( 86, "清除字节集", Empty, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 87, "添加", Append, NULL, -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_commobj_global_var + 3)\
    _MAKE( 88, "插入", Insert, "在指定的插入位置之前插入指定的字节集数据", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 35)\
    _MAKE( 89, "删除字节", DeleteBytes, "删除字节集中从指定字节开始的指定个数的字节（包括起始删除位置处的字节）", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 37)\
    _MAKE( 90, "取字节", GetByte, "取指定位置处的字节值。", -1, _CMD_OS(__OS_WIN), SDT_BYTE, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_commobj_global_var + 39)\
    _MAKE( 91, "无法识别的名字_91", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 92, "无法识别的名字_92", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 93, "无法识别的名字_93", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE( 94, "寻找字节集", Find, "寻找指定数据在本对象数据中从“起始寻找位置”开始第一次出现时的位置索引，未找到则返回-1。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 40)\
    _MAKE( 95, "倒找字节集", FindReverse, "在本对象数据中从“起始寻找位置”开始向数据开头方向寻找指定数据第一次出现时的位置索引（相对数据开头），未找到则返回-1。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 42)\
    _MAKE( 96, "替换字节集", ReplaceBin, "将本对象数据中从指定位置开始指定长度的数据替换为新的数据。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_commobj_global_var + 44)\
    _MAKE( 97, "替换子字节集", ReplaceSubBin, "在本对象数据中查找“被替换数据”，并将其替换为“替换为数据”。返回实际替换次数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_commobj_global_var + 47)\
    _MAKE( 98, "分割字节集", SplitBinary, NULL, -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_commobj_global_var + 51)\
    _MAKE( 99, "无法识别的名字_99", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(100, "无法识别的名字_100", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(101, "无法识别的名字_101", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(102, "无法识别的名字_102", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(103, "无法识别的名字_103", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(104, "无法识别的名字_104", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(105, "无法识别的名字_105", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(106, "无法识别的名字_106", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(107, "无法识别的名字_107", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(108, "无法识别的名字_108", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(109, "无法识别的名字_109", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(110, "无法识别的名字_110", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(111, "无法识别的名字_111", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(112, "无法识别的名字_112", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(113, "无法识别的名字_113", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(114, "无法识别的名字_114", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(115, "无法识别的名字_115", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(116, "无法识别的名字_116", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(117, "无法识别的名字_117", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(118, "无法识别的名字_118", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(119, "无法识别的名字_119", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(120, "无法识别的名字_120", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(121, "无法识别的名字_121", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(122, "无法识别的名字_122", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(123, "无法识别的名字_123", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(124, "无法识别的名字_124", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(125, "无法识别的名字_125", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(126, "从文件读", ReadFromFile, "从指定的文件中读取其所有数据到本对象中。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_commobj_global_var + 28)\
    _MAKE(127, "写到文件", WriteToFile, "将本对象中的数据写入指定的文件中。如果该文件不存在，则自动创建；如果该文件已经存在，其原有内容将被直接覆盖！", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_commobj_global_var + 28)\
    _MAKE(128, "无法识别的名字_128", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(129, "无法识别的名字_129", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(130, "取缓冲区", GetBuffer, "返回指定大小的缓冲区内存首地址，该地址同时也是内部数据指针。如果不能获取足够大小的缓冲区，将返回0。可以向该缓冲区直接写入数据，注意写入数据时请不要写到缓冲区之外。如果改变了缓冲区中数据的长度，必须及时调用“释放缓冲区()”。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_commobj_global_var + 53)\
    _MAKE(131, "释放缓冲区", ReleaseBuffer, "告知对象“最新的、经外界修改过的”数据长度。另请参考“取缓冲区()”中的说明。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_commobj_global_var + 54)\
    _MAKE(132, "置可用内存", SetMemLength, "设置本对象内部使用的最小可用内存和最大可用内存。使用本方法可以优化内存使用，但因为内部已经有适当的初始值，普通用户不必调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_commobj_global_var + 55)\
    _MAKE(133, "置内存增量", SetMemGrowSize, "设置本对象内部使用的内存增量值。所谓“内存增量”，即每次需要申请内存时在原来的基础上增加的内存数量（多申请出来的内存可供后续操作使用，从而减少了申请内存的次数，提高了执行效率）。正确使用本方法可以优化内存使用并提高执行效率，但因为内部已经有适当的初始值，普通用户不必调用本方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_HIGH, 0, 0, 1, g_argumentInfo_commobj_global_var + 32)\
    _MAKE(134, "无法识别的名字_134", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(135, "无法识别的名字_135", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(136, "无法识别的名字_136", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(137, "无法识别的名字_137", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(138, "无法识别的名字_138", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)\
    _MAKE(139, "无法识别的名字_139", _bunengshibie_, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_commobj_global_var + 0)

