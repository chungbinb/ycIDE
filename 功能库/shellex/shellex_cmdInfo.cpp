#if !defined(__E_STATIC_LIB)
#include "include_shellex_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_shellex_global_var[] = 
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

    /*000*/ {"控件句柄", "需要添加提示标签的控件的句柄。", 0, 0, SDT_INT, 0, NULL},
    /*001*/ {"提示文本", "提示标签上显示的文字，如果要显示多行，请加上#换行符。", 0, 0, _SDT_ALL, 0, NULL},
    /*002*/ {"是否为气泡型", "comctl32.dll版本必须≥v5.8，以第一次调用为准，因为所有提示都相同，不能分开定义。", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},
    /*003*/ {"提示工具位置", "为真表示总显示在控件的中下方，为假表示跟随鼠标。", 0, 0, SDT_BOOL, 0, NULL},

    /*004*/ {"控件句柄", "需要删除提示标签的控件的句柄。", 0, 0, SDT_INT, 0, NULL},

    /*005*/ {"控件句柄", "需要修改提示标签的控件的句柄。", 0, 0, SDT_INT, 0, NULL},
    /*006*/ {"提示文本", "新的提示标签的文本。", 0, 0, _SDT_ALL, 0, NULL},

    /*007*/ {"背景底色", "新的提示标签的背景颜色，-1表示系统背景颜色。", 0, 0, SDT_INT, 0, NULL},

    /*008*/ {"文本颜色", "新的提示标签的文本颜色，-1表示系统文本颜色。", 0, 0, SDT_INT, 0, NULL},

    /*009*/ {"显示时间", "提示标签的显示时间，-1表示一直显示。", 0, 0, SDT_INT, 0, NULL},

    /*010*/ {"图标类型", "提示标签的图标类型，0:无图标,1:信息,2:警告,3:错误", 0, 0, SDT_INT, 0, NULL},
    /*011*/ {"图标标题文本", "提示标签的图标标题文本，如果为空文本，则表示无图标。", 0, 0, _SDT_ALL, 0, NULL},

    /*012*/ {"窗口句柄", "需要修改提示标签的窗口句柄。", 0, 0, SDT_INT, 0, NULL},
    /*013*/ {"提示字体", "需要设置的提示字体，注意角度属性无效。", 0, 0, DTP_FONT, 0, NULL},

    /*014*/ {"标签句柄", "使用标签的反馈事件来操作拖放后的功能，标签反馈事件传递过来的参数中参数一为拖放的控件句柄，参数二第一次为拖放的文件个数，而后为拖放的文件名", 0, 0, SDT_INT, 0, NULL},
    /*015*/ {"控件句柄", "需要设置成拖放状态的组件的句柄", 0, 0, SDT_INT, 0, NULL},

    /*016*/ {"窗口句柄", "树型框父窗口句柄", 0, 0, SDT_INT, 0, NULL},
    /*017*/ {"树型框句柄", "需要设置成拖放状态的树型框的句柄", 0, 0, SDT_INT, 0, NULL},
    /*018*/ {"标签句柄", "使用标签的反馈事件来操作拖放后的功能，标签反馈事件传递过来的参数中参数一为拖放的控件句柄，参数二第一次为拖放的文件个数，而后为拖放的文件名", 0, 0, SDT_INT, 0, NULL},

    /*019*/ {"窗口句柄", "窗口句柄", 0, 0, SDT_INT, 0, NULL},
    /*020*/ {"标签句柄", "使用标签的反馈事件来接受热键，反馈事件中的第一个参数为热键标识，第二个参数无效", 0, 0, SDT_INT, 0, NULL},
    /*021*/ {"功能键", "可以为：0-无功能键；1-CTRL键状态；2-SHIFT键状态；4-ALT键状态或各键状态值之和", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
    /*022*/ {"主热键", "键代码，可以使用易语言中的键代码常量", 0, 0, SDT_INT, 0, NULL},

    /*023*/ {"窗口句柄", "窗口句柄", 0, 0, SDT_INT, 0, NULL},
    /*024*/ {"热键标识", "注册热键时所返回的热键标识", 0, 0, SDT_INT, 0, NULL},

    /*025*/ {"参数", "要取得地址的一个参数", 0, 0, _SDT_ALL, 0, NULL},

    /*026*/ {"指针地址", "指针的地址", 0, 0, SDT_SUB_PTR, 0, NULL},
    /*027*/ {"参数", "参数", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*028*/ {"延迟时间", "欲显示时鼠标需悬停的时间", 0, 0, SDT_INT, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_shellex_global_var) / sizeof(g_argumentInfo_shellex_global_var[0]);
#endif


#define SHELLEX_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_shellex_global_var[] = 
{
    SHELLEX_DEF(SHELLEX_DEF_CMDINFO)
};

int g_cmdInfo_shellex_global_var_count = sizeof(g_cmdInfo_shellex_global_var) / sizeof(g_cmdInfo_shellex_global_var[0]);

#endif

