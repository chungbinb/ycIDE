#pragma once

#define __POP3_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 pop3_test_0_pop3
#define POP3_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__POP3_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "pop3_test_0_pop3"
#define POP3_NAME_STR(_index, _name) ______E_FNENAME(__POP3_NAME(_index, _name))

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
#define POP3_DEF(_MAKE) \
    _MAKE(  0, "构造函数", MailInfo, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE(  1, "拷贝构造函数", CopyMailInfo, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE(  2, "析构函数", MailInfo, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE(  3, "取发件人地址", GetSendAddress, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE(  4, "取主题", GetSubject, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE(  5, "取日期", GetDate, NULL, -1, _CMD_OS(__OS_WIN), SDT_DATE_TIME, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE(  6, "取大小", GetSize, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE(  7, "取文本内容", GetTextContent, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE(  8, "取附件个数", GetAttNum, NULL, -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE(  9, "取附件", GetAttachment, NULL, -1, _CMD_OS(__OS_WIN) | CT_RETRUN_ARY_TYPE_DATA, MAKELONG(0x02, 0), 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 10, "连接收信服务器", ConnectPop3Server, "连接到指定的POP3邮件接收服务器，成功返回真，失败返回假", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 11, "断开收信服务器", DisConnectPop3Server, "断开通过“连接收信服务器”命令建立的到POP3邮件接收服务器的连接。", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 12, "获取邮件信息", GetMailInfo, "获取目前服务器上邮件的个数和大小。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_pop3_global_var + 6)\
    _MAKE( 13, "获取邮件大小", GetMailSize, "获取每一封邮件的大小。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_pop3_global_var + 8)\
    _MAKE( 14, "接收邮件", ReceiveMail, "返回“邮件信息”数据类型。", 1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_pop3_global_var + 11)\
    _MAKE( 15, "取邮件错误信息", GetMailErrorMsg, "取到最后一次的错误信息。", 1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 16, "取大小", GetAttSize, NULL, -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 17, "取类型", GetAttType, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 18, "取文件名", GetAttFilename, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 19, "取编码方式", GetAttEncode, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 20, "取数据", GetAttData, NULL, -1, _CMD_OS(__OS_WIN), SDT_BIN, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 21, "取发件人名称", GetSenderName, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 22, "取回复地址", GetReplyAddress, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 23, "取原始信息", GetOriMessage, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 24, "删除邮件", DeleteMail, "将服务器上某封邮件标记为删除", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_pop3_global_var + 12)\
    _MAKE( 25, "复位邮件", ResetMail, "将服务器上标记为删除的邮件标记为未删除", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 26, "取超文本内容", GetHTMLContent, NULL, -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 27, "取是否嵌入式附件", GetIsRelate, NULL, -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 28, "取名称", GetName, "此命令取出的名称用作文件名将嵌入式附件保存到磁盘上。包含超文本内容中的图片等（注：必需将它们放在同一个目录下）", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_pop3_global_var + 0)\
    _MAKE( 29, "接收邮件序号", GetMailNo, "此序号对于每个服务器是唯一的，可用来比较邮件是否接收到本地。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_pop3_global_var + 13)\
    _MAKE( 30, "接收邮件前几行", GetMailTop, "从服务器上获取邮件前几行的内容。", 1, _CMD_OS(__OS_WIN), MAKELONG(0x01, 0), 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_pop3_global_var + 15)\
    _MAKE( 31, "注册邮件接收回调函数", SetMailRecvCallBack, "注册一个回调函数，主要用于接收邮件时的进度显示。注：此回调函数中不要进行耗时的操作，否则邮件服务器会断开!不要调用邮件接收支持库中和服务器交互的命令，否则可能出现死循环!", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_pop3_global_var + 17)\
    _MAKE( 32, "设置代理服务器", SetProxyServer, "设置代理服务器信息。注：此方法要在“连接收信服务器”命令之前调用。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_pop3_global_var + 18)

