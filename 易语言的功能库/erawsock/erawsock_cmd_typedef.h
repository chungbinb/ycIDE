#pragma once

#define __ERAWSOCK_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 erawsock_test_0_erawsock
#define ERAWSOCK_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__ERAWSOCK_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "erawsock_test_0_erawsock"
#define ERAWSOCK_NAME_STR(_index, _name) ______E_FNENAME(__ERAWSOCK_NAME(_index, _name))

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
#define ERAWSOCK_DEF(_MAKE) \
    _MAKE(  0, "创建", CreateSocket, "将创建一个用于访问指定协议报文的原始套接字。成功返回真，失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_erawsock_global_var + 0)\
    _MAKE(  1, "关闭", CloseSocket, "关闭原始套接字。成功返回真，失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_erawsock_global_var + 0)\
    _MAKE(  2, "构造IP报头", FillIPHeader, "自行填充IP报头数据。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_erawsock_global_var + 1)\
    _MAKE(  3, "构造TCP报头", FillTCPHeader, "自行填充TCP报头数据。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_erawsock_global_var + 2)\
    _MAKE(  4, "构造ICMP报头", FillICMPHeader, "自行填充ICMP报头数据。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_erawsock_global_var + 3)\
    _MAKE(  5, "接收", recvfrom, "接收所有通过的数据包。出错返回空字节集，每次接受的最大长度为20480个字节。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_erawsock_global_var + 4)\
    _MAKE(  6, "发送", sendto, "向指定的位置发送原始套接字数据报。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_erawsock_global_var + 5)\
    _MAKE(  7, "取IP报头", GetIPHeader, "从指定的字节集中取得IP报头信息。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_erawsock_global_var + 9)\
    _MAKE(  8, "取TCP报头", GetTCPHeader, "从指定的字节集中取得TCP报头信息。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_erawsock_global_var + 11)\
    _MAKE(  9, "取ICMP报头", GetICMPHeader, "从指定的字节集中取得ICMP报头信息。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_erawsock_global_var + 13)\
    _MAKE( 10, "打开网络适配器", OpenAdapter, "打开指定的网络适配器。成功返回真；失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_erawsock_global_var + 15)\
    _MAKE( 11, "配置网络适配器", SetAdapter, "设置网络适配器的属性。成功返回真；失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_erawsock_global_var + 16)\
    _MAKE( 12, "关闭网络适配器", CloseAdapter, "关闭打开的网络适配器。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_erawsock_global_var + 0)\
    _MAKE( 13, "构造以太网头", FillEthHeader, "自行填充以太网头数据。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_erawsock_global_var + 19)\
    _MAKE( 14, "构造ARP头", FillArpHeader, "自行填充ARP头数据。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_erawsock_global_var + 20)\
    _MAKE( 15, "发送", SendArpPacket, "发送指定的ARP包。成功返回真；失败返回假。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_erawsock_global_var + 21)\
    _MAKE( 16, "接收", RecvArpPacket, "接收所有通过的数据包。出错返回空字节集。", -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_erawsock_global_var + 0)\
    _MAKE( 17, "取IP报头", GetArpIPHeader, "从指定的字节集中取得IP报头信息。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_erawsock_global_var + 22)\
    _MAKE( 18, "取以太网头", GetEthHeader, "从指定的字节集中取得以太网头信息。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_erawsock_global_var + 24)\
    _MAKE( 19, "取ARP头", GetArpHeader, "从指定的字节集中取得ARP头信息。", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_erawsock_global_var + 26)

