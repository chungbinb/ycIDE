#if !defined(__E_STATIC_LIB)
#include "include_esslayer_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_esslayer_global_var[] = 
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


    /*000*/ {"端口号", "服务器的端口号", 0, 0, SDT_INT, 0, NULL},
    /*001*/ {"处理函数", "本参数作为服务程序的事件触发方法，如果有默认事件产生，系统将自动调用的该函数。例如客户端发送数据到服务程序，或者是有客户端连接到服务程序，或者有客户端主动或意外断开与服务程序的连接，系统都会自动执行该方法。该函数没有参数", 0, 0, SDT_SUB_PTR, 0, NULL},

    /*002*/ {"私钥文本", "为十六进制数值文本，为了生成符合要求的随机密钥，可以下载使用 http://dywt.com.cn/RSATool2v14.rar 第三方工具软件（内有操作说明）", 0, 0, SDT_TEXT, 0, NULL},
    /*003*/ {"公共模数", "为十六进制数值文本", 0, 0, SDT_TEXT, 0, NULL},

    /*004*/ {"客户端句柄", "欲发送到客户端的句柄", 0, 0, SDT_INT, 0, NULL},
    /*005*/ {"数据", "准备发送的数据", 0, 0, _SDT_ALL, 0, NULL},

    /*006*/ {"客户句柄", "客户的句柄值", 0, 0, SDT_INT, 0, NULL},

    /*007*/ {"数据", "客户端发送到服务器的数据", 0, 0, SDT_BIN, 0, AS_RECEIVE_VAR},

    /*008*/ {"公钥文本", "为十六进制数值文本", 0, 0, SDT_TEXT, 0, NULL},
    /*009*/ {"公共模数", "为十六进制数值文本", 0, 0, SDT_TEXT, 0, NULL},

    /*010*/ {"加密方式", "该值为0代表采用rc4方法加密，该值为1代表采用des方法加密", 0, 0, SDT_INT, 0, NULL},

    /*011*/ {"服务器地址", "本参数是欲连接的服务器的IP地址，也可以是服务器的域名或者是内网的计算机名", 0, 0, SDT_TEXT, 0, NULL},
    /*012*/ {"端口", "待连接服务器的端口", 0, 0, SDT_INT, 0, NULL},
    /*013*/ {"处理函数", "本参数作为客户程序的事件触发方法，如果有默认事件产生，系统将自动调用的该函数。例如服务器发送数据到客户端，或者是服务器主动或意外断开与客户端的连接，系统都会自动执行该方法。此处理函数可以没有参数，或有一个整数型参数，见下一个参数的说明。", 0, 0, SDT_SUB_PTR, 0, NULL},
    /*014*/ {"处理函数参数", "如果提供了本参数，则处理函数必须接收一个整数型参数，此参数值将被传递到处理函数中。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*015*/ {"数据", "服务器发送到客户端的数据", 0, 0, SDT_BIN, 0, AS_RECEIVE_VAR},

    /*016*/ {"数据", "准备发送的数据", 0, 0, _SDT_ALL, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_esslayer_global_var) / sizeof(g_argumentInfo_esslayer_global_var[0]);
#endif


#define ESSLAYER_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_esslayer_global_var[] = 
{
    ESSLAYER_DEF(ESSLAYER_DEF_CMDINFO)
};

int g_cmdInfo_esslayer_global_var_count = sizeof(g_cmdInfo_esslayer_global_var) / sizeof(g_cmdInfo_esslayer_global_var[0]);

#endif

