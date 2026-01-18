#if !defined(__E_STATIC_LIB)
#include "include_deelxregex_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_deelxregex_global_var[] = 
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



    /*000*/ {"正则表达式文本", "比如“易语言5\\.0(模块|支持库)?”", 0, 0, SDT_TEXT, 0, NULL},
    /*001*/ {"匹配模式", "支持的匹配模式有：单行模式、多行模式、全局模式、忽略大小写、从右向左、扩展模式 这 6 种模式以及它们的组合。\r\n可以用如下常量（#正则常量.单行模式；#正则常量.多行模式；#正则常量.全局模式；#正则常量.忽略大小写；#正则常量.从右向左；#正则常量.扩展模式）可以查看支持库常量说明", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*002*/ {"支持转义符", "默认为假，为真时支持易语言的部分常量/转义符：#换行符、#引号、#左引号、#右引号", 0, 0, SDT_BOOL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*003*/ {"用来匹配的文本", "进行匹配的字符串", 1, 2, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*004*/ {"开始查找匹配的位置", "开始查找匹配的位置.", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*005*/ {"分组编号", "分组编号", 1, 2, SDT_INT, 0, AS_RECEIVE_VAR_OR_OTHER},

    /*006*/ {"命名分组名", "命名分组名", 1, 2, SDT_TEXT, 0, AS_RECEIVE_VAR_OR_OTHER},

    /*007*/ {"用来匹配的文本", "进行匹配的字符串", 1, 2, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*008*/ {"开始查找匹配的位置", "开始查找匹配的位置.", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*009*/ {"欲被替换的文本", "被进行替换的初始文本。", 1, 2, SDT_TEXT, 0, NULL},
    /*010*/ {"用作替换的文本", "“替换为”字符串，将匹配到的子字符串替换成 此变量的 字符串。", 1, 2, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*011*/ {"起始替换位置", "进行查找替换的开始位置。留空默认(-1)表示根据是否“从右向左(RIGHTTOLEFT)”自动决定开始位置。", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*012*/ {"替换进行的次数", "指定进行替换的次数。留空默认(-1)表示替换所有匹配。", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*013*/ {"由(替换)返回的字符串", "释放由 \"替换\" 返回的字符串。", 1, 2, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*014*/ {"搜索结果起始位置", "搜索结果起始位置", 1, 2, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
    /*015*/ {"搜索结果结束位置", "搜索结果结束位置", 1, 2, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},

    /*016*/ {"子表达式索引或名称", "欲取其值的子表达式。该参数对应与正则表达式中的一个子表达式（以圆括号标记）。子匹配索引从1开始，0表示匹配文本。\r\n\t或者传递文本型的子表达式分组名称，比如“(?<数字>\\d+)(?<小写字母>[a-z]+)(?<大写字母>[A-Z]+)”这样的表达式，可以传递“数字”、“小写字母”等名称。\r\n\t也可以传递字节集形式的参数，当成Unicode格式的名称对待。", 1, 2, _SDT_ALL, 0, NULL},

    /*017*/ {"用来匹配的文本", "进行匹配的字符串", 1, 2, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*018*/ {"开始查找匹配的位置", "开始查找匹配的位置.", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*019*/ {"用来匹配的文本", "进行匹配的字符串", 1, 2, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*020*/ {"开始查找匹配的位置", "开始查找匹配的位置.", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*021*/ {"正则表达式文本", "Unicode格式的正则表达式文本，比如“易语言5\\.0(模块|支持库)?”", 0, 0, SDT_BIN, 0, NULL},
    /*022*/ {"匹配模式", "支持的匹配模式有：单行模式、多行模式、全局模式、忽略大小写、从右向左、扩展模式 这 6 种模式以及它们的组合。\r\n可以用如下常量（#正则常量.单行模式；#正则常量.多行模式；#正则常量.全局模式；#正则常量.忽略大小写；#正则常量.从右向左；#正则常量.扩展模式）可以查看支持库常量说明", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*023*/ {"用来匹配的文本", "Unicode格式，进行匹配的字符串", 1, 2, SDT_BIN, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*024*/ {"开始查找匹配的位置", "开始查找匹配的位置.", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*025*/ {"用来匹配的文本", "Unicode格式，进行匹配的字符串", 1, 2, SDT_BIN, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*026*/ {"开始查找匹配的位置", "开始查找匹配的位置.", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*027*/ {"用来匹配的文本", "Unicode格式，进行匹配的字符串", 1, 2, SDT_BIN, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*028*/ {"开始查找匹配的位置", "开始查找匹配的位置.", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*029*/ {"欲被替换的文本", "Unicode格式，被进行替换的初始文本。", 1, 2, SDT_BIN, 0, NULL},
    /*030*/ {"用作替换的文本", "Unicode格式，“替换为”字符串，将匹配到的子字符串替换成 此变量的 字符串。", 1, 2, SDT_BIN, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*031*/ {"起始替换位置", "进行查找替换的开始位置。留空默认(-1)表示根据是否“从右向左(RIGHTTOLEFT)”自动决定开始位置。", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*032*/ {"替换进行的次数", "指定进行替换的次数。留空默认(-1)表示替换所有匹配。", 1, 2, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*033*/ {"搜索结果起始位置", "搜索结果起始位置", 1, 2, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
    /*034*/ {"搜索结果结束位置", "搜索结果结束位置", 1, 2, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},

    /*035*/ {"待分割文本", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*036*/ {"要返回的子文本数目", "如果被省略或等于0，则默认返回所有的子文本", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*037*/ {"待分割文本", "Unicode格式文本", 0, 0, SDT_BIN, 0, NULL},
    /*038*/ {"要返回的子文本数目", "如果被省略或等于0，则默认返回所有的子文本", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*039*/ {"用来匹配的文本", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*040*/ {"要返回的匹配文本数目", "如果被省略或等于0，则默认返回所有的匹配文本", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*041*/ {"用来匹配的文本", "Unicode格式文本", 0, 0, SDT_BIN, 0, NULL},
    /*042*/ {"要返回的匹配文本数目", "如果被省略或等于0，则默认返回所有的匹配文本", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_deelxregex_global_var) / sizeof(g_argumentInfo_deelxregex_global_var[0]);
#endif


#define DEELXREGEX_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_deelxregex_global_var[] = 
{
    DEELXREGEX_DEF(DEELXREGEX_DEF_CMDINFO)
};

int g_cmdInfo_deelxregex_global_var_count = sizeof(g_cmdInfo_deelxregex_global_var) / sizeof(g_cmdInfo_deelxregex_global_var[0]);

#endif

