#if !defined(__E_STATIC_LIB)
#include "include_cnvpe_header.h"

//TODO 静态库需要的部分,会记录所有的函数名到数组里,静态编译的时候需要取回命令名
static ARG_INFO g_argumentInfo_cnvpe_global_var[] = 
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

    /*000*/ {"可执行文件数据", "提供需要转换的PE格式可执行文件数据，可以是EXE或DLL文件内容", 0, 0, SDT_BIN, 0, NULL},
    /*001*/ {"转换码", "可以是非零的任何数值，使用不同的数值将得到完全不同的转换结果，使用相同的数值将得到完全相同的转换结果。如果提供数值0，将自动使用随机数值。如果本参数被省略，默认值为0", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY},
    /*002*/ {"转换结果", "通过为本参数提供一个变量即可获取命令运行后具体的转换是否成功状态，所返回值有以下几种：0:转换成功; -1:所提供数据无效; -2:数据中没有需要转换的部分; -3:无法找到转换代码插入空间。如果本参数被省略，将不返回转换结果", 0, 0, SDT_INT, 0, AS_DEFAULT_VALUE_IS_EMPTY | AS_RECEIVE_VAR},

};

#ifdef _DEBUG    // 这里是为了确认参数序号是否正确, 成员数比最后一个序号大1就是正确
const int dbg_cmd_arg_count__ = sizeof(g_argumentInfo_cnvpe_global_var) / sizeof(g_argumentInfo_cnvpe_global_var[0]);
#endif


#define CNVPE_DEF_CMDINFO(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    { _szName, ______E_FNENAME(_szEgName), _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo },


// 易语言每个命令的定义, 会把信息显示在支持库列表里, 这里每个成员都是详细的描述一个命令的信息
CMD_INFO g_cmdInfo_cnvpe_global_var[] = 
{
    CNVPE_DEF(CNVPE_DEF_CMDINFO)
};

int g_cmdInfo_cnvpe_global_var_count = sizeof(g_cmdInfo_cnvpe_global_var) / sizeof(g_cmdInfo_cnvpe_global_var[0]);

#endif

