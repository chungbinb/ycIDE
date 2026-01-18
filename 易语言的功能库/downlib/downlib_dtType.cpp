#ifndef __E_STATIC_LIB
#include "include_downlib_header.h"

// 下载对象  下的方法索引
static INT s_dtCmdIndexdownlib_Download_static_var_00[] = 
{
    19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 
    29, 30, 31, 32, 33, 39, 
};
static int s_dtCmdIndexdownlib_Download_static_var_count_00 = sizeof(s_dtCmdIndexdownlib_Download_static_var_00) / sizeof(s_dtCmdIndexdownlib_Download_static_var_00[0]);

// FTP上传对象  下的方法索引
static INT s_dtCmdIndexdownlib_FtpUpload_static_var_01[] = 
{
    40, 41, 42, 43, 44, 45, 46, 47, 
};
static int s_dtCmdIndexdownlib_FtpUpload_static_var_count_01 = sizeof(s_dtCmdIndexdownlib_FtpUpload_static_var_01) / sizeof(s_dtCmdIndexdownlib_FtpUpload_static_var_01[0]);

// FTP辅助对象  下的方法索引
static INT s_dtCmdIndexdownlib_FtpAssistant_static_var_02[] = 
{
    54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 
    64, 65, 
};
static int s_dtCmdIndexdownlib_FtpAssistant_static_var_count_02 = sizeof(s_dtCmdIndexdownlib_FtpAssistant_static_var_02) / sizeof(s_dtCmdIndexdownlib_FtpAssistant_static_var_02[0]);

// 下载对象  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventdownlib_Download_static_var_00[] = 
{
    /*000*/ {SDT_INT, 0, "文件句柄", "DownFileHandle", "下载文件句柄", LES_HIDED, (INT)0x00000000},

};
static int s_objEventdownlib_Download_static_var_count_00 = sizeof(s_objEventdownlib_Download_static_var_00) / sizeof(s_objEventdownlib_Download_static_var_00[0]);

// FTP上传对象  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventdownlib_FtpUpload_static_var_01[] = 
{
    /*000*/ {SDT_INT, 0, "文件句柄", "DownFileHandle", "上传文件句柄", LES_HIDED, (INT)0x00000000},

};
static int s_objEventdownlib_FtpUpload_static_var_count_01 = sizeof(s_objEventdownlib_FtpUpload_static_var_01) / sizeof(s_objEventdownlib_FtpUpload_static_var_01[0]);

// FTP辅助对象  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventdownlib_FtpAssistant_static_var_02[] = 
{
    /*000*/ {SDT_INT, 0, "文件句柄", "DownFileHandle", "上传文件句柄", LES_HIDED, (INT)0x00000000},

};
static int s_objEventdownlib_FtpAssistant_static_var_count_02 = sizeof(s_objEventdownlib_FtpAssistant_static_var_02) / sizeof(s_objEventdownlib_FtpAssistant_static_var_02[0]);

// 任务参数  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventdownlib_TaskParam_static_var_03[] = 
{
    /*000*/ {SDT_INT, 0, "类型", "Type", "指定任务类型 可以为以下常量 #下载 #上传 #覆盖 #续传 #改名 #跳过 #被动选择", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*001*/ {SDT_TEXT, 0, "文件地址", "Url", "本参数指定欲读取文件在互联网上的地址，即（URL）", LES_HAS_DEFAULT_VALUE, (INT)0x00926628},
    /*002*/ {SDT_TEXT, 0, "本地文件路径", "LocalFilePath", "本参数指定被下载或待上传文件的本地保存位置，当作为下载文件保存路径使用时。如果没有给定文件名，那必须是一个以“\\”结尾的文件路径。系统会根据“文件地址”指定的的文件名作为本地保存的文件名，系统会处理重定向情况。当收到“下载上传开始”消息后可以通过“取本地文件路径”获得包含文件名的文件路径", LES_HAS_DEFAULT_VALUE, (INT)0x00926628},
    /*003*/ {SDT_TEXT, 0, "用户名", "UserName", "需要身份验证时使用的用户名,当前只用做FTP登录时使用", LES_HAS_DEFAULT_VALUE, (INT)0x0091E02C},
    /*004*/ {SDT_TEXT, 0, "用户密码", "PassWord", "需要身份验证时使用的密码,当前只用做FTP登录时使用", LES_HAS_DEFAULT_VALUE, (INT)0x0091E018},
    /*005*/ {SDT_INT, 0, "默认线程数", "InitThreadCount", "任务启动时默认开启的线程数,默认为1", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*006*/ {SDT_INT, 0, "下载速度", "Rate", "本参数用来限制一个任务的下载速度，如果为0则不做限制。默认为0", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*007*/ {SDT_INT, 0, "连接超时", "ConnectionTimeOut", "连接服务器的超时时间，如果过了这个时间服务器还没有回应则返回，如果用户设置了重试次数，那么会进行指定次数的重试。本参数为毫秒，默认为20000", LES_HAS_DEFAULT_VALUE, (INT)0x00004E20},
    /*008*/ {SDT_INT, 0, "发送超时", "SendDataTimeOut", "发送数据的超时时间，如果用户设置了重试次数，那么会进行指定次数的重试。单位为毫秒，默认为20000", LES_HAS_DEFAULT_VALUE, (INT)0x00004E20},
    /*009*/ {SDT_INT, 0, "接收超时", "RecvDataTimeOut", "接收数据的超时时间，如果用户设置了重试次数，那么会进行指定次数的重试。单位为毫秒，默认为20000", LES_HAS_DEFAULT_VALUE, (INT)0x00004E20},
    /*010*/ {SDT_INT, 0, "线程等待超时", "ThreadTimeOut", "结束线程时等待的时间，单位为毫秒，默认为1000", LES_HAS_DEFAULT_VALUE, (INT)0x000003E8},
    /*011*/ {SDT_INT, 0, "接收大小", "RecvBlockSize", "每次接收数据的块大小，通过调节大小可以加快下载速度，但过大会导致意外错误，请根据实际情况调节。本值不能大于初始化库命令中设置的写入文件长度。单位为字节。默认为20480字节", LES_HAS_DEFAULT_VALUE, (INT)0x00005000},
    /*012*/ {SDT_INT, 0, "发送大小", "SendBlockSize", "每次发送数据的块大小，通过调节大小可以加快上传速度，但过大会导致意外错误，请根据实际情况调节。单位为字节。默认为1024字节", LES_HAS_DEFAULT_VALUE, (INT)0x00000400},
    /*013*/ {SDT_SUB_PTR, 0, "日志回调函数", "LogCallBack", "注册一个回调函数，系统产生日志时会调用此函数通知用户。如果为0则不能接收日志，默认为0。回调函数需要有5个参数：第一个参数，线程信息（整数型），可以通过“取线程信息”获得每个上传或下载对象的线程信息，与本参数比较从而确定发送本消息的对象；第二个参数，消息产生的时间（文本型）；第三个参数，消息的正文（文本型）；第四个参数，消息类型（整数型），参见“#传送常量.*”；第五个参数，对象信息（整数型），可以通过“取对象信息()”获得下载或上传对象的信息与本参数比较，从而确定本消息属于哪个对象", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*014*/ {SDT_SUB_PTR, 0, "交互回调函数", "MutualCallBack", "注册一个回调函数，系统产生需要交互的消息时调用此函数，具体消息请参见传送常量中设置的消息常量。如果为0则不接收，默认为0。回调函数需要有5个参数，一个整数型的返回值。返回值根据消息类型的不同而不同，如果没有说明，则返回值没有意义。参见具体的消息。第一个参数，消息类型（整数型），参见“#传送常量.*”；第二个参数，线程信息（整数型），可以通过”取线程信息“获得每个上传或下载对象的线程信息，与本参数比较从而确定发送本消息的对象，此参数值可能为0，表示不与特定线程有关；第三个参数（整数型）根据消息类型的不同而不同，参见具体的消息；第四个参数（整数型）根据消息类型的不同而不同，参见具体的消息；第五个参数，对象信息（整数型），可以通过“取对象信息()”获得下载或上传对象的信息与本参数比较，从而确定本消息属于哪个对象", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*015*/ {SDT_TEXT, 0, "备用文件名", "ReFileName", "当类型为“改名“时，本参数为上传时使用的新文件名，默认为空", LES_HAS_DEFAULT_VALUE, (INT)0x00926628},
    /*016*/ {SDT_INT, 0, "传送类型", "CarryType", "FTP上传或下载时用来区分传送类型。1 为二进制，2 为文本 3 为根据“类型区分串”选择", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*017*/ {SDT_TEXT, 0, "类型区分串", "CarryTypeStr", "FTP上传或下载时用来区分传送类型的字符串。本字符串由多个通过“|”分割的扩展名组成，如果待下载或上传文件的扩展名被包含在本字符串那么上传的类型为二进制模式否则为ASCII码模式,最大长度为512个字节例如：“.exe|.rar|.zip”.遇到带有上述扩展名的文件则起用二进制模式传送注意：本参数只在“传送类型”为3时有效", LES_HAS_DEFAULT_VALUE, (INT)0x00922860},
    /*018*/ {SDT_TEXT, 0, "代理服务器", "ProxyServer", "目前只支持HTTP GET方式的代理服务器, 格式为 IP地址:端口号 如: 192.168.0.1:80", NULL, (INT)0x00000000},
    /*019*/ {SDT_TEXT, 0, "引用", "Referer", "用户可以根据实际情况填写，如果不填系统会自动填充", NULL, (INT)0x00000000},

};
static int s_objEventdownlib_TaskParam_static_var_count_03 = sizeof(s_objEventdownlib_TaskParam_static_var_03) / sizeof(s_objEventdownlib_TaskParam_static_var_03[0]);

// 传送常量  下的枚举常量成员数组
static LIB_DATA_TYPE_ELEMENT s_objEventdownlib_DownConst_static_var_04[] = 
{
    /*000*/ {SDT_INT, 0, "下载", NULL, "下载文件与上传互斥", LES_HAS_DEFAULT_VALUE, (INT)0x00001000},
    /*001*/ {SDT_INT, 0, "上传", NULL, "上传文件与下载互斥", LES_HAS_DEFAULT_VALUE, (INT)0x00002000},
    /*002*/ {SDT_INT, 0, "覆盖", NULL, "覆盖已存在的同名文件。如果选择本模式上传过程中如果遇到同名文件不向用户询问，直接按此模式所指示的方式处理，与，被动选择，续传，改名，跳过互斥", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*003*/ {SDT_INT, 0, "续传", NULL, "如果已存在的同名文件小于待上传文件则根据断点位置继续上传。如果选择本模式上传过程中如果遇到同名文件不向用户询问，直接按此模式所指示的方式处理，与被动选择，覆盖，改名，跳过互斥", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*004*/ {SDT_INT, 0, "改名", NULL, "发现已存在的同名文件时使用FTP备用文件名保存，如果选择本模式上传过程中如果遇到同名文件不向用户询问，直接按此模式所指示的方式处理，与被动选择，续传，覆盖，跳过互斥", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*005*/ {SDT_INT, 0, "跳过", NULL, "发现已存在的同名文件则跳过，不做任何操作.如果选择本模式上传过程中如果遇到同名文件不向用户询问，直接按此模式所指示的方式处理，与被动选择，续传，改名，覆盖互斥", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*006*/ {SDT_INT, 0, "被动选择", NULL, "当发现已存在的同名文件时系统将调用“交互回调函数”并发送“发现已存在文件消息”通知用户，用户可根据实际情况返回#跳过，#改名，#覆盖，#续传，与跳过，续传，改名，覆盖互斥", LES_HAS_DEFAULT_VALUE, (INT)0x00000010},
    /*007*/ {SDT_INT, 0, "获得文件大小", NULL, "系统获得将要下载文件的大小时发送本消息通知用户", NULL, (INT)0x00000001},
    /*008*/ {SDT_INT, 0, "发现存在文件", NULL, "只有FTP上传对象有效，当发现已存在的同名文件时系统发送本消息通知用户。回调函数的参数3为已存在文件的大小，回调函数的参数4未使用。注意:只有设置为“被动选择”时才会收到本消息。本消息有返回值,可以为以下常量 #跳过，#改名，#覆盖，#续传", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*009*/ {SDT_INT, 0, "下载上传开始", NULL, "开始下载或上传时发送本消息通知用户。回调函数的参数3为待下载或待上传的文件大小，回调函数的参数4未使用。", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*010*/ {SDT_INT, 0, "下载上传结束", NULL, "下载、上传完毕，或用户结束下载、上传，系统发送本消息。回调函数的参数3为任务传送的总长度，回调函数的参数4未使用。", LES_HAS_DEFAULT_VALUE, (INT)0x00000004},
    /*011*/ {SDT_INT, 0, "新下载线程启动", NULL, "执行“增加线程”系统进行相关处理后发送本消息通知用户", LES_HAS_DEFAULT_VALUE, (INT)0x00000005},
    /*012*/ {SDT_INT, 0, "下载线程继续下载", NULL, "回调函数的参数3为本线程开始下载位置，回调函数的参数4为本线程结束下载位置。当下载线程完成指定下载任务后会查找是否有其它任务需要下载。比如，被中断的线程没有下载完毕的任务。如果发现存在这样的任务则发送本消息通知用户", LES_HAS_DEFAULT_VALUE, (INT)0x00000007},
    /*013*/ {SDT_INT, 0, "线程正常结束", NULL, "回调函数的参数3为结束位置，回调函数的参数4未使用。线程正常结束后系统进行结束处理后发送本消息通知用户", LES_HAS_DEFAULT_VALUE, (INT)0x00000008},
    /*014*/ {SDT_INT, 0, "线程异常结束", NULL, "回调函数的参数3为结束位置，回调函数的参数4未使用。线程异常结束后系统进行结束处理后发送本消息通知用户", LES_HAS_DEFAULT_VALUE, (INT)0x00000009},
    /*015*/ {SDT_INT, 0, "收到数据", NULL, "回调函数的参数3为接收到数据的大小，此值正常情况下等于接收大小；回调函数的参数4未使用。线程接收到指定大小的数据后发送本消息通知用户", LES_HAS_DEFAULT_VALUE, (INT)0x00000010},
    /*016*/ {SDT_INT, 0, "发送数据", NULL, "回调函数的参数3为发送数据的大小，此值正常情况下等于发送大小；回调函数的参数4未使用。线程发送指定大小的数据后发送本消息通知用户", LES_HAS_DEFAULT_VALUE, (INT)0x00000011},
    /*017*/ {SDT_INT, 0, "系统日志", NULL, "系统所产生日志", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*018*/ {SDT_INT, 0, "错误日志", NULL, "错误时所产生的日志", LES_HAS_DEFAULT_VALUE, (INT)0x00000003},
    /*019*/ {SDT_INT, 0, "接收日志", NULL, "从服务器接收到数据时发送本类型的日志消息通知用户", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*020*/ {SDT_INT, 0, "发送日志", NULL, "向服务器发送数据时发送本类型的日志消息通知用户", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*021*/ {SDT_INT, 0, "下载数据范围", NULL, "回调函数的参数3为本线程开始下载位置，回调函数的参数4为本线程结束下载位置。收到“新下载线程启动”消息后会收到本消息，本消息说明了新下载线程的下载范围。注意：本消息是在与服务器交互完毕后，准备下载数据前产生的，本消息可以用来确定正式开始接收数据", LES_HAS_DEFAULT_VALUE, (INT)0x00000006},
    /*022*/ {SDT_INT, 0, "系统处理", NULL, "系统自动改名，如已经存在同名文件“file.exe”，将自动更名为“file(1).exe”，如果发现“file(1).exe”也存在则括号中的数字加1，即“file(2).exe”，直到“file(99).exe”为止。", LES_HAS_DEFAULT_VALUE, (INT)0x00000000},
    /*023*/ {SDT_INT, 0, "结束下载", NULL, "当收到“存在同名文件”时使用。用来结束下载线程", LES_HAS_DEFAULT_VALUE, (INT)0x00000001},
    /*024*/ {SDT_INT, 0, "设置新名", NULL, "当收到“存在同名文件”时使用。如果返回本值，那么需要在回调函数返回前调用下载对象中的“设新文件名”", LES_HAS_DEFAULT_VALUE, (INT)0x00000002},
    /*025*/ {SDT_INT, 0, "存在同名文件", NULL, "当下载对象创建本地文件时，发现已存在的同名文件时系统发送本消息通知用户。回调函数的参数3为已存在文件的大小，回调函数的参数4为待下载文件的大小。本消息有返回值，可以为以下常量 #系统处理，#结束下载，#设置新名", LES_HAS_DEFAULT_VALUE, (INT)0x00000012},

};
static int s_objEventdownlib_DownConst_static_var_count_04 = sizeof(s_objEventdownlib_DownConst_static_var_04) / sizeof(s_objEventdownlib_DownConst_static_var_04[0]);

LIB_DATA_TYPE_INFO g_DataType_downlib_global_var[] = 
{
    //1=中文名字,2=英文名字,3=详细解释,4=命令数量,5=索引值,6=标志 LDT_
    //类型为窗口或菜单组件时有效 7=资源ID,8=事件数量,9=组件事件数组,10=属性数  11=属性数组 12=组件交互子程序
    //不为窗口、菜单组件或为枚举数据类型时才有效 13=成员数量,14=成员数据数组
    
/*000*/ { "下载对象", "Download", NULL,
            s_dtCmdIndexdownlib_Download_static_var_count_00, s_dtCmdIndexdownlib_Download_static_var_00, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventdownlib_Download_static_var_count_00, s_objEventdownlib_Download_static_var_00
        },
/*001*/ { "FTP上传对象", "FtpUpload", NULL,
            s_dtCmdIndexdownlib_FtpUpload_static_var_count_01, s_dtCmdIndexdownlib_FtpUpload_static_var_01, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventdownlib_FtpUpload_static_var_count_01, s_objEventdownlib_FtpUpload_static_var_01
        },
/*002*/ { "FTP辅助对象", "FtpAssistant", NULL,
            s_dtCmdIndexdownlib_FtpAssistant_static_var_count_02, s_dtCmdIndexdownlib_FtpAssistant_static_var_02, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventdownlib_FtpAssistant_static_var_count_02, s_objEventdownlib_FtpAssistant_static_var_02
        },
/*003*/ { "任务参数", "TaskParam", NULL,
            NULL, NULL, _DT_OS(__OS_WIN),
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventdownlib_TaskParam_static_var_count_03, s_objEventdownlib_TaskParam_static_var_03
        },
/*004*/ { "传送常量", "DownConst", "本数据类型包括了下载及FTP上传对象用到的常量",
            NULL, NULL, _DT_OS(__OS_WIN) | LDT_ENUM,
            0, NULL, NULL,
            NULL, NULL,
            NULL,
            s_objEventdownlib_DownConst_static_var_count_04, s_objEventdownlib_DownConst_static_var_04
        },
};
int g_DataType_downlib_global_var_count = sizeof(g_DataType_downlib_global_var) / sizeof(g_DataType_downlib_global_var[0]);

#endif

