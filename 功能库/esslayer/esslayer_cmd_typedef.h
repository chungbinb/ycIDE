#pragma once

#define __ESSLAYER_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 esslayer_test_0_esslayer
#define ESSLAYER_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__ESSLAYER_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "esslayer_test_0_esslayer"
#define ESSLAYER_NAME_STR(_index, _name) ______E_FNENAME(__ESSLAYER_NAME(_index, _name))

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
#define ESSLAYER_DEF(_MAKE) \
    _MAKE(  0, "服务器的构造函数", CreateServer, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE(  1, "启动", StartServer, "启动保密服务器，绑定到指定端口，成功真，失败返回假。在启动之前必须设置相应的参数，否则启动失败", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE(  2, "停止", StopServer, "停止服务器，释放已绑定端口", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE(  3, "置连接私钥", SetLinkPrivateKey, "设置服务器连接的过程中使用的私钥和公共模数，公钥和公共模数是公开的。服务程序一旦定下来私钥和公钥，并把公钥公开后，这项连接密钥将不再改变，否则客户使用服务程序公开给他的公钥将不能再连接到服务程序上。加解密采用非对称的RSA加解密方法。该项设置服务器端的私钥和公共模数，客户端掌握公钥和公共模数，这样客户端和服务器端在连接成功后立刻使用该密钥进行验证，以确认通讯的双方是否可以相互信任，如果验证不成功，立刻断开连接。客户端掌握的公钥和公共模数和服务器的私钥和公共模数是相匹配的。设置成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_esslayer_global_var + 2)\
    _MAKE(  4, "发送数据", ServerSendData, "向指定客户端发送数据，该方法执行成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_esslayer_global_var + 4)\
    _MAKE(  5, "取消息代码", ServerGetMsgCode, "取得触发处理函数的消息代码。该方法必须在服务程序的“处理函数”中使用。返回1表示有客户连接到服务程序，返回2表示有客户主动或意外断开与服务程序的连接，返回3表示有客户发送数据到达服务程序，返回-1表示出错", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE(  6, "取客户句柄", GetClientHandle, "取得触发处理函数的客户端句柄。该方法必须在服务程序的“处理函数”中使用。正确执行返回客户的句柄，出错返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE(  7, "取客户IP", GetClientIPAddr, "取客户端IP地址，成功返回客户端IP和端口的文本形式（形如“192.168.0.8:1234”），失败返回空字符串。注意：在连接已经断开的情况下，只有在服务程序的“处理函数”中第一时间（即收到断开通知时）调用本方法，才能获取用户IP；而且，由于连接已经断开，该IP已没有实际意义，仅用于标识。", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_esslayer_global_var + 6)\
    _MAKE(  8, "取回数据", ServerGetMsgData, "取得到达服务程序的数据。该方法必须在服务程序的“处理函数”中使用。使用本方法之前首先应该通过“取消息代码”方法得到消息的代码，如果消息代码等于3再调用本方法得到客户端发送到服务器的数据。如果成功取回数据返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_esslayer_global_var + 7)\
    _MAKE(  9, "断开连接", CloseClient, "断开指定客户端连接", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_esslayer_global_var + 6)\
    _MAKE( 10, "服务析构函数", ServerRelease, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 11, "备用", BeiYong, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 12, "备用", BeiYong, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 13, "备用", BeiYong, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 14, "客户端的构造函数", CreateClient, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 15, "置连接公钥", SetLinkPublicKey, "设置客户端连接的过程中使用的公钥和公共模数，公钥和公共模数是公开的。加解密采用非对称的RSA加解密方法。服务程序掌握私钥和公共模数，这样客户端和服务器端在连接成功后立刻使用该密钥进行验证，以确认通讯的双方是否可以相互信任，如果验证不成功，立刻断开连接。客户端掌握的公钥和公共模数和服务器的私钥和公共模数是相匹配的。设置成功返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_esslayer_global_var + 8)\
    _MAKE( 16, "置加密方式", SetEncryptStyle, "设置数据网络传输过程中的加密方式，如果没有经过该项设置默认的加密方式为des方法", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_esslayer_global_var + 10)\
    _MAKE( 17, "连接", ClientConnect, "连接到保密服务器，成功真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 4, g_argumentInfo_esslayer_global_var + 11)\
    _MAKE( 18, "断开", DisconnectClient, "断开与服务器的连接", -1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 19, "取消息代码", ClientGetMsgCode, "取得触发“处理函数”的消息代码。该方法必须在客户程序的“处理函数”中使用。返回1表示有服务器主动或意外断开与客户程序的连接，返回2表示有数据到达客户程序，返回-1表示出错", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 20, "取回数据", recv_client, "取回服务程序发送到客户端的数据。该方法必须在客户程序的“处理函数”中使用。使用本方法之前首先应该通过“取消息代码”方法得到消息的代码，如果消息代码等于2再调用本方法得到服务程序发送到客户端的数据。如果成功取回数据返回真，否则返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_esslayer_global_var + 15)\
    _MAKE( 21, "发送数据", send_client, "向指定服务器发送数据", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_esslayer_global_var + 16)\
    _MAKE( 22, "客户端的析构函数", ReleaseClient, "如果客户端没有关闭的话，自动关闭", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 23, "备用", BeiYong, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 24, "备用", BeiYong, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)\
    _MAKE( 25, "备用", BeiYong, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_esslayer_global_var + 0)

