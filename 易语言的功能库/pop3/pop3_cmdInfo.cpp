#if !defined(__E_STATIC_LIB)
#include "include_pop3_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_pop3_global_var[] = 
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











    /*000*/ {"收信邮件服务器地址", "指定用作接收邮件的POP3邮件服务器地址。", 0, 0, SDT_TEXT, 0, NULL},
    /*001*/ {"端口号", "指定欲连接到POP3邮件服务器上的端口号，如果本参数被省略，默认值为标准 110 端口。", 0, 0, SDT_INT, 110, AS_HAS_DEFAULT_VALUE},
    /*002*/ {"用户名", "指定在POP3邮件服务器上的用户帐号名称。", 0, 0, SDT_TEXT, 0, NULL},
    /*003*/ {"密码", "指定在POP3邮件服务器上的用户帐号密码。", 0, 0, SDT_TEXT, 0, NULL},
    /*004*/ {"最长等待时间", "指定在收信过程中等待POP3邮件服务器响应的最大时间，单位为毫秒。", 0, 0, SDT_INT, 30000, AS_HAS_DEFAULT_VALUE},
    /*005*/ {"重试次数", "指定在验证阶段，如果某一项(如：用户名或口令)验证失败后重试的次数。如果本参数被省略，默认值为 3 次。", 0, 0, SDT_INT, 3, AS_HAS_DEFAULT_VALUE},

    /*006*/ {"个数", "返回邮件的个数。", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},
    /*007*/ {"大小", "返回邮件的总的大小，单位为字节。", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},

    /*008*/ {"第几封", "指定是第几封邮件,如果此值为-1就表示获取每一封的大小。", 0, 0, SDT_INT, -1, AS_HAS_DEFAULT_VALUE},
    /*009*/ {"大小", "返回一封或多封邮件的大小。如果为此参数提供数组变量，而参数“第几封”不为-1，则指定邮件的大小将写入数组第一个成员中。", 0, 0, SDT_INT, -1, AS_RECEIVE_VAR_OR_ARRAY},
    /*010*/ {"总数", "本参数在“第几封”为-1时有效,为“获取邮件信息”方法返回的邮件总数。如果被省略，则自动获取邮件总数。", 0, 0, SDT_INT, -1, AS_DEFAULT_VALUE_IS_EMPTY},

    /*011*/ {"第几封", "表示接收第几封邮件。", 0, 0, SDT_INT, 0, NULL},

    /*012*/ {"第几封", "表示删除第几封邮件。", 0, 0, SDT_INT, 0, NULL},

    /*013*/ {"第几封", "指定是第几封邮件,如果此值为-1就表示获取每一封的序号。", 0, 0, SDT_INT, -1, AS_HAS_DEFAULT_VALUE},
    /*014*/ {"序号", "返回一封或多封邮件的序号。", 0, 0, SDT_TEXT, -1, AS_RECEIVE_VAR_OR_ARRAY},

    /*015*/ {"第几封", "指定是第几封邮件。", 0, 0, SDT_INT, 0, NULL},
    /*016*/ {"共几行", "指定要获取邮件的前几行内容。", 0, 0, SDT_INT, 0, NULL},

    /*017*/ {"函数地址", "本参数表示函数的地址，是易语言定义的子程序的地址。为0表示取消注册的回调函数。子程序的参数第一个为“整数型”表示接收邮件命令类型值，是“命令类型”数据类型中的常量值；参数第二个为“整数型”表示接收第几封邮件；参数第三个为“整数型”表示本次接收到的数据量单位为字节。子程序返回值为逻辑型。返回真表示继续接收此邮件，返回假表示不接收此邮件。", 0, 0, SDT_SUB_PTR, 0, NULL},

    /*018*/ {"类型", "表示代理服务器的类型，0：不用代理，1：HTTP1.1代理，2：SOCKS5代理。", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
    /*019*/ {"地址", "表示代理服务器的IP地址。", 0, 0, SDT_TEXT, 0, NULL},
    /*020*/ {"端口", "表示代理服务器的端口。", 0, 0, SDT_INT, 0, NULL},
    /*021*/ {"用户名", "表示代理服务器需要验证的用户名。", 0, 0, SDT_TEXT, 0, NULL},
    /*022*/ {"口令", "表示代理服务器需要验证的口令。", 0, 0, SDT_TEXT, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_pop3_global_var) / sizeof(g_argumentInfo_pop3_global_var[0]);
#endif


#define POP3_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_pop3_global_var[] = 
{
    POP3_DEF(POP3_DEF_CMDINFO)
};

int g_cmdInfo_pop3_global_var_count = sizeof(g_cmdInfo_pop3_global_var) / sizeof(g_cmdInfo_pop3_global_var[0]);

#endif

