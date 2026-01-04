#if !defined(__E_STATIC_LIB)
#include "include_btdownload_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_btdownload_global_var[] = 
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



    /*000*/ {"任务信息", "本参数表示下载任务的相关信息。", 0, 0, MAKELONG(0x02, 0), 0, NULL},

    /*001*/ {"任务内容", "如果提供本参数，则更新当前任务内容。如果被省略，保持当前任务内容不变。", 0, 0, SDT_BIN, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*002*/ {"地址", "本参数表示要连接的IP地址，如果本参数为空则自动选择。", 0, 0, SDT_TEXT, 9692712, AS_HAS_DEFAULT_VALUE},
    /*003*/ {"端口号", "本参数表示端口号。", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},

    /*004*/ {"发布文件名", "本参数表示指定发布文件名(Torrent)。", 0, 0, SDT_TEXT, 0, NULL},

    /*005*/ {"文件类型", "本参数表示要发布文件的类型，1表示目录，2表示文件。", 0, 0, SDT_INT, 2, AS_HAS_DEFAULT_VALUE},
    /*006*/ {"名称", "本参数表示目录名或文件名。", 0, 0, SDT_TEXT, 0, NULL},
    /*007*/ {"服务器地址", "本参数表示Tracker服务器的地址。多个服务器之间用“;”分隔。", 0, 0, SDT_TEXT, 0, NULL},
    /*008*/ {"发布路径", "本参数表示生成的发布文件的路径。", 0, 0, SDT_TEXT, 0, NULL},
    /*009*/ {"块大小", "本参数表示每一块的大小。值可以为“块大小”数据类型中的常量值。", 0, 0, SDT_INT, 0, NULL},
    /*010*/ {"注释", "本参数表示关于此发布文件的注释。", 0, 0, SDT_TEXT, 9692712, AS_HAS_DEFAULT_VALUE},
    /*011*/ {"创建者", "如果被省略，默认为“ebit”。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*012*/ {"下载速度", "本参数表示要限制的速度，单位为字节数/每秒。如 1024/s,表示1k/s。", 0, 0, SDT_INT, 0, NULL},

    /*013*/ {"上传速度", "本参数表示要限制的速度，单位为字节数/每秒。如 1024/s,表示1k/s。", 0, 0, SDT_INT, 0, NULL},

    /*014*/ {"下载设置信息", "本参数表示下载的参数信息。", 0, 0, MAKELONG(0x07, 0), 0, NULL},

    /*015*/ {"任务内容", "本参数表示要保存的任务内容。", 0, 0, SDT_BIN, 0, AS_RECEIVE_VAR},

    /*016*/ {"发布文件名", "本参数表示要检查的发布文件名(torrent)。", 0, 0, SDT_TEXT, 0, NULL},
    /*017*/ {"本地文件路径", "本参数表示被下载文件的本地保存路径。", 0, 0, SDT_TEXT, 0, NULL},
    /*018*/ {"本地文件名", "本参数表示被下载文件的本地文件名，如本参数为空则用发布文件中默认的文件名。", 0, 0, SDT_TEXT, 9692712, AS_HAS_DEFAULT_VALUE},
    /*019*/ {"百分比", "本参数传回下载了百分之几。", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},
    /*020*/ {"字节数", "本参数传回已写入硬盘的字节数。", 0, 0, SDT_INT64, 0, AS_RECEIVE_VAR},
    /*021*/ {"任务内容", "本参数传回任务内容，可用作传入“增加新任务”方法的参数中，以避免效验文件。", 0, 0, SDT_BIN, 0, AS_RECEIVE_VAR},
    /*022*/ {"日志", "本参数表示检查进度的日志回调函数，详见“其它日志”数据类型的“检查完整性中”成员。", 0, 0, SDT_SUB_PTR, 0, AS_HAS_DEFAULT_VALUE},

    /*023*/ {"地址", "本参数表示要连接的IP地址，如www.dywt.com.cn。", 0, 0, SDT_TEXT, 0, NULL},
    /*024*/ {"端口", "本参数表示要连接的端口。", 0, 0, SDT_INT, 80, AS_HAS_DEFAULT_VALUE},

    /*025*/ {"地址", "本参数表示备用服务器(Tracker)的IP地址。", 0, 0, SDT_TEXT, 0, NULL},
    /*026*/ {"端口", "本参数表示要连接的端口。", 0, 0, SDT_INT, 6969, AS_HAS_DEFAULT_VALUE},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_btdownload_global_var) / sizeof(g_argumentInfo_btdownload_global_var[0]);
#endif


#define BTDOWNLOAD_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_btdownload_global_var[] = 
{
    BTDOWNLOAD_DEF(BTDOWNLOAD_DEF_CMDINFO)
};

int g_cmdInfo_btdownload_global_var_count = sizeof(g_cmdInfo_btdownload_global_var) / sizeof(g_cmdInfo_btdownload_global_var[0]);

#endif

