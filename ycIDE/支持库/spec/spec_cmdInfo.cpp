#if !defined(__E_STATIC_LIB)
#include "include_spec_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_spec_global_var[] = 
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

    /*000*/ {"代码数据", "欲置入到代码段中的机器指令码数据，可以是字节集数据或二进制文件名文本", 0, 0, _SDT_ALL, 0, NULL},

    /*001*/ {"欲获取其地址的变量", NULL, 0, 0, _SDT_ALL, 0, AS_RECEIVE_VAR_OR_ARRAY},

    /*002*/ {"欲获取其地址的子程序", NULL, 0, 0, SDT_SUB_PTR, 0, NULL},

    /*003*/ {"变量", NULL, 0, 0, _SDT_ALL, 0, AS_RECEIVE_VAR_OR_ARRAY},

    /*004*/ {"欲输出值", "对于非“文本型”参数，将自动转换为文本。本参数可接受任意基本类型，但不接受数组以及自定义数据类型。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_ALL_TYPE_DATA},

    /*005*/ {"欲检查值", "可以为普通数值、变量、命令/方法调用、子语句等。如果本参数为命令/方法调用或子语句，总是会被执行，即使在非调试版本中。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_ALL_TYPE_DATA},

    /*006*/ {"子程序指针或子程序地址", "提示：在子程序名称前加“&”即可得到该子程序的“子程序指针”，如“&子程序1”就是“子程序1”的子程序指针；子程序地址可通过本库中的“取子程序地址()”命令获取，“到整数(&子程序1)”返回的也是子程序1的执行地址，也可通过Windows系统API函数“GetProcAddress()”获取，或由外部程序传入。", 0, 0, _SDT_ALL, 0, NULL},
    /*007*/ {"保留参数", NULL, 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*008*/ {"返回值", "本参数用于接收子程序被调用后的返回值；如果该子程序没有返回值或不需要接收返回值，请省略本参数。提供本参数时，请务必提供准确的数据类型，否则所得结果可能不正确。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*009*/ {"参数", "本参数值将作为参数传入被调用子程序。如果相应子程序没有参数，请省略本参数；如果相应子程序有多个参数，请重复提供本参数。请务必提供准确的参数类型和参数个数，否则后果不可预知。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*010*/ {"等待时间", "单位为毫秒。提示：1000毫秒等于1秒。", 0, 0, SDT_INT, 0, NULL},

    /*011*/ {"推迟时间", "单位为毫秒。如果本参数小于或等于0，则子程序将被立刻调用。本参数最大可设置为 2147483647(0x7fffffff) 毫秒（约 596 小时）。", 0, 0, SDT_INT, 0, NULL},
    /*012*/ {"子程序指针或子程序地址", "提示：在子程序名称前加“&”即可得到该子程序的“子程序指针”，如“&子程序1”就是“子程序1”的子程序指针；子程序地址可通过本库中的“取子程序地址()”命令获取，“到整数(&子程序1)”返回的也是子程序1的执行地址，也可通过Windows系统API函数“GetProcAddress()”获取，或由外部程序传入。", 0, 0, _SDT_ALL, 0, NULL},
    /*013*/ {"保留参数", NULL, 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*014*/ {"返回值", "本参数用于接收子程序被调用后的返回值；如果该子程序没有返回值或不需要接收返回值，请省略本参数。提供本参数时，请务必提供准确的数据类型，否则所得结果可能不正确。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*015*/ {"参数", "本参数值将作为参数传入被调用子程序。如果相应子程序没有参数，请省略本参数；如果相应子程序有多个参数，请重复提供本参数。请务必提供准确的参数类型和参数个数，否则后果不可预知。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*016*/ {"欲申请的内存字节数", NULL, 0, 0, SDT_INT, 0, NULL},
    /*017*/ {"是否清零", "如果本参数为“真”，则将申请到的内存区域全部以0填充。如果省略本参数，默认为“假”。", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*018*/ {"欲释放的内存地址", "本参数应当是“申请内存”命令的返回值。", 0, 0, SDT_INT, 0, NULL},

    /*019*/ {"功能号", "可以为易语言支持库开发手册中定义的以“NES_”,“NRS_”,“NAS_”开头的常量之一。", 0, 0, SDT_INT, 0, NULL},
    /*020*/ {"参数一", "如果本参数被省略，默认为0。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*021*/ {"参数二", "如果本参数被省略，默认为0。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*022*/ {"原始文本", "此文本中可包含“半角双引号替换符（初始值为\"`\"）”“全角左引号替换符（初始值为\"~\"）”“全角右引号替换符（初始值为\"~\"）”“文本替换符”。其中“文本替换符”由前缀文本（初始值为\"%\"）和紧接着的下一个半角字符（可为\"s\",\"d\",\"f\",\"t\",\"T\",\"n\",\"N\"等）组成。当命令执行后，此文本中的这些特殊文本将被替换为相应的文本（如`被替换为\"，~被分别替换为“和”，%s,%d,%f被替换为参数文本，%t被替换为半角双引号包围的参数文本，%T被替换为全角双引号包围的参数文本），%n被替换为换行符（即易语言中的“#换行符”常量），%N被替换为两个换行符（相当于两个连续的%n）。如果此参数的类型不是“文本型”，则首先将其转换为文本型，再进行如上处理。本命令的返回值，就是经过以上文本替换操作后的结果文本。上述提到的各种替换符或其前缀，可通过“取文本_属性设置”命令自行定义。注意：当“文本替换符”为%n或%N时，它不占用后面的参数值。", 0, 0, _SDT_ALL, 0, AS_RECEIVE_ALL_TYPE_DATA},
    /*023*/ {"参数", "此参数值用于替换上一参数中的“文本替换符”，并与之依序一一对应。即，此处的第一个参数用于替换第一个“文本替换符”，第二个参数用于替换第二个“文本替换符”，如此类推，但特殊情况下除外（如%n和%N就不占用此参数）。特别地，如果本参数为数组，则将数组中的每一个成员作为一个逻辑单位与“文本替换符”进行逐一匹配和替换。如果本参数不是文本型，则首先将其（或其成员）转换为文本，然后进行替换操作。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_ALL_TYPE_DATA},

    /*024*/ {"半角双引号替换符", "当调用“取文本”命令时，此处指定的参数值将被替换为半角双引号(\")。如果本参数被省略，则保持原有替换符不变。系统初始化的半角双引号替换符是“`”（此键位于ESC键下方）。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*025*/ {"全角左引号替换符", "当调用“取文本”命令时，此处指定的参数值将被替换为全角左引号(“)。如果本参数被省略，则保持原有替换符不变。系统初始化的全角左引号替换符是“~”（此键位于ESC键下方，输入时需同时按下Shift）。注意，此参数值不可等同于第一个参数“半角双引号替换符”。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*026*/ {"全角右引号替换符", "当调用“取文本”命令时，此处指定的参数值将被替换为全角右引号(”)。如果本参数被省略，则保持原有替换符不变。系统初始化的全角右引号替换符是“~”（此键位于ESC键下方，输入时需同时按下Shift）。注意，此参数值可以等同于第二个参数“全角左引号替换符”，但不可等同于第一个参数“半角双引号替换符”。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*027*/ {"文本替换符前缀", "当调用“取文本”命令时，完整的文本替换符（由前缀+特定字符组成）将被替换为相应位置处的参数文本（或特定文本）。如果本参数被省略，则保持原有替换符前缀不变。系统初始化的文本替换符前缀是“%”。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*028*/ {"半角双引号替换符", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*029*/ {"全角左引号替换符", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*030*/ {"全角右引号替换符", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*031*/ {"文本替换符前缀", NULL, 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*032*/ {"推迟时间", "单位为毫秒。如果本参数小于或等于0，则子程序将被立刻调用。本参数最大可设置为 2147483647(0x7fffffff) 毫秒（约 596 小时）。", 0, 0, SDT_INT, 0, NULL},
    /*033*/ {"子程序指针或子程序地址", "提示：在子程序名称前加“&”即可得到该子程序的“子程序指针”，如“&子程序1”就是“子程序1”的子程序指针；子程序地址可通过本库中的“取子程序地址()”命令获取，“到整数(&子程序1)”返回的也是子程序1的执行地址，也可通过Windows系统API函数“GetProcAddress()”获取，或由外部程序传入。", 0, 0, _SDT_ALL, 0, NULL},
    /*034*/ {"保留参数", NULL, 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*035*/ {"返回值", "本参数用于接收子程序被调用后的返回值；如果该子程序没有返回值或不需要接收返回值，请省略本参数。提供本参数时，请务必提供准确的数据类型，否则所得结果可能不正确。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},
    /*036*/ {"参数", "本参数值将作为参数传入被调用子程序。如果相应子程序没有参数，请省略本参数；如果相应子程序有多个参数，请重复提供本参数。请务必提供准确的参数类型和参数个数，否则后果不可预知。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_spec_global_var) / sizeof(g_argumentInfo_spec_global_var[0]);
#endif


#define SPEC_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_spec_global_var[] = 
{
    SPEC_DEF(SPEC_DEF_CMDINFO)
};

int g_cmdInfo_spec_global_var_count = sizeof(g_cmdInfo_spec_global_var) / sizeof(g_cmdInfo_spec_global_var[0]);

#endif

