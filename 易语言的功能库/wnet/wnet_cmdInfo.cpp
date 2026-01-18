#if !defined(__E_STATIC_LIB)
#include "include_wnet_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_wnet_global_var[] = 
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



    /*000*/ {"欲取的资源类型", "本参数指定欲取出的资源的类型。该参数可以为“资源类型”枚举常量集合中的一个常量值", 0, 0, MAKELONG(0x02, 0), 0, AS_HAS_DEFAULT_VALUE},
    /*001*/ {"取得的资源", "命令执行完毕后本变量数组中依次存放取得的资源的路径", 0, 0, SDT_TEXT, 0, AS_RECEIVE_VAR_ARRAY},
    /*002*/ {"取得的资源类型", "命令执行完毕后本变量数组中依次存放取得的资源的类型，并与取得的资源路径一一对应。该数组成员值可以为以下常量之一：“#资源类型.共享文件夹”或“#资源类型.共享打印机”", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR_ARRAY},

    /*003*/ {"取得的主机名", "命令执行完毕后本变量数组中依次存放取得的主机名", 0, 0, SDT_TEXT, 0, AS_RECEIVE_VAR_ARRAY},

    /*004*/ {"欲测试的资源路径", "可以为局域网中的共享文件夹路径，比如“\\\\Prog5\\共享测试”，也可以为映射到本地的网络驱动器名称，比如“Y:”", 0, 0, SDT_TEXT, 0, NULL},

    /*005*/ {"欲映射的资源路径", "该参数可以为局域网中的共享文件夹或共享打印机的路径，比如“\\\\Prog5\\共享测试”，“\\\\Prog5\\惠普打印机”", 0, 0, SDT_TEXT, 0, NULL},
    /*006*/ {"欲映射到的本地设备名称", "比如“Y:”，“LPT1”。如果本参数省略，系统将自动选择一个可用的设备名称", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*007*/ {"用户名", "访问指定的共享使用的用户名。如果本参数省略，将使用系统默认值", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*008*/ {"密码", "访问指定的共享使用的密码。如果本参数省略，将使用系统默认值", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*009*/ {"是否提示错误", "本参数指定当用户名或密码错误时，是否提示重新输入。如果提供的用户名或密码有误，且本参数为假，该命令将失败。如果本参数省略，默认值为“真”", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*010*/ {"是否自动重新连接", "本参数指定是否在下次登陆计算机时自动重新连接该映射驱动器。如果本参数省略，默认值为“真”", 0, 0, SDT_BOOL, 1, AS_DEFAULT_VALUE_IS_EMPTY},

    /*011*/ {"欲断开的映射驱动器", "本参数指定欲断开连接的映射驱动器（或打印机）名称。比如“X:”或“LPT1”", 0, 0, SDT_TEXT, 0, NULL},
    /*012*/ {"是否永久断开", "本参数指定是否永久断开该映射驱动器。永久断开的映射驱动器在下次登录时不会自动重新连接", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},
    /*013*/ {"是否强制断开", "如果本参数为“真”，即使本映射驱动器正在使用，也立即强制断开", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},

    /*014*/ {"网络路径初始值", "本参数指定出现在“映射网络驱动器”对话框中网络路径编辑框中的初始值", 0, 0, SDT_TEXT, 9543032, AS_HAS_DEFAULT_VALUE},
    /*015*/ {"网络路径可否输入", "本参数指定对话框中的网络路径编辑框是否可用", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},
    /*016*/ {"重新连接是否可选", "本参数指定对话框中“登录时重新连接”复选框是否可用", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},
    /*017*/ {"是否自动连接", "本参数指定对话框中“登录时重新连接”复选框是否被选中", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},
    /*018*/ {"父窗口句柄", "本参数指定打开的对话框的父窗口", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*019*/ {"父窗口句柄", "本参数指定打开的对话框的父窗口", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*020*/ {"网络驱动器名称", "本参数指定欲取其对应的资源路径的网络驱动器，比如“X:”", 0, 0, SDT_TEXT, 0, NULL},

    /*021*/ {"网络驱动器路径", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*022*/ {"取得的资源路径", NULL, 0, 0, SDT_TEXT, 0, AS_RECEIVE_VAR},
    /*023*/ {"取得的连接名称", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*024*/ {"取得的相对路径", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_wnet_global_var) / sizeof(g_argumentInfo_wnet_global_var[0]);
#endif


#define WNET_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_wnet_global_var[] = 
{
    WNET_DEF(WNET_DEF_CMDINFO)
};

int g_cmdInfo_wnet_global_var_count = sizeof(g_cmdInfo_wnet_global_var) / sizeof(g_cmdInfo_wnet_global_var[0]);

#endif

