#include "include_spec_header.h"
#include "elib/fnshare.h"
#include "elib/lang.h"
#pragma comment(lib, "legacy_stdio_definitions.lib")

// 特殊功能支持库主入口
// 支持 Unicode 和 x64

EXTERN_C INT_PTR WINAPI spec_ProcessNotifyLib_spec(INT nMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
#ifndef __E_STATIC_LIB
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:    // 第一次被加载,LoadLibrary加载就触发
        break;
    case DLL_PROCESS_DETACH:    // DLL被释放
        break;
    case DLL_THREAD_ATTACH:     // 线程映射
        break;
    case DLL_THREAD_DETACH:     // 线程卸载
        break;
    }
    return TRUE;
}

#define SPEC_DEF_CMD_PTR(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    SPEC_NAME(_index, _szEgName),

PFN_EXECUTE_CMD g_cmdInfo_spec_global_var_fun[] = { SPEC_DEF(SPEC_DEF_CMD_PTR) };

static LIB_INFO g_LibInfo_spec_global_var = 
{
    /*m_dwLibFormatVer*/        LIB_FORMAT_VER, // 库格式号, 应该等于LIB_FORMAT_VER
    /*m_szGuid*/                "A512548E76954B6E92C21055517615B0", // 对应于本库的唯一GUID串, 不能为NULL或空, 库的所有版本此串都应相同
    /*m_nMajorVersion*/         3, // 本库的主版本号, 必须大于0
    /*m_nMinorVersion*/         1, // 本库的次版本号
    /*m_nBuildNumber*/          0, // 构建版本号, 无需对此版本号作任何处理

    /*m_nRqSysMajorVer*/        3, // 所需要的易语言系统的主版本号
    /*m_nRqSysMinorVer*/        8, // 所需要的易语言系统的次版本号
    /*m_nRqSysKrnlLibMajorVer*/ 3, // 所需要的系统核心支持库的主版本号
    /*m_nRqSysKrnlLibMinorVer*/ 0, // 所需要的系统核心支持库的次版本号

    /*m_szName*/                "特殊功能支持库", // 库名, 不能为NULL或空
    /*m_nLanguage*/             __GBK_LANG_VER, // 库所支持的语言
    /*m_szExplain*/             "本支持库提供部分相对特殊的命令，供易语言高级用户使用。支持Unicode和x64。", // 库详细解释
    /*m_dwState*/               _LIB_OS(OS_ALL), // _LIB_OS() | LBS_ 开头常量, 如果是插件,必须包含 LBS_IDE_PLUGIN

    /*m_szAuthor*/              "大连大有吴涛易语言软件开发有限公司", // 作者
    /*m_szZipCode*/             "116001", // 邮编
    /*m_szAddress*/             "辽宁省大连市中山区人民路55号亚太国际金融中心", // 地址
    /*m_szPhoto*/               "+86(0411)39895831", // 联系电话
    /*m_szFax*/                 "+86(0411)39895834", // 传真
    /*m_szEmail*/               "service@dywt.com.cn", // 邮箱
    /*m_szHomePage*/            "http://www.eyuyan.com", // 主页
    /*m_szOther*/               "支持Unicode和x64位重编译版本", // 其他信息

    /*m_nDataTypeCount*/        g_DataType_spec_global_var_count, // 本库中自定义数据类型的数目
    /*g_DataType_web*/          g_DataType_spec_global_var, // 本库中所有的自定义数据类型

    /*m_nCategoryCount*/        4, // 全局命令类别数目, 可为0
    /*m_szzCategory*/           "0000系统处理\0""0000程序调试\0""0000其他\0""0000附加\0""\0", // 全局命令类别数目, 可为0

    /*m_nCmdCount*/             g_cmdInfo_spec_global_var_count, // 本库中提供的所有命令(全局命令及对象命令)的数目, 可为0
    /*m_pBeginCmdInfo*/         g_cmdInfo_spec_global_var, // 本库中提供的所有命令
    /*m_pCmdsFunc*/             g_cmdInfo_spec_global_var_fun, // 指向每个命令的实现代码首地址, 可为NULL

    /*m_pfnRunAddInFn*/         NULL, // 可为NULL,易语言IDE系统添加辅助功能, 所提供功能会被自动添加到IDE的“工具”菜单中
                                         // 有关AddIn功能的说明, 两个字符串说明一个功能。第一个为功能名称
                                         // (仅限一行20字符, 如果希望自行初始位置而不被自动加入到工具菜单, 则
                                         // 名称应该以@开始, 此时会接收到值为 -(nAddInFnIndex + 1) 的调用通知), 
                                         // 第二个为功能详细介绍(仅限一行60字符), 最后由两个空串结束。
    /*m_szzAddInFnInfo*/        NULL, // 功能详细介绍(仅限一行60字符), 最后由两个空串结束

    /*m_pfnNotify*/             spec_ProcessNotifyLib_spec, // 不能为NULL,和易语言通讯的子程序

    /*m_pfnSuperTemplate*/      NULL, // 模板 可为NULL
                                         //  有关SuperTemplate的说明, 两个字符串说明一个SuperTemplate。
                                         //  第一个为SuperTemplate名称(仅限一行30字符), 第二个为详细介绍(不限), 
                                         //  最后由两个空串结束。
    /*m_szzSuperTemplateInfo*/  NULL, // 模板 可为NULL

    /*m_nLibConstCount*/        g_ConstInfo_spec_global_var_count, // 本库预先定义的所有常量数量
    /*m_pLibConst*/             g_ConstInfo_spec_global_var, // 本库预先定义的所有常量

    /*m_szzDependFiles*/        NULL // 可为NULL, 本库正常运行所需要依赖的其他支持文件
};

EXTERN_C PLIB_INFO WINAPI GetNewInf()
{
    return &g_LibInfo_spec_global_var;
}

#define SPEC_DEF_CMDNAME_STR(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    ______E_FNENAME(SPEC_NAME(_index, _szEgName)),

// 函数名, 这个给静态编译使用
static const LPCSTR g_cmdNamesspec[] = { SPEC_DEF(SPEC_DEF_CMDNAME_STR) };

#endif
EXTERN_C INT_PTR WINAPI spec_ProcessNotifyLib_spec(INT nMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    INT_PTR nRet = NR_OK;
    switch (nMsg)
    {
#ifndef __E_STATIC_LIB
    // 返回所有命令实现函数的的函数名称数组(char*[]), 支持静态编译的动态库必须处理
    case NL_GET_CMD_FUNC_NAMES:
    {
        return (INT_PTR)g_cmdNamesspec;
    }
    // 返回处理系统通知的函数名称(PFN_NOTIFY_LIB函数名称), 支持静态编译的动态库必须处理
    case NL_GET_NOTIFY_LIB_FUNC_NAME:
    {
        return (INT_PTR)"spec_ProcessNotifyLib_spec";
    }
    // 返回静态库所依赖的其它静态库文件名列表(格式为\0分隔的文本,结尾两个\0), 支持静态编译的动态库必须处理
    // kernel32.lib user32.lib gdi32.lib 等常用的系统库不需要放在此列表中
    // 返回NULL或NR_ERR表示不指定依赖文件
    case NL_GET_DEPENDENT_LIBS:
    {
        return (INT_PTR)"\0\0";
    }
#endif
    // 告知库通知系统用的函数指针,在装载支持库前通知,可能有多次,后通知的值应该覆盖前面所通知的值,忽略返回值
    // 库可将此函数指针记录下来以便在需要时使用它通知信息到系统
    // dwParam1: (PFN_NOTIFY_SYS)
    case NL_SYS_NOTIFY_FUNCTION:
    {
        // 把初始化的事件转发到 fnshare.cpp 里执行, 让fnshare里的函数可以使用
        ProcessNotifyLib(nMsg, dwParam1, dwParam2);
        break;
    }
    // 通知支持库释放资源准备退出及释放指定的附加数据
    case NL_FREE_LIB_DATA:
    {
        break;
    }
    // 当支持库在IDE中被用户取消选择后发送本通知
    case NL_UNLOAD_FROM_IDE:
    {
        break;
    }
    // 返回NR_DELAY_FREE表示本支持库需要被延迟释放
    case NR_DELAY_FREE:
    {
        break;
    }
    // 当IDE已经准备好接受用户操作后,此通知被发送到所有具有LBS_IDE_PLUGIN标志的支持库
    // 注意: 当用户在支持库配置对话框中取消已选择支持库或选择新的支持库后,本通知会被再次发送,支持库内需要处理好此种情况
    case NL_IDE_READY:
    {
        break;
    }
    // 当用户在IDE中单击鼠标右键即将弹出菜单前,此通知被发送到所有具有LBS_IDE_PLUGIN标志的支持库
    // dwParam1: 欲弹出菜单句柄(HMENU)
    // dwParam2: 欲弹出菜单在易语言主程序(e.exe)中的资源ID
    case NL_RIGHT_POPUP_MENU_SHOW:
    {
        break;
    }
    // 当IDE准备显示新加入成员对话框之前,此通知被发送到所有具有LBS_IDE_PLUGIN标志的支持库
    // 返回1表示该通知被处理且IDE根据返回内容添加所指定的新成员,返回2表示该通知被处理且IDE中止添加新成员,返回0表示未被处理
    // dwParam1: 提供欲加入成员的类型(ADD_NEW_ELEMENT_TYPE)
    // dwParam2:
    //   如果dwParam1为NAN_NEW_VAR,则为一个指向RES_NEW_VAR_INFO的指针;
    //   如果dwParam1为NAN_NEW_SUB,则为一个指向欲加入子程序名称的文本指针;
    //   如果dwParam1为NAN_NEW_CONST_RES,则为一个指向RES_NEW_CONST_RES_INFO的指针
    case NL_ADD_NEW_ELEMENT:
    {
        break;
    }
    default:
        nRet = NR_ERR;
        break;
    }
    return nRet;
}


