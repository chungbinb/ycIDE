#if !defined(__E_STATIC_LIB)
#include "include_shell_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_shell_global_var[] = 
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

    /*000*/ {"快捷方式文件名", "如果文件名不包含后缀,将自动加上\".lnk\"后缀名.", 0, 0, SDT_TEXT, 0, NULL},
    /*001*/ {"指向文件路径", "提供快捷方式所指向文件的全路径名", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*002*/ {"备注信息", "提供对应此快捷方式的描述文本信息", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*003*/ {"命令行文本", "提供启动快捷方式指向文件时附加的命令行文本", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*004*/ {"工作目录", "提供启动快捷方式指向文件时所需预置的工作目录", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*005*/ {"热键", "热键键值，具体参照微软接口", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*006*/ {"快捷方式文件名", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*007*/ {"标题", "本参数提供显示在对话框上部的标题文本", 0, 0, SDT_TEXT, 0, NULL},
    /*008*/ {"是否包含文件", "本参数指定是否在浏览框中同时列出文件名。如果被省略，默认值为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*009*/ {"窗口句柄", "默认为当前进程活动句柄，也就是焦点所在句柄", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*010*/ {"删除选项", "本参数提供删除操作选项，可以为以下常量值之一或之和： 1: #不询问删除; 2: #不提示错误; 4: #不显示进度。如果被省略，默认值为 0 ", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*011*/ {"欲删除文件", "本参数提供显示在对话框上部的标题文本", 0, 0, SDT_TEXT, 0, NULL},

    /*012*/ {"目的目录", "本参数提供目的目录路径，如果该目录不存在，系统将自动询问是否创建", 0, 0, SDT_TEXT, 0, NULL},
    /*013*/ {"源文件", "可以以数组方式同时提供多个文件或文件夹名，注意必须是全路径，支持通配符“*”和“?”", 0, 0, SDT_TEXT, 0, NULL},

    /*014*/ {"目的目录", "本参数提供目的目录路径，如果该目录不存在，系统将自动询问是否创建", 0, 0, SDT_TEXT, 0, NULL},
    /*015*/ {"源文件", "可以以数组方式同时提供多个文件或文件夹名，注意必须是全路径，支持通配符“*”和“?”", 0, 0, SDT_TEXT, 0, NULL},

    /*016*/ {"命令类型", "本参数提供欲执行命令的类型，可以为以下常量值之一： ， 1: #编辑文档; 启动编辑软件打开“欲操作文件名”参数所指定的文档文件进行编辑。 ， 2: #浏览文件夹; 打开“欲操作文件名”参数所指定的文件夹进行浏览。 ， 3: #寻找文件; 打开“欲操作文件名”参数所指定的文件夹进行文件寻找操作。 ， 4: #打开文件; 打开“欲操作文件名”参数所指定的文件，该文件可以是可执行文件、文档文件或文件夹。 ， 5: #打印文档; 打印“欲操作文件名”参数所指定的文档文件，注意该文档的编辑软件可能不支持本命令，故使用前需要先测试是否可用。 ， 如果本参数被省略或者为0，系统将首先搜寻注册表以获取对应于欲操作文件的一个合适命令类型，如果未找到，将使用“#打开文件”命令", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*017*/ {"欲操作文件名", "本参数提供欲操作文件或文件夹的名称", 0, 0, SDT_TEXT, 0, NULL},
    /*018*/ {"命令行", "如果欲进行操作的类型为“打开文件”且被打开文件为可执行文件，本参数提供启动该可执行文件时欲附加的命令行文本，其它情况下本参数将被忽略。如果本参数被省略，默认为空文本", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*019*/ {"默认当前目录", "本参数提供命令执行时的默认当前目录，如果被省略，默认为空文本", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*020*/ {"窗口显示方式", "参数值可以为以下常量之一：1、#隐藏窗口； 2、#普通激活； 3、#最小化激活； 4、#最大化激活； 5、#普通不激活； 6、#最小化不激活。如果省略本参数，默认为“普通激活”方式。如果参数“欲操作文件名”不是可执行文件，窗口显示方式将转交给与其相关联的可执行程序处理，至于是否能正确处理此显示方式，取决于该程序自身。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*021*/ {"欲获取目录类型", "本参数指定欲获取目录的类型，可以为以下常量值之一： 1: #我的文档; 2: #我的收藏夹; 3: #系统桌面; 4: #系统字体; 5: #开始菜单组; 6: #程序菜单组; 7: #启动菜单组; 8: #程序数据目录; 9: #Windows安装目录; 10: #Windows系统目录; 11: #临时文件目录", 0, 0, SDT_INT, 0, NULL},

    /*022*/ {"关闭方式", "本参数可以为以下常量值之一： 1: #关机; 2: #重启; 3: #注销; 4: #休眠 5: #冬眠。 注意在Windows95/98/ME下冬眠等同于休眠", 0, 0, SDT_INT, 0, NULL},
    /*023*/ {"是否强制执行", "本参数指定是否不等待当前正在运行的所有软件正常结束而立即执行指定的操作。如果本参数被省略，默认值为假", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_shell_global_var) / sizeof(g_argumentInfo_shell_global_var[0]);
#endif


#define SHELL_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_shell_global_var[] = 
{
    SHELL_DEF(SHELL_DEF_CMDINFO)
};

int g_cmdInfo_shell_global_var_count = sizeof(g_cmdInfo_shell_global_var) / sizeof(g_cmdInfo_shell_global_var[0]);

#endif

