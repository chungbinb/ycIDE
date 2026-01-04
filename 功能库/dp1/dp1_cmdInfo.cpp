#if !defined(__E_STATIC_LIB)
#include "include_dp1_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_dp1_global_var[] = 
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

    /*000*/ {"字节集数据", "为命令提供所需的字节集数据", 0, 0, SDT_BIN, 0, NULL},

    /*001*/ {"字节集数据", "为命令提供所需的字节集数据", 0, 0, SDT_BIN, 0, NULL},
    /*002*/ {"密码文本", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*003*/ {"加密算法", "指定具体使用的加密算法，可以为以下常量值之一：1: #DES算法； 2: #RC4算法。加密和解密必须使用相同的算法，有关算法的具体说明请参阅有关文献。如果本参数被省略，则默认值为1，即DES算法", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*004*/ {"字节集数据", "为命令提供所需的字节集数据", 0, 0, SDT_BIN, 0, NULL},
    /*005*/ {"密码文本", NULL, 0, 0, SDT_TEXT, 0, NULL},
    /*006*/ {"加密算法", "指定具体使用的加密算法，可以为以下常量值之一：1: #DES算法； 2: #RC4算法。加密和解密必须使用相同的算法，有关算法的具体说明请参阅有关文献。如果本参数被省略，则默认值为1，即DES算法", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},

    /*007*/ {"待签署字节集数据", NULL, 0, 0, SDT_BIN, 0, NULL},
    /*008*/ {"签署者私钥文本", "为十六进制数值文本", 0, 0, SDT_TEXT, 0, NULL},
    /*009*/ {"签署者公共模数", "为十六进制数值文本", 0, 0, SDT_TEXT, 0, NULL},

    /*010*/ {"待验证被签署字节集", NULL, 0, 0, SDT_BIN, 0, NULL},
    /*011*/ {"签署结果文本", "必须为“数字签名”命令所返回的结果", 0, 0, SDT_TEXT, 0, NULL},
    /*012*/ {"签署者公钥文本", "为十六进制数值文本", 0, 0, SDT_TEXT, 0, NULL},
    /*013*/ {"签署者公共模数", "为十六进制数值文本", 0, 0, SDT_TEXT, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_dp1_global_var) / sizeof(g_argumentInfo_dp1_global_var[0]);
#endif


#define DP1_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_dp1_global_var[] = 
{
    DP1_DEF(DP1_DEF_CMDINFO)
};

int g_cmdInfo_dp1_global_var_count = sizeof(g_cmdInfo_dp1_global_var) / sizeof(g_cmdInfo_dp1_global_var[0]);

#endif

