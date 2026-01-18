#pragma once

#define __INTERNET_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 internet_test_0_internet
#define INTERNET_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__INTERNET_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "internet_test_0_internet"
#define INTERNET_NAME_STR(_index, _name) ______E_FNENAME(__INTERNET_NAME(_index, _name))

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
#define INTERNET_DEF(_MAKE) \
    _MAKE(  0, "连接发信服务器", ConnectSmtpServer, "连接到指定的SMTP邮件发送服务器，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_internet_global_var + 0)\
    _MAKE(  1, "断开发信服务器", DisconnectSmtpServer, "断开通过“连接发信服务器”命令建立的到SMTP邮件发送服务器的连接", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_internet_global_var + 0)\
    _MAKE(  2, "添加附件文件", AttachFile, "添加指定的文件附件到即将发送的邮件中，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_internet_global_var + 5)\
    _MAKE(  3, "添加附件数据", AttachData, "添加附件数据到即将发送的邮件中，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_internet_global_var + 6)\
    _MAKE(  4, "清除所有附件", EmptyAttachment, "清除所有使用“添加附件文件”和“添加附件数据”命令添加到即将发送邮件中的附件数据", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_internet_global_var + 0)\
    _MAKE(  5, "发送邮件", SendMail, "发送邮件到指定信箱，注意邮件中包含所有使用“添加附件文件”和“添加附件数据”命令添加的附件。成功返回空文本，失败返回具体错误信息文本", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_internet_global_var + 8)\
    _MAKE(  6, "置代理服务器", SetProxyName, "设置本库此后的HTTP和FTP新建连接所使用的代理服务器信息。对FTP操作而言，目前不支持代理服务器验证，将忽略用户名和密码参数。", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_internet_global_var + 15)\
    _MAKE(  7, "HTTP读文件", GetHttpFile, "获取互联网上指定HTTP地址处的网页或数据文件，返回文件内容字节集", 2, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_internet_global_var + 18)\
    _MAKE(  8, "连接FTP服务器", ConnectFTPServer, "连接到互联网上指定地址处的FTP服务器。在进行所有FTP操作之前必须首先成功连接FTP服务器。成功返回真，失败返回假", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_internet_global_var + 19)\
    _MAKE(  9, "断开FTP服务器", DisconnectFTPServer, "断开通过“连接FTP服务器”命令建立的到互联网上FTP服务器的连接，本命令执行后不能再进行FTP操作", 2, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_internet_global_var + 0)\
    _MAKE( 10, "FTP文件下载", GetFtpFile, "将FTP服务器上的指定文件下载到本地，成功返回真，失败返回假", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_internet_global_var + 24)\
    _MAKE( 11, "FTP文件上传", PutFtpFile, "将本地文件上传到FTP服务器上的指定位置，成功返回真，失败返回假", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_internet_global_var + 27)\
    _MAKE( 12, "FTP删除文件", DeleteFtpFile, "删除FTP服务器上的指定文件。成功返回真，失败返回假", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_internet_global_var + 30)\
    _MAKE( 13, "FTP文件改名", RenameFtpFile, "更改FTP服务器上指定文件的名称。成功返回真，失败返回假", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_internet_global_var + 31)\
    _MAKE( 14, "FTP创建目录", CreateFtpDir, "在FTP服务器上指定位置处创建新的目录。成功返回真，失败返回假", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_internet_global_var + 33)\
    _MAKE( 15, "FTP删除目录", RemoveFtpDir, "删除FTP服务器上的指定目录。成功返回真，失败返回假", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_internet_global_var + 33)\
    _MAKE( 16, "FTP置现行目录", SetCurrentFtpDir, "设置FTP服务器上的当前目录，设置后可以在其他FTP命令中使用相对路径来指定文件。成功返回真，失败返回假", 2, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_internet_global_var + 33)\
    _MAKE( 17, "FTP取现行目录", GetCurrentFtpDir, "返回FTP服务器上的当前目录。成功返回目录路径，失败返回空文本", 2, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_internet_global_var + 0)\
    _MAKE( 18, "FTP目录列表", ListFtpDir, "返回FTP服务器上指定目录内的所有匹配文件和子目录信息。成功返回被找到的文件和子目录的数目，失败返回 0 ", 2, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_internet_global_var + 34)\
    _MAKE( 19, "取拨号连接数", GetEntriesCount, "返回用户在Windows系统中已建立的拨号连接配置数目", 3, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_internet_global_var + 0)\
    _MAKE( 20, "取连接名称", GetEntryName, "返回指定拨号连接配置的名称", 3, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_internet_global_var + 40)\
    _MAKE( 21, "取用户帐号", GetUserNameW, "返回指定拨号连接配置所使用的用户帐号名称", 3, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_internet_global_var + 40)\
    _MAKE( 22, "拨号", RasDial, "开始拨号并试图建立网络连接。成功返回真，失败返回假。注意如果“是否等待连接完毕”参数为假，则即使本命令返回真，也并不意味着马上就可以使用该网络连接，必须等到“是否已在线”命令返回真时才可以。如果“是否等待连接完毕”参数为真，则本命令会一直等待到连接操作执行完毕后才会返回。另外，建议在执行本命令前调用“是否已在线”命令查看当前是否已经连接到网络上，如果已经连接，则不需要再执行本命令", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_internet_global_var + 41)\
    _MAKE( 23, "是否已在线", IsOnline, "如果当前已经存在可用的网络连接(即已连接到网络上)，返回真，否则返回假", 3, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_internet_global_var + 0)\
    _MAKE( 24, "挂断", RasHangUp, "挂断当前拨号网络连接。一旦不再需要拨号网络连接，就可以使用本命令将连接断开", 3, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_internet_global_var + 0)

