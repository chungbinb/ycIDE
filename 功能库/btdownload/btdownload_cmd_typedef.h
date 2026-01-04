#pragma once

#define __BTDOWNLOAD_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__E_FNENAME)##_##_name##_##_index##_

// 传递函数名和索引, 拼接成 定义库名_名字_序号_定义库名, 比如 btdownload_test_0_btdownload
#define BTDOWNLOAD_NAME(_index, _name) __LIB2_FNE_NAME_LEFT(__BTDOWNLOAD_NAME(_index, _name))__LIB2_FNE_NAME_LEFT(__E_FNENAME)

// 传递函数名和索引, 拼接成 "定义库名_名字_序号_定义库名", 比如 "btdownload_test_0_btdownload"
#define BTDOWNLOAD_NAME_STR(_index, _name) ______E_FNENAME(__BTDOWNLOAD_NAME(_index, _name))

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
#define BTDOWNLOAD_DEF(_MAKE) \
    _MAKE(  0, "构造函数", con, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE(  1, "析构函数", des, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE(  2, "增加新任务", AddNewTask, "增加一个下载任务。注：本函数返回真并不是已经真正开始下载了，要通过“其它日志”中的“下载已全部停止”的参数三来判断。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE(  3, "暂停本任务", PauseTask, "暂停一个正在下载的任务，可以用“继续本任务”方法使本任务继续运行。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE(  4, "继续本任务", ContinueTask, "继续下载一个暂停的任务。如果发现下载的文件丢失或数据有误，可在“重新检查完整性”后指定新的任务内容。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_btdownload_global_var + 1)\
    _MAKE(  5, "停止本任务", StopTask, "停止一个运行中的任务。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE(  6, "增加连接", AddConnect, "增加一个连接。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_btdownload_global_var + 2)\
    _MAKE(  7, "减少连接", DelConnect, "减少一个连接。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE(  8, "取下载速度", GetDownloadRate, "获得下载速度，本命令获得的速度为即时速度。如，每间隔1秒调用一次本命令，那么获得的值表示1秒内收到的数据，间隔2秒则表示2秒内收到的数据。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE(  9, "取上传速度", GetUploadRate, "获得上传速度，本命令获得的速度为即时速度。如，每间隔1秒调用一次本命令，那么获得的值表示1秒内发送的数据，间隔2秒则表示2秒内发送的数据。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE( 10, "info构造函数", StopTask, "停止一个运行中的任务。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE( 11, "intfo拷贝构造函数", StopTask, "停止一个运行中的任务。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE( 12, "info析构函数", StopTask, "停止一个运行中的任务。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE( 13, "分析发布文件", AnalyseTorrentFile, "分析发布文件(Torrent文件)，取到其中的信息。", 1, _CMD_OS(__OS_WIN), MAKELONG(0x05, 0), 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_btdownload_global_var + 4)\
    _MAKE( 14, "制做发布文件", MakeTorrentFile, "制做发布文件(Torrent文件)。返回0表示成功，-1表示参数错误，-2表示编码出错，-3表示写文件失败, -4表示计算SHA1值失败。", 1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_btdownload_global_var + 5)\
    _MAKE( 15, "task构造函数", StopTask, "停止一个运行中的任务。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_CONSTURCT_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE( 16, "task拷贝构造函数", StopTask, "停止一个运行中的任务。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE( 17, "task析构函数", StopTask, "停止一个运行中的任务。", -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_FREE_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE( 18, "限制下载速度", SetDownloadRate, "限制下载速度。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_btdownload_global_var + 12)\
    _MAKE( 19, "限制上传速度", SetUploadRate, "限制上传速度。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_btdownload_global_var + 13)\
    _MAKE( 20, "下载设置", DownloadSet, "进行一些基本的全局的参数设置。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_btdownload_global_var + 14)\
    _MAKE( 21, "取任务内容", GetTaskInfo, "获得任务内容，在指定的时间(一般是停止任务之前)执行本命令来取得任务信息,在“增加新任务”方法的参数中传进去，可以避免每次都效验文件。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 1, g_argumentInfo_btdownload_global_var + 15)\
    _MAKE( 22, "重新检查完整性", ReChcekWhole, "重新检查下载后文件的完整性。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 7, g_argumentInfo_btdownload_global_var + 16)\
    _MAKE( 23, "取下载号", GetDownloadNumber, "取当前下载对象的下载号。主要用于各种日志的第一个参数。", -1, _CMD_OS(__OS_WIN), SDT_INT, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE( 24, "是否停止", IsStopTask, "返回当前任务是否收到停止消息，主要用在回调函数中，如果任务已收到停止消息，就要马上从回调函数中返回。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)\
    _MAKE( 25, "测试代理服务器", TestProxyServer, "测试代理服务器是否能成功连上。注：调用此方法要先调用“下载设置”命令设置代理服务器相关内容。", 1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_btdownload_global_var + 23)\
    _MAKE( 26, "添加服务器", AddTreckerServer, "手动添加服务器(Tracker)。注：有些BT的Tracker服务器有备用的，并不在发布文件中，所以要有此方法。", -1, _CMD_OS(__OS_WIN), SDT_BOOL, 0, LVL_SIMPLE, 0, 0, 2, g_argumentInfo_btdownload_global_var + 25)\
    _MAKE( 27, "拷贝构造函数", copy, NULL, -1, _CMD_OS(__OS_WIN) | CT_IS_HIDED | CT_IS_OBJ_COPY_CMD, _SDT_NULL, 0, LVL_SIMPLE, 0, 0, 0, g_argumentInfo_btdownload_global_var + 0)

