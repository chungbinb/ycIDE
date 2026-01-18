#if !defined(__E_STATIC_LIB)
#include "include_sock_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_sock_global_var[] = 
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

    /*000*/ {"端口", "服务器的端口", 0, 0, SDT_INT, 0, NULL},

    /*001*/ {"等待时间", "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）", 0, 0, SDT_INT, 0, NULL},

    /*002*/ {"客户端句柄", "通过“网络服务器.监听()”得到的客户端句柄", 0, 0, SDT_INT, 0, NULL},
    /*003*/ {"等待时间", "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）", 0, 0, SDT_INT, 0, NULL},
    /*004*/ {"是否成功", "如果提供本参数，在接收数据成功的情况下，其值将被改写为真；在接收数据失败的情况下（如尚未连接或连接已断开或其它未知原因），其值将被改写为假。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*005*/ {"客户端句柄", "通过“网络服务器.监听()”得到的客户端句柄", 0, 0, SDT_INT, 0, NULL},
    /*006*/ {"数据", "准备发送的数据", 0, 0, _SDT_ALL, 0, NULL},
    /*007*/ {"等待时间", "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）", 0, 0, SDT_INT, 0, NULL},

    /*008*/ {"IP地址", "待连接服务器的IP地址", 0, 0, SDT_TEXT, 0, NULL},
    /*009*/ {"端口", "待连接服务器的端口", 0, 0, SDT_INT, 0, NULL},

    /*010*/ {"等待时间", "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）", 0, 0, SDT_INT, 0, NULL},
    /*011*/ {"是否成功", "如果提供本参数，在接收数据成功的情况下，其值将被改写为真；在接收数据失败的情况下（如尚未连接或连接已断开或其它未知原因），其值将被改写为假。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*012*/ {"数据", "准备发送的数据", 0, 0, _SDT_ALL, 0, NULL},
    /*013*/ {"等待时间", "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）", 0, 0, SDT_INT, 0, NULL},

    /*014*/ {"端口", "数据报服务器的端口", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},

    /*015*/ {"等待时间", "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）", 0, 0, SDT_INT, 0, NULL},
    /*016*/ {"对方信息", "获得对方的IP及端口信息", 0, 0, MAKELONG(0x04, 0), 0, AS_RECEIVE_VAR},
    /*017*/ {"是否成功", "如果提供本参数，在接收数据成功的情况下，其值将被改写为真；在接收数据失败的情况下，其值将被改写为假。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*018*/ {"对方信息", "接收方IP及端口信息", 0, 0, MAKELONG(0x04, 0), 0, NULL},
    /*019*/ {"数据", "准备发送的数据", 0, 0, _SDT_ALL, 0, NULL},
    /*020*/ {"等待时间", "等待连接的时间范围 0 无限等待，-1 不等待 其它 具体等待时间（毫秒）", 0, 0, SDT_INT, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_sock_global_var) / sizeof(g_argumentInfo_sock_global_var[0]);
#endif


#define SOCK_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_sock_global_var[] = 
{
    SOCK_DEF(SOCK_DEF_CMDINFO)
};

int g_cmdInfo_sock_global_var_count = sizeof(g_cmdInfo_sock_global_var) / sizeof(g_cmdInfo_sock_global_var[0]);

#endif

