#if !defined(__E_STATIC_LIB)
#include "include_bmpoperate_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_bmpoperate_global_var[] = 
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



    /*000*/ {"源对象", NULL, 0, 0, MAKELONG(0x01, 0), 0, NULL},

    /*001*/ {"位图文件名", NULL, 0, 0, SDT_TEXT, 0, NULL},

    /*002*/ {"位图文件数据", NULL, 0, 0, SDT_BIN, 0, NULL},

    /*003*/ {"宽度", "位图的实际宽度", 0, 0, SDT_INT, 0, NULL},
    /*004*/ {"高度", "位图的实际高度", 0, 0, SDT_INT, 0, NULL},
    /*005*/ {"位深度", "位图的颜色位深度。参看“取位深度”命令", 0, 0, SDT_SHORT, 0, NULL},
    /*006*/ {"初始颜色", "位图的初始颜色。如果本参数被省略，则新建的位图内包含的颜色数据不会被初始化，这样可以加快创建的速度，但是在使用本位图前可能需要手动进行必要的初始化。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*007*/ {"横坐标", "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败", 0, 0, SDT_INT, 0, NULL},
    /*008*/ {"纵坐标", "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败", 0, 0, SDT_INT, 0, NULL},

    /*009*/ {"横坐标", "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败", 0, 0, SDT_INT, 0, NULL},
    /*010*/ {"纵坐标", "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败", 0, 0, SDT_INT, 0, NULL},

    /*011*/ {"横坐标", "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败", 0, 0, SDT_INT, 0, NULL},
    /*012*/ {"纵坐标", "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败", 0, 0, SDT_INT, 0, NULL},
    /*013*/ {"欲置入的值", NULL, 0, 0, SDT_INT, 0, NULL},

    /*014*/ {"横坐标", "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败", 0, 0, SDT_INT, 0, NULL},
    /*015*/ {"纵坐标", "以像素为单位。如果该参数指定的坐标超出了位图的实际范围，本命令将失败", 0, 0, SDT_INT, 0, NULL},
    /*016*/ {"欲置入的值", NULL, 0, 0, SDT_INT, 0, NULL},

    /*017*/ {"欲置入的颜色表数据", NULL, 0, 0, SDT_INT, 0, AS_RECEIVE_ARRAY_DATA},

    /*018*/ {"欲置入的点阵数据", NULL, 0, 0, SDT_BIN, 0, NULL},

    /*019*/ {"欲转换为的位深度", "本参数的有效值请参看本数据类型的“取位深度”命令", 0, 0, SDT_SHORT, 0, NULL},

    /*020*/ {"欲复制区域的左边", NULL, 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
    /*021*/ {"欲复制区域的顶边", NULL, 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
    /*022*/ {"欲复制区域的宽度", "如果本参数省略，默认值为源位图宽度", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*023*/ {"欲复制区域的高度", "如果本参数省略，默认值为源位图高度", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*024*/ {"复制到的目的位图", "如果本参数省略，则复制到源位图对象。注意：如果是复制到自身，且欲复制区域和复制到的区域有重叠部分，可能出现重叠复制。", 0, 0, MAKELONG(0x01, 0), 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*025*/ {"欲复制到位置左边", "注意：复制到的矩形不能超出目的位图的区域，否则本命令调用失败", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
    /*026*/ {"欲复制到位置顶边", "注意：复制到的矩形不能超出目的位图的区域，否则本命令调用失败", 0, 0, SDT_INT, 0, AS_HAS_DEFAULT_VALUE},
    /*027*/ {"透明色", "透明色为源位图在复制过程中将被忽略的颜色。如果本参数省略，不使用透明色。", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*028*/ {"位图信息头指针", "该参数接收一个指向“位图信息头(BITMAPINFOHEADER)”结构的指针，也是指向BITMAPINFO结构的指针", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},
    /*029*/ {"像素点阵数据指针", "该参数接收一个指向位图实际像素点阵数据的指针。", 0, 0, SDT_INT, 0, AS_RECEIVE_VAR},

    /*030*/ {"是否顺时针旋转", "真，顺时针方向旋转；假，逆时针", 0, 0, SDT_BOOL, 1, AS_HAS_DEFAULT_VALUE},

    /*031*/ {"是否水平镜像", NULL, 0, 0, SDT_BOOL, 0, NULL},
    /*032*/ {"是否垂直镜像", NULL, 0, 0, SDT_BOOL, 0, NULL},

    /*033*/ {"窗口句柄", "所创建出来的位图将和该窗口的设备环境（DC）兼容。如果本参数省略，默认使用桌面窗口句柄", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*034*/ {"位图句柄", "一个位图句柄（HBITMAP）", 0, 0, SDT_INT, 0, NULL},

    /*035*/ {"旋转角度", "正值代表正时针旋转，负值代表逆时针旋转", 0, 0, SDT_FLOAT, 0, NULL},
    /*036*/ {"背景填充颜色", "如果位图经过旋转后尺寸变大，新增的部分将用该颜色填充", 0, 0, SDT_INT, 0, NULL},

    /*037*/ {"分辨率", "单位为“像素/米”", 0, 0, SDT_INT, 0, NULL},

    /*038*/ {"位图文件名", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*039*/ {"DC句柄", NULL, 0, 0, SDT_INT, 0, NULL},

    /*040*/ {"DC句柄", NULL, 0, 0, SDT_INT, 0, NULL},
    /*041*/ {"原始位图数据", "如果本参数指定的不是有效的位图数据，将转换失败甚至造成程序崩溃", 0, 0, SDT_BIN, 0, NULL},
    /*042*/ {"目的位深度", "参看“位图”对象的相关方法", 0, 0, SDT_BYTE, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_bmpoperate_global_var) / sizeof(g_argumentInfo_bmpoperate_global_var[0]);
#endif


#define BMPOPERATE_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_bmpoperate_global_var[] = 
{
    BMPOPERATE_DEF(BMPOPERATE_DEF_CMDINFO)
};

int g_cmdInfo_bmpoperate_global_var_count = sizeof(g_cmdInfo_bmpoperate_global_var) / sizeof(g_cmdInfo_bmpoperate_global_var[0]);

#endif

