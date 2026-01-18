#if !defined(__E_STATIC_LIB)
#include "include_einterprocess_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_einterprocess_global_var[] = 
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

    /*000*/ {"名称", "欲创建命名管道的名称", 0, 0, SDT_TEXT, 0, NULL},

    /*001*/ {"命名管道句柄", "已经创建的命名管道句柄", 0, 0, SDT_INT, 0, NULL},

    /*002*/ {"命名管道名称", "欲连接命名管道的名称", 0, 0, SDT_TEXT, 0, NULL},
    /*003*/ {"超时时间", "连接命名管道时的等待时间，-1为无限等待，其他为具体的等待时间，单位是毫秒", 0, 0, SDT_INT, 0, NULL},

    /*004*/ {"命名管道句柄", "欲读取的命名管道句柄", 0, 0, SDT_INT, 0, NULL},
    /*005*/ {"数据", "从命名管道中读取出的数据", 0, 0, SDT_BIN, 0, AS_RECEIVE_VAR},

    /*006*/ {"命名管道句柄", "欲写入的命名管道句柄", 0, 0, SDT_INT, 0, NULL},
    /*007*/ {"数据", "欲向命名管道中写入的数据", 0, 0, SDT_BIN, 0, NULL},

    /*008*/ {"命名管道句柄", "已经连接的命名管道句柄", 0, 0, SDT_INT, 0, NULL},

    /*009*/ {"邮槽名称", "欲在服务器上创建的邮槽名称，注意在Win9x下使用，系统最大允许名称的长度为8位，多余的会被抛弃", 0, 0, SDT_TEXT, 0, NULL},

    /*010*/ {"数据", "从向邮槽中读出的数据", 0, 0, SDT_BIN, 0, AS_RECEIVE_VAR},

    /*011*/ {"服务器名称", "欲连接的邮槽服务器名称，也可以是局域网的域名，这时表示客户机要连接这个局域网域内的所有计算机上名称为参数2的邮槽。该参数也可以是“.”，表示连接本机上的名称为参数2的邮槽。该参数也可以是“*”，表示连接系统主域内的所有计算机上的名称为参数2的邮槽", 0, 0, SDT_TEXT, 0, NULL},
    /*012*/ {"邮槽名称", "欲连接服务器上邮槽的名称", 0, 0, SDT_TEXT, 0, NULL},

    /*013*/ {"数据", "欲向邮槽中写入的数据，数据长度必须小于424字节，否则出错", 0, 0, SDT_BIN, 0, NULL},

    /*014*/ {"文件名称", "欲创建内存映射文件的文件全路径名称", 0, 0, SDT_TEXT, 0, NULL},
    /*015*/ {"是否创建", "是打开一个已经存在的文件还是创建一个新文件。真代表创建一个新文件，假代表打开一个已经存在的文件", 0, 0, SDT_BOOL, 0, NULL},
    /*016*/ {"文件大小", "欲创建内存映射文件的文件大小。如果是打开一个已经存在的文件，可以指定该值为0，表示创建的内存映射文件大小和文件本身大小是一致的", 0, 0, SDT_INT64, 0, NULL},
    /*017*/ {"名称", "创建内存映射文件的名称，该参数可以为空，表示创建一个没有名称的内存映射文件。其他进程可以通过该名称打开内存映射文件", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*018*/ {"名称", "其他进程创建的内存映射文件名称", 0, 0, SDT_TEXT, 0, NULL},

    /*019*/ {"起始位置", "映射到内存的文件起始位置。注意该值一定要是系统分配内存的分配粒度。windows的分配粒度是64k，所以该值一定要是64k的倍数，否则会执行出错。起始位置的值从0开始", 0, 0, SDT_INT64, 0, NULL},
    /*020*/ {"大小", "映射到内存的字节数量。该值等于0代表从起始位置开始文件其余部分全部映射到内存", 0, 0, SDT_INT, 0, NULL},

    /*021*/ {"起始位置", "从已经映射到内存的内存映射文件中读出的数据的起始位置。起始位置的值从0开始", 0, 0, SDT_INT64, 0, NULL},
    /*022*/ {"长度", "从已经映射到内存的内存映射文件中读出的数据的长度", 0, 0, SDT_INT, 0, NULL},
    /*023*/ {"数据", "从内存映射文件中读出的数据，作为方法的返回值", 0, 0, SDT_BIN, 0, AS_RECEIVE_VAR},

    /*024*/ {"起始位置", "向内存映射文件中写入的数据的起始位置。起始位置的值从0开始", 0, 0, SDT_INT64, 0, NULL},
    /*025*/ {"数据", "欲写入的数据。注意写数据时，数据的长度不能超过映射文件的边界", 0, 0, SDT_BIN, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_einterprocess_global_var) / sizeof(g_argumentInfo_einterprocess_global_var[0]);
#endif


#define EINTERPROCESS_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_einterprocess_global_var[] = 
{
    EINTERPROCESS_DEF(EINTERPROCESS_DEF_CMDINFO)
};

int g_cmdInfo_einterprocess_global_var_count = sizeof(g_cmdInfo_einterprocess_global_var) / sizeof(g_cmdInfo_einterprocess_global_var[0]);

#endif

