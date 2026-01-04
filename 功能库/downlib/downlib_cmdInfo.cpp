#if !defined(__E_STATIC_LIB)
#include "include_downlib_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_downlib_global_var[] = 
{
    // 1=参数名称, 2=参数详细解释, 3=指定图像索引,从1开始,0表示无, 4=图像数目(用作动画)
    // 5=参数类型, 6=参数默认值,   7=参数标志 AS_ 开头常量
    // AS_HAS_DEFAULT_VALUE         有默认值,倒数第二个参数是默认值
    // AS_DEFAULT_VALUE_IS_EMPTY    默认值为空,有可空标志
    // AS_RECEIVE_VAR               只能传递变量,相当于传引用,传递过来的肯定不是数组
    // AS_RECEIVE_VAR_ARRAY         传递过来的肯定是数组变量引用
    // AS_RECEIVE_VAR_OR_ARRAY      传递变量或者数组变量引用
    // AS_RECEIVE_ARRAY_DATA        传递数组
    // AS_RECEIVE_ALL_TYPE_DATA     传递数组/非数组
    // AS_RECEIVE_VAR_OR_OTHER      可以传递 变量/数据/返回值数据

    /*000*/ {"写入文件大小", "本参数指定每次写入文件的指定大小，默认为20480, 注意:如果待下载文件的总长度小于此值，那么将不能使用多个线程下载", 0, 0, SDT_INT, 20480, AS_HAS_DEFAULT_VALUE},
    /*001*/ {"重连时间", "出错时连接服务器间隔时间，默认为0", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
    /*002*/ {"重连次数", "出错时重复连接服务器次数，默认为0", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},

    /*003*/ {"重试时间", "尝试重新连接的时间", 0, 0, SDT_INT, 5000, AS_HAS_DEFAULT_VALUE},

    /*004*/ {"配置信息", "配置任务相关信息", 0, 0, MAKELONG(0x04, 0), 0, NULL},

    /*005*/ {"任务数据", "本参数是用于保存下载任务的相关数据的", 0, 0, SDT_BIN, 0, AS_RECEIVE_VAR},
    /*006*/ {"执行方式", "本参数无意义", 0, 0, SDT_INT, 2, AS_HAS_DEFAULT_VALUE},

    /*007*/ {"配置信息", "配置任务相关信息", 0, 0, MAKELONG(0x04, 0), 0, NULL},
    /*008*/ {"任务数据", "本参数是通过“停止任务”命令所获得的任务数据", 0, 0, SDT_BIN, 0, NULL},

    /*009*/ {"速度", "限制后的速度，单位为 字节/秒", 0, 0, SDT_INT, 0, NULL},

    /*010*/ {"线程信息数组", "本参数是用于保存线程信息数组", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR_ARRAY},

    /*011*/ {"任务数据", "本参数是通过“停止任务”命令所获得的任务数据", 0, 0, SDT_BIN, 0, NULL},
    /*012*/ {"数据类型", "需要获得的任务信息的类型。如果”类型参数“作为指定具体断点序号使用，那么它从1开始不大于断点个数,1 待下载文件总长度，2 已下载总长度,3 断点个数,4 指定断点开始下载位置,类型参数为序号,5 指定断点待下载长度,类型参数为序号,6 指定断点已下载长度,类型参数为序号", 0, 0, SDT_INT, 0, NULL},
    /*013*/ {"类型参数", "根据”数据类型“参数的不同而不同。当需要查询的信息类型没有指定本参数的意义时，本参数无意义", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},

    /*014*/ {"新文件名", "新的本地文件名", 0, 0, SDT_TEXT, 0, NULL},

    /*015*/ {"速度", "指定上传速度", 0, 0, SDT_INT, 0, NULL},

    /*016*/ {"新文件名", "上传到服务器的文件的新名", 0, 0, SDT_TEXT, 9573972, AS_HAS_DEFAULT_VALUE},

    /*017*/ {"FTP服务器地址", "本参数指定FTP服务器在互联网上的地址，格式为 ftp://域名或ip/", 0, 0, SDT_TEXT, 9573876, AS_HAS_DEFAULT_VALUE},
    /*018*/ {"用户名", "本参数提供用作通过FTP服务器用户验证的用户名称，如果被省略，默认值为匿名用户名“anonymous”", 0, 0, SDT_TEXT, 9560108, AS_HAS_DEFAULT_VALUE},
    /*019*/ {"密码", "本参数提供用作通过FTP服务器用户验证的密码，如果被省略，默认值为空文本", 0, 0, SDT_TEXT, 9594408, AS_HAS_DEFAULT_VALUE},
    /*020*/ {"端口", "本参数提供欲连接到FTP服务器上的端口号，如果被省略，默认值为标准端口 21", 0, 0, SDT_INT, 21, AS_HAS_DEFAULT_VALUE},
    /*021*/ {"日志回调函数", "注册一个回调函数，系统产生日志时会调用此函数通知用户,如果为0则不能接收日志，默认为0。回调函数需要有5个参数，第一个参数（整数型），线程信息，第二个参数（文本型），消息产生的时间。第三个参数（文本型），消息的正文。第四个参数（整数型），消息类型，参见”传送常量“第五个参数（整数型），对象信息，可以通过“取对象信息”获得下载或上传对象的信息与本参数比较，从而确定本消息属于哪个对象", 0, 0, SDT_SUB_PTR, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*022*/ {"超时", "登录过程中数据交互的超时时间,单位为毫秒，如果忽略则默认为2000", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*023*/ {"欲删除的FTP文件", "本参数指定在FTP服务器上欲删除文件的路径", 0, 0, SDT_TEXT, 0, NULL},

    /*024*/ {"欲改名的FTP文件", "本参数指定FTP服务器上欲改名文件的路径", 0, 0, SDT_TEXT, 0, NULL},
    /*025*/ {"新文件名称", "本参数指定欲改为的文件名称", 0, 0, SDT_TEXT, 0, NULL},

    /*026*/ {"FTP目录路径", "FTP目录路径", 0, 0, SDT_TEXT, 0, NULL},

    /*027*/ {"欲列出的文件", "可以包含路径，可以使用多字符 (*) 和单字符 (?) 通配符来指定多重文件及目录", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*028*/ {"存放文件名的数组变量", "提供参数数据时只能提供变量数组。在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件和子目录名称。变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR_OR_ARRAY},
    /*029*/ {"存放文件属性的数组变量", "当前不被支持,留做以后扩展", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR_OR_ARRAY},
    /*030*/ {"存放文件尺寸的数组变量", "提供参数数据时只能提供变量数组。在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的尺寸，并与文件名数组成员一一对应", 0, 0, SDT_INT64, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR_OR_ARRAY},
    /*031*/ {"存放文件时间的数组变量", "提供参数数据时只能提供变量数组。在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的最后修改时间，并与文件名数组成员一一对应。变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目", 0, 0, SDT_DATE_TIME, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR_OR_ARRAY},

    /*032*/ {"旧文件路径", "旧文件路径", 0, 0, SDT_TEXT, 0, NULL},
    /*033*/ {"新文件路径", "新文件路径", 0, 0, SDT_TEXT, 0, NULL},

    /*034*/ {"FTP命令", "欲执行的FTP命令", 0, 0, SDT_TEXT, 0, NULL},
    /*035*/ {"返回命令数据", "执行FTP命令后服务器返回的命令文本，最大支持返回51200字节。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_downlib_global_var) / sizeof(g_argumentInfo_downlib_global_var[0]);
#endif


#define DOWNLIB_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_downlib_global_var[] = 
{
    DOWNLIB_DEF(DOWNLIB_DEF_CMDINFO)
};

int g_cmdInfo_downlib_global_var_count = sizeof(g_cmdInfo_downlib_global_var) / sizeof(g_cmdInfo_downlib_global_var[0]);

#endif

