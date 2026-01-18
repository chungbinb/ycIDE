#pragma once

#define __ETHREAD_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 ethread_test_0_ethread
#define ETHREAD_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__ETHREAD_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "ethread_test_0_ethread"
#define ETHREAD_NAME_STR(_index, _name) ______E_FNENAME(__ETHREAD_NAME(_index, _name))

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
#define ETHREAD_DEF(_MAKE) \
    _MAKE(  0, "启动线程", CreateThread, "创建并启动一条线程，可重复使用以创建多条线程，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ethread_global_var + 0)\
    _MAKE(  1, "创建进入许可证", CreateCriticalSection, "创建并返回一个进入许可证数值，此许可证值用作进入程序中的指定许可代码区，以避免多线程冲突。成功返回非零整数值，失败返回0。所创建的许可证在不再使用后，必须使用“删除进入许可证”命令将其删除", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE(  2, "删除进入许可证", DeleteCriticalSection, "删除由“创建进入许可证”命令所创建返回的进入许可证。成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 3)\
    _MAKE(  3, "进入许可区", EnterCriticalSection, "根据已经创建的许可证进入指定许可代码区，在此线程未退出之前，其它线程如要通过同一个进入许可证进入该许可代码区则必须先等待此线程退出许可代码区，从而保证了指定许可代码区在任何时候都只能有一条线程进入并执行", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 3)\
    _MAKE(  4, "退出许可区", LeaveCriticalSection, "指示当前线程将退出许可代码区，并允许其它使用同一进入许可证的线程进入此许可代码区", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 3)\
    _MAKE(  5, "等待线程", WaitThread, "等待，直到指定线程结束或等待超时才返回。返回真表示等待成功。注意，在Linux下暂不支持超时等待。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ethread_global_var + 4)\
    _MAKE(  6, "强制结束线程", TerminateThread, "返回真表示执行成功。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 4)\
    _MAKE(  7, "关闭线程句柄", CloseThreadHandle, "返回真表示已成功关闭线程句柄。在Linux下，如果线程已经结束，本命令可能返回假。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 4)\
    _MAKE(  8, "线程池构造函数", ThreadPoolConsturct, "线程池构造函数", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE(  9, "线程池复制构造函数", ThreadPoolCopy, "线程池复制构造函数", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 10, "线程池析构函数", ThreadPoolFree, "线程池析构函数", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 11, "初始化", ThreadPool_Init, "初始化线程池, 返回是否初始化成功\r\n设计理念是: 你有一次使用到了这么多条线程, 那就会使用到第二次, 所以不会自动释放空闲的线程", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ethread_global_var + 6)\
    _MAKE( 12, "添加任务", ThreadPool_AddTask, "添加线程任务, 回调函数有多少个参数就填写多少个参数, 返回是否添加成功", -1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_ethread_global_var + 8)\
    _MAKE( 13, "销毁", ThreadPool_Destroy, "销毁线程池, 根据传递的参数决定如何销毁, 默认是等待所有任务结束", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 10)\
    _MAKE( 14, "取空闲线程数", ThreadPool_GetVacant, "取当前空闲的线程数", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 15, "取执行线程数", ThreadPool_GetExecuteCount, "取当前执行的线程数", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 16, "取队列任务数", ThreadPool_GetQueueCount, "取等待执行的任务数量, 添加到队列里还没被执行的任务", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 17, "取线程池容量", ThreadPool_size, "取当前线程池创建的线程数量", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 18, "取线程池最大容量", ThreadPool_MaxSize, "取当前线程池创建时填写的最大线程数量", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 19, "是否空闲", ThreadPool_IsVacant, "取当前线程池是否空闲", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 20, "取状态", ThreadPool_GetState, "取当前线程池状态, 0=就绪, 1=正在工作, 2=正在创建, 3=正在销毁", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 21, "启动线程Ex", CreateThreadEx, "创建并启动一条线程, 可重复使用以创建多条线程, 成功返回线程ID, 失败返回0", 1, _CMD_OS(__OS_WIN) | CT_ALLOW_APPEND_NEW_ARG, SDT_INT, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ethread_global_var + 11)\
    _MAKE( 22, "等待线程Ex", WaitThreadEx, "等待线程, 直到线程结束或者超时才返回, 返回等待的值\r\n        返回0表示线程执行完毕\r\n        返回258则表示等待超时, 线程还在运行中\r\n        返回-1表示调用失败", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_ethread_global_var + 15)\
    _MAKE( 23, "挂起线程", SuspendThread, "将指定线程的执行挂起,注意，同一个线程可以被挂起多次，如果挂起多次，就要恢复多次", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 15)\
    _MAKE( 24, "唤醒线程", ResumeThread, "唤醒挂起线程", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 15)\
    _MAKE( 25, "打开线程", OpenThread, "打开一个线程, 返回打开的线程句柄, 不使用这个句柄时需要调用\"关闭线程句柄\"释放这个句柄", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 18)\
    _MAKE( 26, "创建线程消息循环", CreateThreadMsgLoop, "创建一个线程消息循环, 返回线程ID, 可以通过线程ID向这个消息循环投递消息\r\n        本命令创建的消息循环遇到 WM_QUIT(18) 消息时就会退出消息循环", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 19)\
    _MAKE( 27, "退出线程消息循环", QuitThreadMsgLoop, "退出线程消息循环, 只能退出由 \"创建线程消息循环\" 创建的消息循环\r\n        调用本命令是给消息循环的线程投递一条 WM_QUIT(18) 的消息\r\n        \"创建线程消息循环\" 创建的消息循环遇到 WM_QUIT(18) 消息时就会退出消息循环", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 20)\
    _MAKE( 28, "投递线程消息", PostThreadMsg, "向有消息循环的线程投递一条消息, 成功返回真, 失败返回假, 返回假时可以调用 GetLastError 获取错误码\r\n        如果投递 WM_QUIT(18) 消息, 则 \"创建线程消息循环\" 创建的消息循环会退出循环\r\n    【注意】如果向其他进程投递线程消息则会有权限的问题, 如果返回假, 并且 GetLastError() 返回5, 那就是当前进程的权限不支持投递消息到目标进程", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_ethread_global_var + 21)\
    _MAKE( 29, "挂起任务", ThreadPool_Suspend, "挂起线程池所有任务线程", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 30, "唤醒任务", ThreadPool_Resume, "唤醒线程池所有任务线程", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_ethread_global_var + 0)\
    _MAKE( 31, "取信息", ThreadPool_GetInfo, "获取线程池基础信息", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_ethread_global_var + 25)

