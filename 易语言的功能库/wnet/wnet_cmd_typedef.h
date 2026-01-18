#pragma once

#define __WNET_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 wnet_test_0_wnet
#define WNET_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__WNET_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "wnet_test_0_wnet"
#define WNET_NAME_STR(_index, _name) ______E_FNENAME(__WNET_NAME(_index, _name))

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
#define WNET_DEF(_MAKE) \
    _MAKE(  0, "构造函数", Constructor, "“局域网操作”类型的构造函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_wnet_global_var + 0)\
    _MAKE(  1, "析构函数", Desstructor, "“局域网操作”类型的析构函数。隐藏", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_wnet_global_var + 0)\
    _MAKE(  2, "取共享资源", GetSharedRes, "取出当前局域网中指定类型的共享资源（包括共享目录和共享文件夹）的路径。返回取得的资源个数", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_wnet_global_var + 0)\
    _MAKE(  3, "取所有主机名", GetServerName, "取出当前局域网中所有的主机名称。返回取得的主机个数", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_wnet_global_var + 3)\
    _MAKE(  4, "取连接速度", GetConnectionSpeed, "取出与局域网内指定的共享资源（共享目录或共享打印机等）之间的连接速度。以“100比特/秒（bit/s）”为单位。如果该命令执行出错，返回-1；如果无法取出连接速度，返回0。注意，该命令取出的速度为大概值，可能有误差，仅供参考", -1, _CMD_OS(__OS_WIN), SDT_INT64, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_wnet_global_var + 4)\
    _MAKE(  5, "映射资源", MapRes, "映射一个局域网内的共享资源到本地驱动器。成功返回映射到本地的驱动器名称（比如“X:”），失败返回空文本", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_wnet_global_var + 5)\
    _MAKE(  6, "断开映射", CancelResMap, "断开一个映射到本地驱动器的共享资源。成功返回“真”，失败返回“假”", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_wnet_global_var + 11)\
    _MAKE(  7, "打开映射对话框", OpenMapDlg, "打开系统“映射网络驱动器”对话框。如果通过该对话框成功映射了一个网络驱动器，返回该驱动器的名称，否则返回空文本", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_wnet_global_var + 14)\
    _MAKE(  8, "打开中断对话框", OpenCancelMapDlg, "打开系统“中断网络驱动器”对话框", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_wnet_global_var + 19)\
    _MAKE(  9, "取对应资源", GetResPath, "取得指定的网络驱动器所对应的共享资源路径。失败返回空文本", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_wnet_global_var + 20)\
    _MAKE( 10, "取资源路径", GetUNCPath, "取得指定的网络驱动器的路径所对应的局域网资源路径、连接名称和相对路径。比如将“\\\\Prog5\\共享测试”映射到了驱动器“X:”，那么“X:\\易语言教程.doc”所对应的资源路径为“\\\\Prog5\\共享测试\\易语言教程.doc”，连接名称为“\\\\Prog5\\共享测试”，相对路径为“\\易语言教程.doc”", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_wnet_global_var + 21)\
    _MAKE( 11, "取错误信息", GetLastError, "当调用“局域网操作”的成员命令失败后，可以立即调用该命令来取出相关的错误信息。注意：请仅在调用“局域网操作”的成员命令失败后调用本命令，否则取得的信息没有意义", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_wnet_global_var + 0)

