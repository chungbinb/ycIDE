#ifndef __E_STATIC_LIB
#include "include_btdownload_header.h"

// BT下载  下的方法索引
static INT s_dtCmdIndexbtdownload_BT_download_static_var_00[] = 
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    18, 19, 21, 23, 24, 26, 27, 
};
static int s_dtCmdIndexbtdownload_BT_download_static_var_count_00 = sizeof(s_dtCmdIndexbtdownload_BT_download_static_var_00) / sizeof(s_dtCmdIndexbtdownload_BT_download_static_var_00[0]);

// 任务信息  下的方法索引
static INT s_dtCmdIndexbtdownload_task_info_static_var_01[] = 
{
    15, 16, 17, 
};
static int s_dtCmdIndexbtdownload_task_info_static_var_count_01 = sizeof(s_dtCmdIndexbtdownload_task_info_static_var_01) / sizeof(s_dtCmdIndexbtdownload_task_info_static_var_01[0]);

// 发布文件信息  下的方法索引
static INT s_dtCmdIndexbtdownload_TorrentFileInfo_static_var_04[] = 
{
    10, 11, 12, 
};
static int s_dtCmdIndexbtdownload_TorrentFileInfo_static_var_count_04 = sizeof(s_dtCmdIndexbtdownload_TorrentFileInfo_static_var_04) / sizeof(s_dtCmdIndexbtdownload_TorrentFileInfo_static_var_04[0]);

// BT下载  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventbtdownload_BT_download_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "BT对象句柄", NULL, NULL, LES_HIDED, (INT)0x00000000},

};
static int s_objEventbtdownload_BT_download_static_var_count_00 = sizeof(s_objEventbtdownload_BT_download_static_var_00) / sizeof(s_objEventbtdownload_BT_download_static_var_00[0]);

// 任务信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventbtdownload_task_info_static_var_01[] = 
{
    /*000*/ {SDT_TEXT, 0, "发布文件名", "TorrentFile", "本参数指定发布文件(torrent)的路径和文件名。", LES_HAS_DEFAULT_VALUE, (INT)0x0093E628},
    /*001*/ {SDT_TEXT, 0, "本地文件路径", "FilePath", "本参数指定被下载文件的本地保存路径。", LES_HAS_DEFAULT_VALUE, (INT)0x0093E628},
    /*002*/ {SDT_TEXT, 0, "本地文件名", "FileName", "本参数指定被下载文件的本地文件名，如本参数为空则用Torrent文件中默认的文件名。", LES_HAS_DEFAULT_VALUE, (INT)0x0093E628},
    /*003*/ {SDT_SUB_PTR, 0, "服务器日志", "Tracker log", "本参数指定与Tracker服务器通讯时的回调函数。本函数有六个参数，第一个类型为“整数型”，是调用此回调函数对象的下载号;第二个类型为“整数型”，是“服务器日志”数据类型中的常量;第三个类型为“文本型”根据第二个参数的不同意义不同，详细看“服务器日志”数据类型中的说明;第四个类型为“整数型”根据第二个参数的不同意义不同，详细看“服务器日志”数据类型中的说明;第五个类型为“整数型”根据第二个参数的不同意义不同，详细看“服务器日志”数据类型中的说明;第六个类型为“日期时间型”表示记录日志的时间。返回值为“逻辑型”。", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*004*/ {SDT_SUB_PTR, 0, "上传下载日志", "UdloadLog", "本参数指定用于上传下载日志的回调函数。本函数有五个参数，第一个类型为“整数型”，是调用此回调函数对象的下载号;第二个类型为“整数型”，是“上传下载日志”数据类型中的常量;第三个类型为“整数型”本参数表示连接序号，每一个连接都有唯一的序号。-1表示还没有选择序号;第四个类型为“整数型”根据第二个参数的不同意义不同，详细看“上传下载日志”数据类型中的说明;第五个类型为“文本型”根据第二个参数的不同意义不同，详细看“上传下载日志”数据类型中的说明;返回值为“逻辑型”。注：在使用本参数时，在易语言中对应的函数中如果要对本函数中作用域之外的变量改变,在函数入口和出口处分别加上“进入许可区”“退出许可区”命令，以确保不会发生多线程的竟争冲突。", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*005*/ {SDT_SUB_PTR, 0, "其它日志", "OtherLog", "本参数指定用于其它日志的回调函数。本函数有六个参数，第一个类型为“整数型”，是调用此回调函数对象的下载号;第二个类型为“整数型”，是“其它日志”数据类型中的常量;第三个类型为“整数型”根据第二个参数的不同意义不同，详细看“其它日志”数据类型中的说明;第四个类型为“整数型”根据第二个参数的不同意义不同，详细看“其它日志”数据类型中的说明;第五个类型为“文本型”根据第二个参数的不同意义不同，详细看“其它日志”数据类型中的说明;第六个类型为“文本型”根据第二个参数的不同意义不同，详细看“其它日志”数据类型中的说明;返回值为“逻辑型”。", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*006*/ {SDT_INT, 0, "主动连接数", "InitiateConnect", "本参数指定默认的主动连接数。", LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*007*/ {SDT_INT, 0, "最大连接数", "MaxConnect", "本参数指定的最大的连接数。", LES_HAS_DEFAULT_VALUE, (INT)0x0000000A},
    /*008*/ {SDT_BIN, 0, "任务内容", "TaskContent", "本参数指定“取任务内容”方法返回的字节集,可以避免重新效验文件。如果为空字节集或传入字节集错误就会效验文件。", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},

};
static int s_objEventbtdownload_task_info_static_var_count_01 = sizeof(s_objEventbtdownload_task_info_static_var_01) / sizeof(s_objEventbtdownload_task_info_static_var_01[0]);

// 服务器日志  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventbtdownload_ServerLog_static_var_02[] = 
{
    /*000*/ {SDT_INT, 0, "服务器地址", "ServerAddress", "得到一个服务器地址。参数三表示服务器地址，参数四表示此服务器的端口。参数五无意义。", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "连接服务器", "ConnectServer", "准备连接服务器。参数三表示服务器地址，参数四表示此服务器的端口。参数五无意义。以后的服务器操作都是针对这个服务器的，必要时可记录参数三和参数四的值，以备后用。", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "连接服务器失败", "ConnectServerFailed", "连接服务器失败。参数三表示服务器地址，参数四表示此服务器的端口。参数五无意义。", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*003*/ {SDT_INT, 0, "发送请求", "SendGet", "发送请求。参数三为请求内容，参数四和参数五无意义。", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*004*/ {SDT_INT, 0, "发送请求失败", "SendGetFailed", "发送请求失败。参数三为请求内容，参数四和参数五无意义。", LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*005*/ {SDT_INT, 0, "准备接收数据", "WillRecvData", "准备接收数据。参数三无意义，参数四无意义。参数五无意义。", LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*006*/ {SDT_INT, 0, "接收数据失败", "RecvDataFailed", "接收数据失败。参数三无意义，参数四无意义。参数五无意义。", LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*007*/ {SDT_INT, 0, "分析返回数据", "AnaReturnData", "分析返回数据。参数三是服务器返回数据的文本形式，参数四是服务器返回数据的内存地址，参数五是服务器返回数据的数据长度（单位是字节）。考虑到服务器所返回的数据不一定是纯文本，所以额外提供了参数四和参数五，可使用“指针到字节集”命令读取。此处约定的参数含义可能会在后续版本中进行改动。", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*008*/ {SDT_INT, 0, "分析返回数据失败", "AnareturnDataFailed", "分析返回数据失败。参数三表示失败原因，参数四和参数五无意义。", LES_HAS_DEFAULT_VALUE, (INT)0x00000009},
    /*009*/ {SDT_INT, 0, "取到远程客户端", "GetRemoteClient", "取到远程的客户端。参数四表示取到的远程客户端个数。参数三和参数五无意义。", LES_HAS_DEFAULT_VALUE, (INT)0x0000000A},
    /*010*/ {SDT_INT, 0, "设置重试时间", "SetTime", "设置下一次连接该服务器的时间。参数三表示要设置服务器的地址，参数四表示要设置服务器的端口，参数五表示重试时间（单位为毫秒）。", LES_HAS_DEFAULT_VALUE, (INT)0x0000000B},
    /*011*/ {SDT_INT, 0, "重定向", "HTTPRedirect", "在此服务器上发生重定向，后续操作将自动连接重定向后的服务器。参数三和参数四分别表示重定向后的服务器地址和端口。参数五无意义。", LES_HAS_DEFAULT_VALUE, (INT)0x0000000C},

};
static int s_objEventbtdownload_ServerLog_static_var_count_02 = sizeof(s_objEventbtdownload_ServerLog_static_var_02) / sizeof(s_objEventbtdownload_ServerLog_static_var_02[0]);

// 上传下载日志  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventbtdownload_UlLog_static_var_03[] = 
{
    /*000*/ {SDT_INT, 0, "选择地址失败", "ChoiceIPFailed", "主动连接选择IP地址失败，准备重试。", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "选择地址成功", "ChoiceIPSucceed", "主动连接选择IP地址成功。参数四表示端口号，参数五表示IP地址。", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "拒绝连接", "RefuseConnect", "超过最大连接数，拒绝连接。参数四表示端口号，参数五表示IP地址。", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*003*/ {SDT_INT, 0, "选择序号成功", "ChoiceNoSucceed", "选择序号成功。此操作之后序号就唯一的表示了本次连接。参数四表示端口号，参数五表示IP地址。", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*004*/ {SDT_INT, 0, "连接失败", "ConnectFailed", "连接失败。", LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*005*/ {SDT_INT, 0, "握手成功", "HandshakeSucceed", "握手成功。BT通信前必须要经过握手。", LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*006*/ {SDT_INT, 0, "完成百分比", "DownloadPercent", "对方已经下载完成的百分比。参数四表示下载了百分之几。", LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*007*/ {SDT_INT, 0, "准备上传", "ReadyUpload", "已经可以上传。", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*008*/ {SDT_INT, 0, "准备下载", "ReadyDownload", "已经可以下载。", LES_HAS_DEFAULT_VALUE, (INT)0x00000009},
    /*009*/ {SDT_INT, 0, "下载", "Download", "本次下载的字节数。参数四表示本次下载的字节数。", LES_HAS_DEFAULT_VALUE, (INT)0x0000000A},
    /*010*/ {SDT_INT, 0, "上传", "Upload", "本次上传的字节数。参数四表示本次上传的字节数。", LES_HAS_DEFAULT_VALUE, (INT)0x0000000B},
    /*011*/ {SDT_INT, 0, "连接完成", "Disconnect", "本次连接完成。", LES_HAS_DEFAULT_VALUE, (INT)0x0000000C},
    /*012*/ {SDT_INT, 0, "写入磁盘", "WriteDisk", "写入磁盘的实际字节数。参数四表示本次写入磁盘的字节数。", LES_HAS_DEFAULT_VALUE, (INT)0x0000000D},

};
static int s_objEventbtdownload_UlLog_static_var_count_03 = sizeof(s_objEventbtdownload_UlLog_static_var_03) / sizeof(s_objEventbtdownload_UlLog_static_var_03[0]);

// 发布文件信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventbtdownload_TorrentFileInfo_static_var_04[] = 
{
    /*000*/ {SDT_TEXT, 0, "服务器列表", "TrackServerList", "本参数表示服务器(Tracker)列表，多个服务器用“;”分隔开。", LES_HAS_DEFAULT_VALUE, (INT)0x0093E628},
    /*001*/ {SDT_TEXT, 0, "注释", "Comment", "本参数表示此文件的注释。", NULL, (INT)0x00000000},
    /*002*/ {SDT_TEXT, 0, "建立者", "CreateBy", "本参数表示此文件的建立者。", NULL, (INT)0x00000000},
    /*003*/ {SDT_TEXT, 0, "文件目录或名称", "FilesName", "本参数表示此Torrent包含的文件名称，如果是单文件则就是文件名，如果是多文件则为目录名。", NULL, (INT)0x00000000},
    /*004*/ {SDT_INT64, 0, "文件总长度", "FilesLength", "本参数表示此Torrent包含的文件总长度。", NULL, (INT)0x00000000},
    /*005*/ {SDT_INT, 0, "每块长度", "PieceLength", "本参数表示每一块的长度。", NULL, (INT)0x00000000},
    /*006*/ {SDT_INT, 0, "块数", "PieceSize", "本参数表示本文件一共有多少块。", NULL, (INT)0x00000000},
    /*007*/ {SDT_INT64, 0, "文件长度", "FileSize", "本参数表示如果是多文件时每一个文件的长度，数组下标与“文件名称”相对应。", NULL, (INT)0x00000000},
    /*008*/ {SDT_TEXT, 0, "文件名称", "FileName", "本参数表示如果是多文件时每一个文件的名称，数组下标与“文件长度”相对应。", NULL, (INT)0x00000000},
    /*009*/ {SDT_TEXT, 0, "效验码", "Sha1", "本参数表示此文件的效验码。", NULL, (INT)0x00000000},
    /*010*/ {SDT_DATE_TIME, 0, "建立时间", "CreateDate", "本参数表示此文件建立的时间。", NULL, (INT)0x00000000},

};
static int s_objEventbtdownload_TorrentFileInfo_static_var_count_04 = sizeof(s_objEventbtdownload_TorrentFileInfo_static_var_04) / sizeof(s_objEventbtdownload_TorrentFileInfo_static_var_04[0]);

// 其它日志  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventbtdownload_OtherLog_static_var_05[] = 
{
    /*000*/ {SDT_INT, 0, "检查完整性后", "CheckWhole", "检查后传回已下载了多少。参数三表示下载了百分之几，参数四表示已写入硬盘的千字节数(KB)。", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "远程客户端", "RemoteClient", "主动连接获得一个远程客户端。参数三表示端口，参数四无意义，参数五表示IP地址，参数六表示本客户端的唯一标识符（可能为空）。", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "监听端口", "ListenPort", "监听端口成功。参数三表示监听的端口。", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*003*/ {SDT_INT, 0, "检查完整性中", "CheckWholeIng", "检查完整性中,每检查完一片，就发送一次本日志。参数三表示检查完第几片，参数四表示一共有几片。", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*004*/ {SDT_INT, 0, "下载已全部停止", "DownloadStop", "表示下载已全部停止。参数三表示退出原因。0:正常退出。-1:解码发布文件失败。-2:建立或打开本地文件失败。-3:对本地文件进行分片失败。-4:准备上传失败。-5:与Tracker服务器通讯失败。", LES_HAS_DEFAULT_VALUE, (INT)0x00000005},

};
static int s_objEventbtdownload_OtherLog_static_var_count_05 = sizeof(s_objEventbtdownload_OtherLog_static_var_05) / sizeof(s_objEventbtdownload_OtherLog_static_var_05[0]);

// 下载设置信息  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventbtdownload_GlobleInfo_static_var_06[] = 
{
    /*000*/ {SDT_INT, 0, "每地址连接数", "PerAddressConnects", "本参数表示一个IP地址最多允许同时有几个连接。", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "开始监听端口", "StartListenPort", "本参数表示监听的端口开始值。", LES_HAS_DEFAULT_VALUE, (INT)0x00002329},
    /*002*/ {SDT_INT, 0, "结束监听端口", "EndListenPort", "本参数表示监听的端口结束值。", LES_HAS_DEFAULT_VALUE, (INT)0x0000238C},
    /*003*/ {SDT_INT, 0, "阻塞值", "ChokeValue", "本参数表示当上传比下载大多少时，就阻塞对方(不给对方上传)。单位为:千字节(KB)注：在下载完成后，本参数失效。", LES_HAS_DEFAULT_VALUE, (INT)0x00000800},
    /*004*/ {SDT_INT, 0, "连接超时", "ConnectTimeOut", "本参数表示在一条连接上多长时间没有发生数据交换就断开此连接。单位为毫秒。", LES_HAS_DEFAULT_VALUE, (INT)0x0000EA60},
    /*005*/ {SDT_INT, 0, "代理服务器类型", "ProxyType", "本参数表示代理服务器的类型0为不使用，1为HTTP1.1, 2为SOCKS5。", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*006*/ {SDT_TEXT, 0, "代理服务器地址", "ProxyAddress", "本参数表示代理服务器的IP地址。", NULL, (INT)0x00000000},
    /*007*/ {SDT_INT, 0, "代理服务器端口", "ProxyPort", "本参数表示代理服务器的端口。", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*008*/ {SDT_TEXT, 0, "用户名", "ProxyUser", "本参数表示代理服务器需要验证的用户名。", NULL, (INT)0x00000000},
    /*009*/ {SDT_TEXT, 0, "口令", "ProxyPassword", "本参数表示代理服务器需要验证的口令。", NULL, (INT)0x00000000},
    /*010*/ {SDT_INT, 0, "连接服务器超时", "ConnectServerTimeOut", "本参数表示连接Tracker服务器时的超时。单位为毫秒。注：本超时不能低于10秒。", LES_HAS_DEFAULT_VALUE, (INT)0x00007530},
    /*011*/ {SDT_INT, 0, "连接下载者超时", "ConnectPeerTimeOut", "本参数表示连接其它下载者时的超时。单位为毫秒。注：本超时不能低于20秒。", LES_HAS_DEFAULT_VALUE, (INT)0x00007530},

};
static int s_objEventbtdownload_GlobleInfo_static_var_count_06 = sizeof(s_objEventbtdownload_GlobleInfo_static_var_06) / sizeof(s_objEventbtdownload_GlobleInfo_static_var_06[0]);

// 块大小  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventbtdownload_PieceSize_static_var_07[] = 
{
    /*000*/ {SDT_INT, 0, "KB32", NULL, "表示每块大小为32KB。", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_INT, 0, "KB64", NULL, "表示每块大小为64KB。", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*002*/ {SDT_INT, 0, "KB128", NULL, "表示每块大小为128KB。", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*003*/ {SDT_INT, 0, "KB256", NULL, "表示每块大小为256KB。", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*004*/ {SDT_INT, 0, "KB512", NULL, "表示每块大小为512KB。", LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*005*/ {SDT_INT, 0, "KB1024", NULL, "表示每块大小为1024KB。", LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*006*/ {SDT_INT, 0, "KB2048", NULL, "表示每块大小为2048KB。", LES_HAS_DEFAULT_VALUE, (INT)0x00000007},

};
static int s_objEventbtdownload_PieceSize_static_var_count_07 = sizeof(s_objEventbtdownload_PieceSize_static_var_07) / sizeof(s_objEventbtdownload_PieceSize_static_var_07[0]);

LIB_DATA_TYPE_INFO g_DataType_btdownload_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "BT下载", "BT download", NULL,
            s_dtCmdIndexbtdownload_BT_download_static_var_count_00, s_dtCmdIndexbtdownload_BT_download_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventbtdownload_BT_download_static_var_count_00, s_objEventbtdownload_BT_download_static_var_00
        },
/*001*/ { "任务信息", "task info", "下载时要提供的信息",
            s_dtCmdIndexbtdownload_task_info_static_var_count_01, s_dtCmdIndexbtdownload_task_info_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventbtdownload_task_info_static_var_count_01, s_objEventbtdownload_task_info_static_var_01
        },
/*002*/ { "服务器日志", "ServerLog", "本数据类型包括了与Tracker服务器通信过程中写入日志的类型。",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventbtdownload_ServerLog_static_var_count_02, s_objEventbtdownload_ServerLog_static_var_02
        },
/*003*/ { "上传下载日志", "UlLog", "本数据类型包括了上传下载过程中写入日志的类型。",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventbtdownload_UlLog_static_var_count_03, s_objEventbtdownload_UlLog_static_var_03
        },
/*004*/ { "发布文件信息", "TorrentFileInfo", "本数据类型包括了发布文件(Torrent)中的信息。",
            s_dtCmdIndexbtdownload_TorrentFileInfo_static_var_count_04, s_dtCmdIndexbtdownload_TorrentFileInfo_static_var_04, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventbtdownload_TorrentFileInfo_static_var_count_04, s_objEventbtdownload_TorrentFileInfo_static_var_04
        },
/*005*/ { "其它日志", "OtherLog", "本数据类型包括了其它日志的类型。",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventbtdownload_OtherLog_static_var_count_05, s_objEventbtdownload_OtherLog_static_var_05
        },
/*006*/ { "下载设置信息", "GlobleInfo", "设置下载参数的信息",
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventbtdownload_GlobleInfo_static_var_count_06, s_objEventbtdownload_GlobleInfo_static_var_06
        },
/*007*/ { "块大小", "PieceSize", "本数据类型包括了关于块大小的常量。",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventbtdownload_PieceSize_static_var_count_07, s_objEventbtdownload_PieceSize_static_var_07
        },
};
int g_DataType_btdownload_global_var_count = sizeof(g_DataType_btdownload_global_var) / sizeof(g_DataType_btdownload_global_var[0]);

#endif

