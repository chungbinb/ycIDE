#pragma once

#define __SOCK_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 sock_test_0_sock
#define SOCK_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__SOCK_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "sock_test_0_sock"
#define SOCK_NAME_STR(_index, _name) ______E_FNENAME(__SOCK_NAME(_index, _name))

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
#define SOCK_DEF(_MAKE) \
    _MAKE(  0, "启动", start_server, "启动网络服务器，绑定到指定端口，成功真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sock_global_var + 0)\
    _MAKE(  1, "停止", stop_server, "停止网络服务器，释放已绑定端口，成功真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)\
    _MAKE(  2, "监听", accept_server, "监听一个端口，等待一个客户端连接，如果设置为无限等待则有连接返回客户端句柄，如果设置等待时间，在等待时间内无连接则返回0，在等待时间之内有连接则返回客户端句柄,出错返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sock_global_var + 1)\
    _MAKE(  3, "接收", recv_server, "接收来自客户端的数据，每次接受的最大长度为20480个字节。如果出错，将向第三个参数写入假，并返回空字节集。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_sock_global_var + 2)\
    _MAKE(  4, "发送", send_server, "向指定客户端发送数据", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_sock_global_var + 5)\
    _MAKE(  5, "断开连接", close_client, "断开指定客户端连接", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sock_global_var + 2)\
    _MAKE(  6, "取客户IP", get_client_ip, "取客户端IP,成功返回客户端IP，失败返回空字符串", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sock_global_var + 2)\
    _MAKE(  7, "取客户端口", get_client_port, "取客户端口,成功返回客户端口，失败返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sock_global_var + 2)\
    _MAKE(  8, "连接", connect_client, "连接到网络服务器,成功真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sock_global_var + 8)\
    _MAKE(  9, "断开", client_close, "断开与网络服务器的连接,成功真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)\
    _MAKE( 10, "接收", recv_client, "接收来自网络服务器的数据，每次接受的最大长度为20480个字节。如果出错，将向第二个参数写入假，并返回空字节集。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sock_global_var + 10)\
    _MAKE( 11, "发送", send_client, "向指定网络服务器发送数据", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_sock_global_var + 12)\
    _MAKE( 12, "配置", setup_udp, "配置网络数据报,成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_sock_global_var + 14)\
    _MAKE( 13, "关闭", close_udp, "关闭网络数据报，成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)\
    _MAKE( 14, "接收", recvfrom_udp, "接收来自网络数据报的数据，每次接受的最大长度为20480个字节。如果出错，将向第三个参数写入假，并返回空字节集。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_sock_global_var + 15)\
    _MAKE( 15, "发送", sendto_udp, "向指定网络数据报发送数据", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_sock_global_var + 18)\
    _MAKE( 16, "取本机名", get_local_name, "取本机名,成功返回本机名，失败返回空字符串", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)\
    _MAKE( 17, "取本机IP", get_local_ip, "取本机IP,返回IP列表数组", 1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)\
    _MAKE( 18, "取本机端口", get_local_port, "取本机端口,成功返回本机端口，失败-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)\
    _MAKE( 19, "取本机端口", get_local_port_old, "本命令已经被“网络客户端”对象内的取本机端口本命令所取代，所以本已经作废.如果执行则返回-1", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)\
    _MAKE( 20, "析构函数", server_obj_free_cmd, "服务器对象析构函数", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)\
    _MAKE( 21, "析构函数", client_obj_free_cmd, "客户端对象析构函数", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)\
    _MAKE( 22, "析构函数", udp_obj_free_cmd, "数据报对象析构函数", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_sock_global_var + 0)

