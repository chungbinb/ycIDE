#if !defined(__E_STATIC_LIB)
#include "include_portio_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_portio_global_var[] = 
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

    /*000*/ {"端口号", "本参数提供欲进行读写端口的端口号", 0, 0, SDT_INT, 0, NULL},

    /*001*/ {"端口号", "本参数提供欲进行读写端口的端口号", 0, 0, SDT_INT, 0, NULL},
    /*002*/ {"欲写出字节", "本参数提供欲写出到指定端口的字节数据", 0, 0, SDT_BYTE, 0, NULL},

    /*003*/ {"并口端口号", "本参数提供ECP类型并口的端口号", 0, 0, SDT_INT, 0, NULL},
    /*004*/ {"ECP模式", "本参数可以为以下值之一:  1: SPP;  2: PS2;  3: FastCentronics;  4: ECP;  5: EPP;  6: Test;  7: Configuration", 0, 0, SDT_INT, 0, NULL},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_portio_global_var) / sizeof(g_argumentInfo_portio_global_var[0]);
#endif


#define PORTIO_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_portio_global_var[] = 
{
    PORTIO_DEF(PORTIO_DEF_CMDINFO)
};

int g_cmdInfo_portio_global_var_count = sizeof(g_cmdInfo_portio_global_var) / sizeof(g_cmdInfo_portio_global_var[0]);

#endif

