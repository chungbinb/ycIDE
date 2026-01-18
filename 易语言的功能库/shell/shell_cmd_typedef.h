#pragma once

#define __SHELL_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 shell_test_0_shell
#define SHELL_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__SHELL_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "shell_test_0_shell"
#define SHELL_NAME_STR(_index, _name) ______E_FNENAME(__SHELL_NAME(_index, _name))

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
#define SHELL_DEF(_MAKE) \
    _MAKE(  0, "创建快捷方式", CreateLink, "建立指定的快捷方式文件,返回是否成功.", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_shell_global_var + 0)\
    _MAKE(  1, "查询快捷方式", GetShortCut, "如果文件名不包含后缀，将自动加上“.lnk”后缀名", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_shell_global_var + 6)\
    _MAKE(  2, "浏览文件夹", BrowseForFolder, "打开一个对话框，浏览系统中存在的所有文件夹，返回被选定文件夹的路径名，如果未选中任何文件夹，则返回空文本。", 2, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_shell_global_var + 7)\
    _MAKE(  3, "删除到回收站", DeleteIntoRecycleBin, "本命令可以将一个或一批文件删除到Windows系统回收站中，成功返回真，失败或被用户中止删除操作返回假。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shell_global_var + 10)\
    _MAKE(  4, "进度复制文件", ShellCopyFile, "本命令可以将一个或一批文件复制到另一个目录中，并同时显示一个复制进度对话框，成功返回真，失败或被用户中止复制操作返回假。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shell_global_var + 12)\
    _MAKE(  5, "进度移动文件", ShellMoveFile, "本命令可以将一个或一批文件移动到另一个目录中，并同时显示一个移动进度对话框，成功返回真，失败或被用户中止移动操作返回假。", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shell_global_var + 14)\
    _MAKE(  6, "执行", ShellExecuteW, "使用Windows的用户界面系统执行指定命令。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_shell_global_var + 16)\
    _MAKE(  7, "取特定目录", GetSpecialFolderPath, "本命令返回指定的Windows系统特定目录，所返回目录名以“\\”符号结束，如失败返回空文本。", 3, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_shell_global_var + 21)\
    _MAKE(  8, "关闭系统", ExitWindows, "本命令可以执行关机、重启、注销等Windows系统操作。成功返回真，失败返回假。", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_shell_global_var + 22)

