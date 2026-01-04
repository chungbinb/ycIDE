#if !defined(__E_STATIC_LIB)
#include "include_commobj_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_commobj_global_var[] = 
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

























    /*000*/ {"起始位置", "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*001*/ {"字符数", "如果本参数被省略，默认为从“起始位置”到文本结尾的字符个数。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*002*/ {"数据", "参数类型支持各种基本数据类型及其数组数据。", 0, 0, _SDT_ALL, 0, AS_RECEIVE_ALL_TYPE_DATA},

    /*003*/ {"欲添加的数据", "参数类型支持各种基本数据类型及其数组数据。", 0, 0, _SDT_ALL, 0, AS_RECEIVE_ALL_TYPE_DATA},

    /*004*/ {"插入位置", "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*005*/ {"欲插入的数据", "参数类型支持各种基本数据类型及其数组数据。", 0, 0, _SDT_ALL, 0, AS_RECEIVE_ALL_TYPE_DATA},

    /*006*/ {"起始删除位置", "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。", 0, 0, SDT_INT, 0, NULL},
    /*007*/ {"欲删除的字符数", "如果参数值为-1，表示删除从“起始删除位置”开始到文本结尾的所有字符。", 0, 0, SDT_INT, 0, NULL},

    /*008*/ {"欲寻找的文本", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*009*/ {"起始寻找位置", "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*010*/ {"是否区分大小写", NULL, 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},

    /*011*/ {"欲寻找的文本", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*012*/ {"起始寻找位置", "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*013*/ {"是否区分大小写", NULL, 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},

    /*014*/ {"起始替换位置", "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。", 0, 0, SDT_INT, 0, NULL},
    /*015*/ {"替换长度", NULL, 0, 0, SDT_INT, 0, NULL},
    /*016*/ {"替换为文本", "如果本参数被省略，则删除文本中的指定部分。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*017*/ {"被替换文本", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*018*/ {"替换为文本", "如果本参数被省略，则表示将删除被替换文本。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*019*/ {"起始替换位置", "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*020*/ {"最大替换次数", "如果为-1，表示不限制替换次数。如果本参数被省略，默认为-1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*021*/ {"是否区分大小写", NULL, 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},

    /*022*/ {"用作分割的文本", "如果被省略，则默认使用半角逗号“,”字符作为分隔符。如果是一个长度为零的文本，则返回的数组仅包含一个成员，其内容是本对象中的文本。", 0, 0, SDT_TEXT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*023*/ {"要返回的子文本数目", "如果为-1，表示不限制替换次数。如果本参数被省略，默认为-1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*024*/ {"起始字符位置", "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*025*/ {"字符个数", "如果本参数被省略，默认为从参数“起始字符位置”开始到文本结尾的字符个数。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*026*/ {"起始字符位置", "位置索引可以为正值，1表示第一个字符，2表示第二个字符，依次类推；位置索引也可以为负值，-1表示最后一个字符，-2表示倒数第二个字符，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*027*/ {"字符个数", "如果本参数被省略，默认为从参数“起始字符位置”开始到文本结尾的字符个数。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*028*/ {"文件名", "请提供包含完整路径的文本名称。", 0, 0, SDT_TEXT, 0, NULL},

    /*029*/ {"缓冲区大小", "应大于零并小于“最大可用内存”。如果本参数被省略，默认为当前文本长度。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*030*/ {"最小可用内存", "单位为字节。如果本参数被省略，则保持现有“最小可用内存”数值不变。其内部初始值为10240字节，约10K。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*031*/ {"最大可用内存", "单位为字节。如果本参数被省略，则保持现有“最大可用内存”数值不变。其内部初始值为2147483647字节，约2G。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*032*/ {"内存增量", "单位为字节。如果此值过小，可能导致数据操作效率降低；如果此值过大，可能导致消耗较多内存。内部初始值为10240字节，约10K。", 0, 0, SDT_INT, 0, NULL},

    /*033*/ {"起始位置", "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*034*/ {"字节数", "如果本参数被省略，默认为从“起始位置”到字节集结尾的字节数。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*035*/ {"插入位置", "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。", 0, 0, SDT_INT, 0, NULL},
    /*036*/ {"欲插入的数据", "参数类型支持各种基本数据类型及其数组数据。", 0, 0, _SDT_ALL, 0, AS_RECEIVE_ALL_TYPE_DATA},

    /*037*/ {"起始删除位置", "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。", 0, 0, SDT_INT, 0, NULL},
    /*038*/ {"欲删除的字节数", NULL, 0, 0, SDT_INT, 0, NULL},

    /*039*/ {"位置索引", "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。", 0, 0, SDT_INT, 0, NULL},

    /*040*/ {"欲寻找的数据", "参数类型支持各种基本数据类型。", 0, 0, _SDT_ALL, 0, NULL},
    /*041*/ {"起始寻找位置", "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*042*/ {"欲寻找的数据", "参数类型支持各种基本数据类型。", 0, 0, _SDT_ALL, 0, NULL},
    /*043*/ {"起始寻找位置", "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。默认为-1，即从数据结尾开始向前寻找。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*044*/ {"起始替换位置", "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。", 0, 0, SDT_INT, 0, NULL},
    /*045*/ {"替换长度", NULL, 0, 0, SDT_INT, 0, NULL},
    /*046*/ {"替换为字节集", "参数类型支持各种基本数据类型。如果本参数被省略，则删除字节集中的指定部分。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*047*/ {"被替换数据", "参数类型支持各种基本数据类型。", 0, 0, _SDT_ALL, 0, NULL},
    /*048*/ {"替换为数据", "参数类型支持各种基本数据类型。如果本参数被省略，则表示将删除被替换数据。", 0, 0, _SDT_ALL, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*049*/ {"起始替换位置", "位置索引可以为正值，1表示第一个字节，2表示第二个字节，依次类推；位置索引也可以为负值，-1表示最后一个字节，-2表示倒数第二个字节，依次类推。如果本参数被省略，默认为1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*050*/ {"最大替换次数", "如果本参数被省略，或值为-1，表示不限制替换次数。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*051*/ {"用作分隔的字节集", "如果被省略，则默认使用字节 0 作为分隔符。如果是一个长度为零的文本，则返回的数组仅包含一个成员，其内容是本对象中的文本。", 0, 0, SDT_BIN, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*052*/ {"要返回的子文本数目", "如果为-1，表示不限制替换次数。如果本参数被省略，默认为-1。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*053*/ {"缓冲区大小", "应大于零并小于“最大可用内存”。如果本参数被省略，默认为当前数据长度。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*054*/ {"数据长度", "经外界修改后的数据长度。", 0, 0, SDT_INT, 0, NULL},

    /*055*/ {"最小可用内存", "单位为字节。如果本参数被省略，则保持现有“最小可用内存”数值不变。其内部初始值为10240字节，约10K。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*056*/ {"最大可用内存", "单位为字节。如果本参数被省略，则保持现有“最大可用内存”数值不变。其内部初始值为2147483647字节，约2G。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_commobj_global_var) / sizeof(g_argumentInfo_commobj_global_var[0]);
#endif


#define COMMOBJ_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_commobj_global_var[] = 
{
    COMMOBJ_DEF(COMMOBJ_DEF_CMDINFO)
};

int g_cmdInfo_commobj_global_var_count = sizeof(g_cmdInfo_commobj_global_var) / sizeof(g_cmdInfo_commobj_global_var[0]);

#endif

