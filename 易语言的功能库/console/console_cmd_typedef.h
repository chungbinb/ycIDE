#pragma once

#define __CONSOLE_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 console_test_0_console
#define CONSOLE_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__CONSOLE_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "console_test_0_console"
#define CONSOLE_NAME_STR(_index, _name) ______E_FNENAME(__CONSOLE_NAME(_index, _name))

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
#define CONSOLE_DEF(_MAKE) \
    _MAKE(  0, "构造", construct, "构造函数", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_console_global_var + 0)\
    _MAKE(  1, "析构", free, "析构函数", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_console_global_var + 0)\
    _MAKE(  2, "清屏", Clear, "清空屏幕", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_console_global_var + 0)\
    _MAKE(  3, "输入", InPut, "从控制台接收输入的数据，如果“横坐标”或“纵坐标”为空，那么从当前光标位置开始输入，注意，输入时不过滤特殊字符，如退格键等", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_console_global_var + 0)\
    _MAKE(  4, "输出", OutPut, "向控制台输出数据，如果“横坐标”或“纵坐标”为空，那么从当前光标位置开始输出", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_console_global_var + 7)\
    _MAKE(  5, "置光标位置", SetCursorPosition, "设置光标位置", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_console_global_var + 13)\
    _MAKE(  6, "取光标位置", GetCursorPosition, "获得光标位置", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_console_global_var + 15)\
    _MAKE(  7, "取显示区大小", GetScreenSize, "取显示区大小", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_console_global_var + 17)\
    _MAKE(  8, "显示光标", ShowCursor, "显示光标", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_console_global_var + 0)\
    _MAKE(  9, "隐藏光标", HideCursor, "隐藏光标", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_console_global_var + 0)\
    _MAKE( 10, "填充背景颜色", FillBackColor, "填充背景颜色，返回实际填充的长度，失败返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_console_global_var + 19)\
    _MAKE( 11, "填充字符", FillChar, "填充字符", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_console_global_var + 23)

