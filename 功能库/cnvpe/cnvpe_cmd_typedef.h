#pragma once

#define __CNVPE_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 cnvpe_test_0_cnvpe
#define CNVPE_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__CNVPE_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "cnvpe_test_0_cnvpe"
#define CNVPE_NAME_STR(_index, _name) ______E_FNENAME(__CNVPE_NAME(_index, _name))

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
#define CNVPE_DEF(_MAKE) \
    _MAKE(  0, "转换可执行文件数据", cnvpe, "转换所提供的使用易语言编译的可执行文件或易语言支持库内容数据，使其内容完全被改变，但不影响其正常使用。返回转换后的结果字节集。如果转换失败，将返回空字节集，同时如果参数中提供了转换结果获取变量，其中将返回具体的错误代码值。本命令主要用作应付一些使用落后的特征码查毒技术的杀毒软件，使其不再或很难对易语言编译出来的程序和易语言本身支持库误报，建议大家在发布自己的软件前使用本命令转换下编译后的可执行文件和所需要携带的支持库。注意：1、多次重复转换可能会导致转换后的可执行文件或易语言支持库无法使用，请确保只转换一次；2、对于非易语言编译的可执行文件或DLL，不保证转换后能够正常使用", 1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_cnvpe_global_var + 0)

