#pragma once

#define __EINTERPROCESS_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 einterprocess_test_0_einterprocess
#define EINTERPROCESS_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__EINTERPROCESS_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "einterprocess_test_0_einterprocess"
#define EINTERPROCESS_NAME_STR(_index, _name) ______E_FNENAME(__EINTERPROCESS_NAME(_index, _name))

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
#define EINTERPROCESS_DEF(_MAKE) \
    _MAKE(  0, "创建命名管道", CreateNamedPipeW, "命名管道可在不同进程之间，支持可靠的、单向或双向的数据通信。Win9x操作系统下不支持该命令。创建成功返回命名管道的句柄，否则返回0", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE(  1, "监听命名管道", ListenNamedPipe, "监听一个已经创建好的命名管道，等待客户的连接，如果有客户连接则返回，没有客户连接的情况下，该方法处于“锁定”模式，不再向下执行。如果监听失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_einterprocess_global_var + 1)\
    _MAKE(  2, "连接命名管道", ConnectNamedPipe, "连接一个已经创建完毕并在监听的命名管道。连接成功返回管道的句柄，失败返回0", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_einterprocess_global_var + 2)\
    _MAKE(  3, "读命名管道", ReadNamedPipe, "从命名管道中读取数据。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_einterprocess_global_var + 4)\
    _MAKE(  4, "写命名管道", WriteNamedPipe, "向命名管道中写入数据，返回实际写入到命名管道中的字节数", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_einterprocess_global_var + 6)\
    _MAKE(  5, "断开管道连接", DisConnectNamedPipe, "一个命名管道的客户断开与命名管道的连接", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_einterprocess_global_var + 8)\
    _MAKE(  6, "关闭命名管道", ReleaseNamedPipe, "关闭并释放已经创建的命名管道。如果调用了“监听命名管道”，必须等待“监听命名管道”方法执行完毕后才能执行该方法，否则该方法执行过程中处于“锁定”状态", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_einterprocess_global_var + 1)\
    _MAKE(  7, "创建", ConstructorServer, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE(  8, "析构", DestructorServer, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE(  9, "创建", CreateMailslotServer, "创建成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_einterprocess_global_var + 9)\
    _MAKE( 10, "读数据", rDataFromServer, "只能在服务器端从邮槽中取出数据。读取成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_einterprocess_global_var + 10)\
    _MAKE( 11, "关闭", CloseMailslotServer, "关闭邮槽服务器", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE( 12, "创建", ConstructorClient, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE( 13, "析构", DestructorClient, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE( 14, "创建", CreateMailslotClient, "邮槽客户机只能向邮槽中写入数据，不能从中读出数据。创建成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_einterprocess_global_var + 11)\
    _MAKE( 15, "写数据", wDataToClient, "只能在客户机端向邮槽中写入数据。注意写入的数据长度有限制，最大的长度为424字节，如果通讯数据的长度太大，最好选用“命名管道”。写入成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_einterprocess_global_var + 13)\
    _MAKE( 16, "关闭", CloseMailslotClient, "关闭已经创建的邮槽客户机", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE( 17, "映射文件构造", MapConstructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE( 18, "映射文件析构", MapDestructor, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE( 19, "创建映射文件", CreateMapFile, "创建内存映射文件。执行成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_einterprocess_global_var + 14)\
    _MAKE( 20, "打开映射文件", OpenMapFile, "打开一个其他进程创建的内存映射文件。执行成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_einterprocess_global_var + 18)\
    _MAKE( 21, "映射到内存", MapToMemory, "将一个创建完毕的或已打开的内存映射文件的全部或一部分映射到进程的内存空间中。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_einterprocess_global_var + 19)\
    _MAKE( 22, "读数据", ReadMapFile, "从已经创建或打开的内存映射文件中读出数据，成功返回真，失败返回假。注意：使用该命令时一定要注意不能超过映射文件的边界，否则会造成非法的内存访问，意外退出", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_einterprocess_global_var + 21)\
    _MAKE( 23, "写数据", WriteMapFile, "向已经创建或打开的内存映射文件中写入数据，成功写入返回真，失败返回假。注意：使用该命令时一定要注意不能超过映射文件的边界，否则会造成非法的内存访问，意外退出", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_einterprocess_global_var + 24)\
    _MAKE( 24, "解除映射", UnMapToMemory, "“映射到内存”方法的反操作，将已经映射到内存的文件解除映射", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)\
    _MAKE( 25, "关闭映射文件", CloseMapFile, "关闭已经创建或打开的内存映射文件", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_einterprocess_global_var + 0)

