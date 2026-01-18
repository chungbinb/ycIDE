#if !defined(__E_STATIC_LIB)
#include "include_eimgconverter_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_eimgconverter_global_var[] = 
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

    /*000*/ {"源文件名", "可以是本支持库支持的任意文件格式，用文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*001*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},
    /*002*/ {"灰度还是真彩", "参数值等于真为8位灰度图，等于假为24位真彩图，默认值为假", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},
    /*003*/ {"图像品质", "参数值在 1 到 100 之间，否则为错！默认为 75 ", 0, 0, SDT_INT, 75, AS_HAS_DEFAULT_VALUE},
    /*004*/ {"是否使用优化编码", "同等质量的JPEG图像文件，优化编码是压缩比最好的，默认值为真", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},
    /*005*/ {"是否柔化", "参数值在 0 到 100 之间，否则为错！0 为无柔化。在压缩比很大的JPEG文件中，适当的柔化可以改善图像质量。默认值为 0 ", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},

    /*006*/ {"源文件名", "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*007*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},

    /*008*/ {"源文件名", "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*009*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},
    /*010*/ {"灰度还是真彩", "参数值等于真为8位灰度图，等于假为24位真彩图，默认值为假", 0, 0, SDT_BOOL, 0, AS_HAS_DEFAULT_VALUE},

    /*011*/ {"源文件名", "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*012*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},
    /*013*/ {"目标文件的颜色位数", "该参数目前只能为数值 24 (目前仅支持 24 位颜色)", 0, 0, SDT_INT, 24, AS_HAS_DEFAULT_VALUE},

    /*014*/ {"源文件名", "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*015*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},
    /*016*/ {"保存模式", "参数值为真则保存为二进制模式，为假则保存为文本模式，默认参数值为真", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},

    /*017*/ {"源文件名", "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*018*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},
    /*019*/ {"保存模式", "参数值为真则保存为二进制模式，为假则保存为文本模式，默认参数值为真", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},

    /*020*/ {"源文件名", "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*021*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},

    /*022*/ {"源文件名", "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*023*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},

    /*024*/ {"源文件名", "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*025*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},

    /*026*/ {"源文件名", "可以是本支持库支持的任意文件格式，请提供文件全名(包含路径)", 0, 0, SDT_TEXT, 0, NULL},
    /*027*/ {"目标文件名", "参数值指定转换后保存到的目标文件名", 0, 0, SDT_TEXT, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_eimgconverter_global_var) / sizeof(g_argumentInfo_eimgconverter_global_var[0]);
#endif


#define EIMGCONVERTER_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_eimgconverter_global_var[] = 
{
    EIMGCONVERTER_DEF(EIMGCONVERTER_DEF_CMDINFO)
};

int g_cmdInfo_eimgconverter_global_var_count = sizeof(g_cmdInfo_eimgconverter_global_var) / sizeof(g_cmdInfo_eimgconverter_global_var[0]);

#endif

