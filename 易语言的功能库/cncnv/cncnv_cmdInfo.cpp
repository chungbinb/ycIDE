#if !defined(__E_STATIC_LIB)
#include "include_cncnv_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_cncnv_global_var[] = 
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

    /*000*/ {"待转换文本", "本参数用作提供需要转换的文本", 0, 0, SDT_TEXT, 0, NULL},
    /*001*/ {"转换方式", "可以为以下常量值之一： 1、#GBK到BIG5； 2、#BIG5到GBK； 3、#GBK繁体到简体； 4、#GBK简体到繁体； 5、#BIG5到GBK简体； 6、#GBK到SJIS； 7、#SJIS到GBK", 0, 0, SDT_INT, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_cncnv_global_var) / sizeof(g_argumentInfo_cncnv_global_var[0]);
#endif


#define CNCNV_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_cncnv_global_var[] = 
{
    CNCNV_DEF(CNCNV_DEF_CMDINFO)
};

int g_cmdInfo_cncnv_global_var_count = sizeof(g_cmdInfo_cncnv_global_var) / sizeof(g_cmdInfo_cncnv_global_var[0]);

#endif

