#pragma once

#define __ICONV_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 iconv_test_0_iconv
#define ICONV_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__ICONV_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "iconv_test_0_iconv"
#define ICONV_NAME_STR(_index, _name) ______E_FNENAME(__ICONV_NAME(_index, _name))

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
#define ICONV_DEF(_MAKE) \
    _MAKE(  0, "编码转换", iconv, "将指定的数据从一种编码转换为另一种编码，返回转换后的数据。如果执行失败，将返回空字节集，同时设置参数“执行结果”为“假”。本命令相当于“编码转换_打开()”“编码转换_转换()”“编码转换_关闭()”三个命令的组合使用。", 1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_iconv_global_var + 0)\
    _MAKE(  1, "编码转换_打开", iconv_open, "初始化编码转换环境，返回一个句柄，供后续“编码转换_转换()”命令使用。如果本命令返回值为-1，表示执行失败。本命令返回的有效句柄必须使用“编码转换_关闭()”命令关闭。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_iconv_global_var + 4)\
    _MAKE(  2, "编码转换_转换", iconv_iconv, "对指定数据进行编码转换，返回转换后的数据。对同一个“转换句柄”，可多次执行本命令，直到该句柄被关闭。如果执行失败，将返回空字节集，同时设置参数“执行结果”为“假”。", 1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_iconv_global_var + 6)\
    _MAKE(  3, "编码转换_关闭", iconv_close, "关闭由“编码转换_打开()”命令所返回的转换句柄，释放相关资源。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_iconv_global_var + 6)

