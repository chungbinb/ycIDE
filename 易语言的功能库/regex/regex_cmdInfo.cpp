#if !defined(__E_STATIC_LIB)
#include "include_regex_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_regex_global_var[] = 
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

    /*000*/ {"用来匹配的文本", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*001*/ {"正则表达式文本", "比如“易语言4\\.0(模块|支持库)?”", 0, 0, SDT_TEXT, 0, NULL},
    /*002*/ {"是否区分大小写", "本参数指定这个正则表达式在创建成功后进行匹配、搜索、替换时，是否区分大小写。如果本参数被省略，默认值为“真”", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*003*/ {"欲被搜索的文本", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*004*/ {"起始搜索位置", NULL, 0, 0, SDT_INT, 1, AS_HAS_DEFAULT_VALUE},
    /*005*/ {"搜索长度", "本参数和“起始搜索位置”一起指定搜索的范围。如果本参数被省略，默认值为从“起始搜索位置”到文本结尾的长度", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*006*/ {"被搜索的文本", "应该使用和所属的“正则表达式.搜索”命令同样的参数", 0, 0, SDT_TEXT, 0, NULL},
    /*007*/ {"起始位置", "该命令调用成功后，本参数存放取得的子文本在整个被搜索文本中的起始位置", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*008*/ {"被搜索的文本", "应该使用和所属的“正则表达式.搜索”命令同样的参数", 0, 0, SDT_TEXT, 0, NULL},
    /*009*/ {"子表达式索引", "欲取其值的子表达式。该参数对应与正则表达式中的一个子表达式（以圆括号标记）。索引从1开始", 0, 0, SDT_INT, 0, NULL},
    /*010*/ {"起始位置", "该命令调用成功后，本参数存放取得的子文本在整个被搜索文本中的起始位置", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

    /*011*/ {"源对象", NULL, 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*012*/ {"源对象", NULL, 0, 0, MAKELONG(0x02, 0), 0, NULL},

    /*013*/ {"欲被替换的文本", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*014*/ {"替换格式", "本参数指定替换的具体方式。比如“<font color = gray>$1</font>”表示把第一个子表达式所匹配的文本显示为灰色（在HTML中）", 0, 0, SDT_TEXT, 0, NULL},
    /*015*/ {"起始替换位置", NULL, 0, 0, SDT_INT, 1, AS_HAS_DEFAULT_VALUE},
    /*016*/ {"替换长度", "本参数和“起始替换位置”一起指定替换的范围。如果本参数被省略，默认值为从“起始替换位置”到文本结尾的长度", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*017*/ {"是否包含不匹配部分", "本参数指定替换结果中是否包含与正则表达式不匹配的部分。如果本参数为假，则不匹配部分将被抛弃", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},
    /*018*/ {"是否全部替换", "如果本参数为真，则参数1中所有匹配的子文本均被替换，否则仅第一个匹配的子文本被替换", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},

    /*019*/ {"分组名称", "本参数指定要获取的分组名称,分组命名形式为(?<name>.*?)", 0, 0, SDT_TEXT, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_regex_global_var) / sizeof(g_argumentInfo_regex_global_var[0]);
#endif


#define REGEX_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_regex_global_var[] = 
{
    REGEX_DEF(REGEX_DEF_CMDINFO)
};

int g_cmdInfo_regex_global_var_count = sizeof(g_cmdInfo_regex_global_var) / sizeof(g_cmdInfo_regex_global_var[0]);

#endif

