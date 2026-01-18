#pragma once

#define __EXMLRPC_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 exmlrpc_test_0_exmlrpc
#define EXMLRPC_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__EXMLRPC_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "exmlrpc_test_0_exmlrpc"
#define EXMLRPC_NAME_STR(_index, _name) ______E_FNENAME(__EXMLRPC_NAME(_index, _name))

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
#define EXMLRPC_DEF(_MAKE) \
    _MAKE(  0, "服务器的构造函数", CreateServer, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE(  1, "启动", StartServer, "启动服务程序。服务启动后，就可以接收客户端发出的信息，并且可以向客户端发送回复信息。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE(  2, "停止", StopServer, "停止正在工作的服务程序。由于要释放大量的系统资源，该方法执行过程中会有一点延时，应尽量避免在该方法执行过程中，强制关闭主程序。为了正确使用该方法，建议把该方法放在程序的主线程中，同步执行该方法和关闭主程序", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE(  3, "置线程池大小", SetPoolThreadNum, "设置每一个线程池内的线程数量的最大值和最小值，该项设置必须在服务器启动之前设置。服务系统拥有四个线程池，一个接收线程池一个发送线程池还有两个处理线程池，该方法是设置每一个线程池内的允许线程数量。由于线程池内的线程数量是可以随环境的变化而增加或减少的，所以该方法也就是设置线程池内的线程数量的变化区间，系统会根据该设置取最优化的值。如果不进行该项设置，系统将采用默认的设置，即最小线程数量为5，最大线程数量为20。线程数量的最小值不能小于1。设置成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlrpc_global_var + 3)\
    _MAKE(  4, "取请求文本", GetRequestText, "取得客户发出的请求信息文本。如果正确得到客户的请求信息，该方法返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlrpc_global_var + 5)\
    _MAKE(  5, "取请求字节集", GetRequestBin, "取得客户发出的请求信息字节集。如果正确得到客户的请求信息，该方法返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlrpc_global_var + 8)\
    _MAKE(  6, "发送文本", SendText, "服务程序发送文本数据到客户端。该方法执行成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlrpc_global_var + 11)\
    _MAKE(  7, "发送字节集", SendBinary, "服务程序发送字节集数据到客户端。该方法执行成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlrpc_global_var + 13)\
    _MAKE(  8, "取客户句柄", GetClientHandle, "根据客户的请求信息地址取得客户的句柄。成功取得客户句柄，返回真，如果客户已经不可用或其他意外情况发生，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlrpc_global_var + 15)\
    _MAKE(  9, "取客户IP", GetClientIPAddr, "根据客户的句柄值取得客户的IP地址。成功取得客户的IP地址，返回真，如果客户已经不可用或其他意外情况发生，返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlrpc_global_var + 17)\
    _MAKE( 10, "取客户数", GetClientCount, "取得当前连接到服务器的客户端数量。该方法正确执行，返回大于等于0的整数值，如果发生错误返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE( 11, "取客户数组", GetAllClient, "取得当前连接到服务器的所有客户端的句柄。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlrpc_global_var + 19)\
    _MAKE( 12, "断开连接", Disconnect, "断开与一个客户的连接", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlrpc_global_var + 20)\
    _MAKE( 13, "服务析构函数", ServerRelease, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE( 14, "客户端的构造函数", CreateClient, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE( 15, "连接", ClientConnect, "连接到指定地址和端口号的服务器。连接成功返回真，连接失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_exmlrpc_global_var + 21)\
    _MAKE( 16, "断开", ClientdisConnect, "切断与服务器的连接", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE( 17, "同步发送文本", SendSynRequestText, "在客户端发送同步请求文本到服务器，当收到服务器返回的结果后，该方法才返回，否则为阻塞状态直到超时。如果该方法正确执行并接收到服务器返回的结果，该方法返回1。如果在超时时间内没能接收到服务器的返回结果，该方法返回0。如果出现其他错误没能接收到服务器的返回结果，该方法返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlrpc_global_var + 25)\
    _MAKE( 18, "同步发送字节集", SendSynRequestBin, "在客户端发送同步请求字节集到服务器，当收到服务器返回的结果后，该方法才返回，否则为阻塞状态直到超时。如果该方法正确执行并接收到服务器返回的结果，该方法返回1。如果在超时时间内没能接收到服务器的返回结果，该方法返回0。如果出现其他错误没能接收到服务器的返回结果，该方法返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_exmlrpc_global_var + 28)\
    _MAKE( 19, "异步发送文本", SendAsynRequestText, "客户端发送异步请求文本到服务器。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlrpc_global_var + 31)\
    _MAKE( 20, "异步发送字节集", SendAsynRequestBin, "客户端发送异步请求字节集到服务器。该方法执行成功返回真，执行失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlrpc_global_var + 32)\
    _MAKE( 21, "取返回文本", GetResultText, "在异步的请求方式下，取得服务器返回的结果。该方法必须在客户端的触发函数中调用。如果正确取得服务器的返回结果，该方法返回真，否则，该方法返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlrpc_global_var + 33)\
    _MAKE( 22, "取返回字节集", GetResultBin, "在异步的请求方式下，取得服务器返回的结果。该方法必须在客户端的触发函数中调用。如果正确取得服务器的返回结果，该方法返回真，否则，该方法返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_exmlrpc_global_var + 35)\
    _MAKE( 23, "客户端的析构函数", ReleaseClient, "如果客户端没有关闭的话，自动关闭", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE( 24, "取消息类型", ClientGetMsgKind, "取得触发客户端处理函数的消息类型。该方法必须在客户端的触发函数中调用。如果服务器关闭或者与客户的连接意外断开返回0，如果客户端接收到数据返回1，出错返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlrpc_global_var + 37)\
    _MAKE( 25, "备用", BeiYong, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE( 26, "备用", BeiYong, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)\
    _MAKE( 27, "取消息类型", GetMsgKind, "取得触发服务端处理函数的消息类型。有客户连接到服务端返回0，有客户主动或意外断开与服务端的连接返回1，客户端发送字节流到服务器返回2，出错返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_exmlrpc_global_var + 38)\
    _MAKE( 28, "置超时时间", ServerSetTimeout, "设置网络超时时间。不同的网络环境下的网络传输的情况是不同的，而且局域网和互联网的差别是相当大的，所以在不同的网络环境下要设置相应的超时时间。这里指的超时时间是指服务器和客户端一次交互的超时时间，一般的收发数据服务器要和客户端进行多次交互才能完成，每次最多能够收发10K左右的字节，每次交互都会检查是否超时。这个设置非常重要，如果设置的不合理，会影响服务器的性能，默认情况下服务器的超时时间为300毫秒", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_exmlrpc_global_var + 0)

