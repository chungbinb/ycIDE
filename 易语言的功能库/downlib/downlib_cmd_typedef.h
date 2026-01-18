#pragma once

#define __DOWNLIB_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 downlib_test_0_downlib
#define DOWNLIB_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__DOWNLIB_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "downlib_test_0_downlib"
#define DOWNLIB_NAME_STR(_index, _name) ______E_FNENAME(__DOWNLIB_NAME(_index, _name))

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
#define DOWNLIB_DEF(_MAKE) \
    _MAKE(  0, "网络通讯设置", InitDownLib, "初始化下载支持库，进行基本设置.", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_downlib_global_var + 0)\
    _MAKE(  1, "设重试时间", SetTautologyTime, "设置重新尝试连接的时间", 1, _CMD_OS(__OS_WIN), _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 3)\
    _MAKE(  2, "取总下载速度", GetAllRate, "获得所有任务的下载速度", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE(  3, "备用命令2-1", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE(  4, "备用命令2-2", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE(  5, "备用命令4", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE(  6, "备用命令5", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE(  7, "备用命令6", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE(  8, "备用命令7", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE(  9, "备用命令8", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 10, "备用命令9", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 11, "备用命令10", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 12, "备用命令11", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 13, "备用命令12", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 14, "备用命令13", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 15, "备用命令14", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 16, "备用命令15", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 17, "备用命令16", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 18, "备用命令17", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 19, "增加新任务", AddDownFile, "增加一个下载文件的任务,当前支持HTTP，FTP，MMS，RTSP协议。成功返回真，失败返回假。注意：默认线程原则上没有限制。但使用时应根据具体情况设置。否则不但起不到提高速度的作用还会影响系统的稳定", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 4)\
    _MAKE( 20, "停止指定任务", StopDownFile, "停止一个下载文件的任务,当前支持HTTP，FTP协议。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_downlib_global_var + 5)\
    _MAKE( 21, "继续下载任务", ContinueDownFile, "继续一个下载文件的任务,当前支持HTTP，FTP，MMS，RTSP协议。成功返回真，失败返回假。注意：继续下载时默认线程数不是根据任务参数中的默认线程数进行设置的，而是根据任务信息中的没有完成的断点信息的数量进行设置的。断点的数量则是根据任务自本次运行以前开启的线程的总数减去已经完成下载任务的线程数获得的", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_downlib_global_var + 7)\
    _MAKE( 22, "增加线程", AddDownThread, "增加一个下载线程,当前支持HTTP，FTP，MMS协议。如果最大块的长度小于接收大小的2倍时将不能开启新线程注意：下载线程原则上可以无限增加，但是请您根据您的实际情况谨慎使用。不要过多或过与频繁的增加线程，否则不但不能提高效率，相反会影响系统的稳定", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 23, "减少线程", DecreaseDownThread, "减少一个下载线程, 当前支持HTTP，FTP，MMS协议。当只有一个下载线程工作时，将不能结束这个线程", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 24, "限制速度", SetDownRate, "限制下载速度。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 9)\
    _MAKE( 25, "取下载速度", GetDownRate, "获得下载速度，本命令获得的速度为即时速度。如，每间隔1秒调用一次本命令，那么获得的值表示1秒内收到的数据,间隔2秒则表示2秒内收到的数据", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 26, "取已下载长", GetDownFileSize, "获得任务已下载大小", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 27, "取任务数据", GetThreadSize, "获得任务数据，在指定的时间内执行本命令来取得任务数据。如果下载任务出现异常，可以通过“继续下载任务”来继续任务", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 5)\
    _MAKE( 28, "取重试次数", GetTautologyCount, "获得重试次数", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 29, "取对象信息", GetObjInfo, "获得下载对象信息,通常用于多个对象使用相同的日志或交互回调函数时，在日志或交互回调函数中确定对象，如果失败返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 30, "取线程信息", GetThreadInfo, "获得线程信息，通常用于多个对象使用相同的日志或交互回调函数时，在日志或交互回调函数中确定线程", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 10)\
    _MAKE( 31, "取任务数据项", GetStopDownObjInfo, "获得通过“停止指定任务”命令返回的任务信息中的各类数据。失败返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 3, g_argumentInfo_downlib_global_var + 11)\
    _MAKE( 32, "取本地文件路径", GetFilePath, "获得待下载文件保存在本地的路径。本命令在收到”存在同名文件“消息时使用可以获得已经存在的同名文件的路径。本命令在收到“下载上传开始”消息后使用可以获得当前下载的文件保存在本地的准确路径", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 33, "设新文件名", SetNewName, "本命令用来设置新的本地文件名。本命令必须是在处理“存在同名文件”这个消息的过程中使用。并且返回 #设置新名", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 14)\
    _MAKE( 34, "备用命令23", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 35, "备用命令24", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 36, "备用命令25", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 37, "备用命令26", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 38, "备用命令27", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 39, "下载对象析构函数", DownFreeCmd, "下载对象析构函数", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 40, "上传文件", UpDataFile, "增加一个上传文件的任务,当前支持FTP协议。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 4)\
    _MAKE( 41, "停止上传", StopUpData, "停止一个上传文件的任务,当前支持FTP协议。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 42, "限制速度", SetRate, "限制上传速度。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 15)\
    _MAKE( 43, "取上传速度", GetRate, "获得上传速度,本命令获得的速度为即时速度。如，每间隔1秒调用一次本命令，那么获得的值表示1秒内收到的数据间隔2秒则表示2秒内收到的数据", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 44, "取已上传长", GetUpDataSize, "获得任务已上传大小", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 45, "取重试数", GetUpDataTautologyCount, "获得重试次数", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 46, "设新文件名", SetNewName, "本命令用来设置新的上传文件名", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 16)\
    _MAKE( 47, "取对象信息", GetFtpObjInfo, "获得FTP上传对象信息,通常用于多个FTP上传对象使用相同的日志或交互回调函数时，在日志或交互回调函数中确定FTP上传对象, 如果失败返回-1", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 48, "备用命令29", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 49, "备用命令30", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 50, "备用命令31", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 51, "备用命令32", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 52, "备用命令33", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 53, "备用命令34", GDownLibStandbyFun, "备用命令", 1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 54, "连接FTP服务器", FtpLogin, "连接到互联网上指定地址处的FTP服务器。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 6, g_argumentInfo_downlib_global_var + 17)\
    _MAKE( 55, "断开FTP服务器", FtpQuit, "断开通过“连接FTP服务器”命令建立的到互联网上FTP服务器的连接", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 56, "删除文件", FtpDelFile, "删除FTP服务器上的指定文件。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 23)\
    _MAKE( 57, "文件改名", FtpReFileName, "更改FTP服务器上指定文件的名称。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_downlib_global_var + 24)\
    _MAKE( 58, "创建目录", FtpCreateDir, "在FTP服务器上指定位置处创建新的目录。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 26)\
    _MAKE( 59, "删除目录", FtpDelDir, "删除FTP服务器上的指定目录。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 26)\
    _MAKE( 60, "置现行目录", FtpSetCurrentDir, "设置FTP服务器上的当前目录，设置后可以在其他FTP命令中使用相对路径来指定文件。成功返回真，失败返回假", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_downlib_global_var + 26)\
    _MAKE( 61, "取现行目录", FtpGetCurrentDir, "返回FTP服务器上的当前目录。成功返回目录路径，失败返回空文本", -1, _CMD_OS(__OS_WIN), SDT_TEXT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)\
    _MAKE( 62, "取目录列表", FtpGetDirList, "返回FTP服务器上指定目录内的所有匹配文件和子目录信息。成功返回被找到的文件和子目录的数目，失败返回 -1失败的原因有可能是FTP服务器的目录结构暂时不被支持", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 5, g_argumentInfo_downlib_global_var + 27)\
    _MAKE( 63, "移动文件", FtpMoveFile, "移动文件或目录到新的位置", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_downlib_global_var + 32)\
    _MAKE( 64, "执行FTP命令", FtpRunCmd, "执行一条FTP命令, 返回服务器返回的信息代码。如果执行失败返回0。 本命令只有通过“连接FTP服务器”命令连接到FTP服务器以后才能使用。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_HIGH, 0, 0, 2, g_argumentInfo_downlib_global_var + 34)\
    _MAKE( 65, "取连接句柄", FtpGetConnectHandle, "返回与FTP服务器的连接句柄。返回的句柄可以作为参数传给FTP上传对象或采用FTP的下载对象。这样做可以是FTP上传或下载操作基于同一个连接完成，成功返回连接句柄，失败返回-1", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED, SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_downlib_global_var + 0)

