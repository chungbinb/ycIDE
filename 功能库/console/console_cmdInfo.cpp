#if !defined(__E_STATIC_LIB)
#include "include_console_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_console_global_var[] = 
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




    /*000*/ {"横坐标", "指定输入数据时光标的横坐标", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*001*/ {"纵坐标", "指定输入数据时光标的纵坐标", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*002*/ {"保存当前光标", "是否保存当前光标位置", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},
    /*003*/ {"是否回显", "是否把输入的数据回显到控制台设备", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},
    /*004*/ {"回显数据", "回显的数据，如果忽略，则使用输入的字符，在“是否回显”为真时有意义", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*005*/ {"回车结束", "是否以回车结束输入", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},
    /*006*/ {"最大接收长度", "最大接收数据长度，“回车结束”为真时有意义", 0, 0, SDT_INT, 1024, AS_HAS_DEFAULT_VALUE},

    /*007*/ {"横坐标", "指定输出数据时光标的横坐标", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*008*/ {"纵坐标", "指定输出数据时光标的纵坐标", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*009*/ {"保存当前光标", "是否保存当前光标位置", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},
    /*010*/ {"前景颜色", "前景即字体的颜色，颜色值为“控制台颜色”数据类型中定义的值", 0, 0, SDT_INT, 8, AS_HAS_DEFAULT_VALUE},
    /*011*/ {"背景颜色", "背景的颜色，颜色值为“控制台颜色”数据类型中定义的值", 0, 0, SDT_INT, 1, AS_HAS_DEFAULT_VALUE},
    /*012*/ {"输出数据", "待输出的数据", 0, 0, SDT_TEXT, 0, NULL},

    /*013*/ {"横坐标", "光标的新横坐标", 0, 0, SDT_INT, 0, NULL},
    /*014*/ {"纵坐标", "光标的新纵坐标", 0, 0, SDT_INT, 0, NULL},

    /*015*/ {"横坐标", "光标的横坐标", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},
    /*016*/ {"纵坐标", "光标的新纵坐标", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},

    /*017*/ {"宽度", "显示区宽度，以字符为单位", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},
    /*018*/ {"高度", "显示区高度，以字符为单位", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},

    /*019*/ {"横坐标", "开始填充的横坐标", 0, 0, SDT_INT, 0, NULL},
    /*020*/ {"纵坐标", "开始填充的纵坐标", 0, 0, SDT_INT, 0, NULL},
    /*021*/ {"背景颜色", "背景颜色", 0, 0, SDT_INT, 0, NULL},
    /*022*/ {"填充长度", "填充区域的长度", 0, 0, SDT_INT, 0, NULL},

    /*023*/ {"横坐标", "开始填充的横坐标", 0, 0, SDT_INT, 0, NULL},
    /*024*/ {"纵坐标", "开始填充的纵坐标", 0, 0, SDT_INT, 0, NULL},
    /*025*/ {"填充字符", "本参数是一个文本，但只有第一个字符有效", 0, 0, SDT_TEXT, 0, NULL},
    /*026*/ {"填充长度", "填充区域的长度", 0, 0, SDT_INT, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_console_global_var) / sizeof(g_argumentInfo_console_global_var[0]);
#endif


#define CONSOLE_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_console_global_var[] = 
{
    CONSOLE_DEF(CONSOLE_DEF_CMDINFO)
};

int g_cmdInfo_console_global_var_count = sizeof(g_cmdInfo_console_global_var) / sizeof(g_cmdInfo_console_global_var[0]);

#endif

