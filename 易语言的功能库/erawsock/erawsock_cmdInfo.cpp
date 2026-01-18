#if !defined(__E_STATIC_LIB)
#include "include_erawsock_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_erawsock_global_var[] = 
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

    /*000*/ {"协议类型", "用来指明所要处理的协议类型。", 0, 0, SDT_INT, 0, NULL},

    /*001*/ {"IP报头", NULL, 0, 0, MAKELONG(0x03, 0), 0, NULL},

    /*002*/ {"TCP报头", NULL, 0, 0, MAKELONG(0x04, 0), 0, NULL},

    /*003*/ {"ICMP报头", NULL, 0, 0, MAKELONG(0x05, 0), 0, NULL},

    /*004*/ {"等待时间", "等待接收的时间范围。单位为毫秒。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*005*/ {"对方IP", "接收方的IP信息。", 0, 0, SDT_TEXT, 0, NULL},
    /*006*/ {"对方端口", "接收方的端口信息。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*007*/ {"等待时间", "等待发送的时间范围。单位为毫秒。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*008*/ {"数据", "准备发送的数据。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*009*/ {"数据", "待取得报头信息的数据。", 0, 0, SDT_BIN, 0, NULL},
    /*010*/ {"IP报头", "待取得的IP报头。", 0, 0, MAKELONG(0x03, 0), 0, NULL},

    /*011*/ {"数据", "待取得报头信息的数据。", 0, 0, SDT_BIN, 0, NULL},
    /*012*/ {"TCP报头", "待取得的TCP报头。", 0, 0, MAKELONG(0x04, 0), 0, NULL},

    /*013*/ {"数据", "待取得报头信息的数据。", 0, 0, SDT_BIN, 0, NULL},
    /*014*/ {"ICMP报头", "待取得的ICMP报头。", 0, 0, MAKELONG(0x05, 0), 0, NULL},

    /*015*/ {"网卡序号", "要打开的网卡序号。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*016*/ {"网卡模式", "可以为以下常量值之一：1、直接模式； 2、混杂模式（此模式可以监听所有流过本地主机的数据报）。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*017*/ {"缓冲区大小", "设置网卡接收数据包的缓冲区大小。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*018*/ {"等待时间", "设置接收一个数据报后的等待时间。单位为毫秒。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*019*/ {"以太网头", "待取得的以太网头。", 0, 0, MAKELONG(0x06, 0), 0, NULL},

    /*020*/ {"ARP头", NULL, 0, 0, MAKELONG(0x07, 0), 0, NULL},

    /*021*/ {"发送次数", "在发送一个ARP包时，重复发送的次数。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*022*/ {"数据", "待取得报头信息的数据。", 0, 0, SDT_BIN, 0, NULL},
    /*023*/ {"IP报头", "待取得的IP报头。", 0, 0, MAKELONG(0x03, 0), 0, NULL},

    /*024*/ {"数据", "待取得报头信息的数据。", 0, 0, SDT_BIN, 0, NULL},
    /*025*/ {"以太网头", "待取得的以太网头。", 0, 0, MAKELONG(0x06, 0), 0, NULL},

    /*026*/ {"数据", "待取得报头信息的数据。", 0, 0, SDT_BIN, 0, NULL},
    /*027*/ {"ARP头", "待取得的ARP头。", 0, 0, MAKELONG(0x07, 0), 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_erawsock_global_var) / sizeof(g_argumentInfo_erawsock_global_var[0]);
#endif


#define ERAWSOCK_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_erawsock_global_var[] = 
{
    ERAWSOCK_DEF(ERAWSOCK_DEF_CMDINFO)
};

int g_cmdInfo_erawsock_global_var_count = sizeof(g_cmdInfo_erawsock_global_var) / sizeof(g_cmdInfo_erawsock_global_var[0]);

#endif

