#include "include_isapi_header.h"
#include "elib/fnshare.h"
#include "elib/lang.h"
#pragma comment(lib, "legacy_stdio_definitions.lib")    // 加这一行是为了不修改link.ini配置就能用2017链接器编译

EXTERN_C INT WINAPI isapi_ProcessNotifyLib_isapi(INT nMsg, DWORD dwParam1, DWORD dwParam2);
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

#define ISAPI_DEF_CMD_PTR(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    ISAPI_NAME(_index, _szEgName),

PFN_EXECUTE_CMD g_cmdInfo_isapi_global_var_fun[] = { ISAPI_DEF(ISAPI_DEF_CMD_PTR) };

static LIB_INFO g_LibInfo_isapi_global_var = 
{
    /*m_dwLibFormatVer*/        LIB_FORMAT_VER, // 库格式号, 应该等于LIB_FORMAT_VER
    /*m_szGuid*/                "0DD316AB105442f882C4B535F45E63CB", // 对应于本库的唯一GUID串, 不能为NULL或空, 库的所有版本此串都应相同
    /*m_nMajorVersion*/         3, // 本库的主版本号, 必须大于0
    /*m_nMinorVersion*/         1, // 本库的次版本号
    /*m_nBuildNumber*/          1, // 构建版本号, 无需对此版本号作任何处理

    /*m_nRqSysMajorVer*/        3, // 所需要的易语言系统的主版本号
    /*m_nRqSysMinorVer*/        7, // 所需要的易语言系统的次版本号
    /*m_nRqSysKrnlLibMajorVer*/ 3, // 所需要的系统核心支持库的主版本号
    /*m_nRqSysKrnlLibMinorVer*/ 7, // 所需要的系统核心支持库的次版本号

    /*m_szName*/                "互联网服务支持库", // 库名, 不能为NULL或空
    /*m_nLanguage*/             __GBK_LANG_VER, // 库所支持的语言
    /*m_szExplain*/             "本支持库实现了对互联网服务 IIS ISAPI Extension/Filter 的支持。数据类型“互联网扩展”用于辅助开发ISAPI扩展程序(Extension)，数据类型“互联网筛选器”用于辅助开发IIS筛选器(Filter)。依据微软ISAPI规范，扩展程序和筛选器都是DLL，且需要公开导出满足特定要求的函数，这些导出函数是提供给IIS供它调用的。\r\n    对于扩展程序，必须导出 GetExtensionVersion, HttpExtensionProc 这两个函数，根据需要还可以导出 TerminateExtension 函数：GetExtensionVersion在扩展程序被加载时被调用，有一个整数型参数“版本信息”，返回值为逻辑型表示是否初始化成功，把它的参数传给“互联网扩展.初始化版本信息()”即可完成扩展扩展程序的初始化；HttpExtensionProc在每次处理HTTP请求(Request)时被调用，有一个整数型参数“扩展程序上下文”，返回值为整数型表示处理结果（要求返回 #互联网扩展返回值.* 常量之一），把它的参数传给“互联网扩展.初始化互联网扩展()”之后即可通过互联网扩展对象执行各种操作；TerminateExtension如果存在，在扩展程序被卸载时被调用，有一个整数型参数“标记”，返回值为逻辑型。\r\n    对于筛选器，必须导出 GetFilterVersion, HttpFilterProc 这两个函数，根据需要还可以导出 TerminateFilter 函数：GetFilterVersion在筛选器被载入时调用，它有一个整数型参数“版本信息”，返回值为逻辑型表示是否初始化成功，把它的参数传给“互联网筛选器.初始化版本信息()”即可完成筛选器的初始化；HttpFilterProc在每次筛选器事件发生时被调用，它有三个整数型参数“筛选器上下文”“通知类型”“通知数据”，返回值为整数型表示处理结果（要求返回 #筛选器返回值.* 常量之一），把它的三个参数传给“互联网筛选器.初始化互联网筛选器()”之后就可以通过互联网筛选器对象执行各种操作；TerminateFilter如果存在，在筛选器被卸载时被调用，它有一个整数型参数“标记”(无实际意义)，返回值为逻辑型。", // 库详细解释
    /*m_dwState*/               _LIB_OS(__OS_WIN), // _LIB_OS() | LBS_ 开头常量, 如果是插件,必须包含 LBS_IDE_PLUGIN

    /*m_szAuthor*/              "大有吴涛易语言软件公司", // 作者
    /*m_szZipCode*/             "116001", // 邮编
    /*m_szAddress*/             "辽宁省大连市中山区人民路55号亚太国际金融中心", // 地址
    /*m_szPhoto*/               "+86(0411)39895831", // 联系电话
    /*m_szFax*/                 "+86(0411)39895834", // 传真
    /*m_szEmail*/               "service@dywt.com.cn", // 邮箱
    /*m_szHomePage*/            "http://www.eyuyan.com", // 主页
    /*m_szOther*/               "祝您一帆风顺，心想事成！", // 其他信息

    /*m_nDataTypeCount*/        g_DataType_isapi_global_var_count, // 本库中自定义数据类型的数目
    /*g_DataType_web*/          g_DataType_isapi_global_var, // 本库中所有的自定义数据类型

    /*m_nCategoryCount*/        0, // 全局命令类别数目, 可为0
    /*m_szzCategory*/           NULL, // 全局命令类别数目, 可为0

    /*m_nCmdCount*/             g_cmdInfo_isapi_global_var_count, // 本库中提供的所有命令(全局命令及对象命令)的数目, 可为0
    /*m_pBeginCmdInfo*/         g_cmdInfo_isapi_global_var, // 本库中提供的所有命令
    /*m_pCmdsFunc*/             g_cmdInfo_isapi_global_var_fun, // 指向每个命令的实现代码首地址, 可为NULL

    /*m_pfnRunAddInFn*/         NULL, // 可为NULL,易语言IDE系统添加辅助功能, 所提供功能会被自动添加到IDE的“工具”菜单中
                                         // 有关AddIn功能的说明, 两个字符串说明一个功能。第一个为功能名称
                                         // (仅限一行20字符, 如果希望自行初始位置而不被自动加入到工具菜单, 则
                                         // 名称应该以@开始, 此时会接收到值为 -(nAddInFnIndex + 1) 的调用通知), 
                                         // 第二个为功能详细介绍(仅限一行60字符), 最后由两个空串结束。
    /*m_szzAddInFnInfo*/        NULL, // 功能详细介绍(仅限一行60字符), 最后由两个空串结束

    /*m_pfnNotify*/             isapi_ProcessNotifyLib_isapi, // 不能为NULL,和易语言通讯的子程序

    /*m_pfnSuperTemplate*/      NULL, // 模板 可为NULL
                                         //  有关SuperTemplate的说明, 两个字符串说明一个SuperTemplate。
                                         //  第一个为SuperTemplate名称(仅限一行30字符), 第二个为详细介绍(不限), 
                                         //  最后由两个空串结束。
    /*m_szzSuperTemplateInfo*/  NULL, // 模板 可为NULL

    /*m_nLibConstCount*/        g_ConstInfo_isapi_global_var_count, // 本库预先定义的所有常量数量
    /*m_pLibConst*/             g_ConstInfo_isapi_global_var, // 本库预先定义的所有常量

    /*m_szzDependFiles*/        NULL // 可为NULL, 本库正常运行所需要依赖的其他支持文件
};

EXTERN_C PLIB_INFO WINAPI GetNewInf()
{
    return &g_LibInfo_isapi_global_var;
}

#define ISAPI_DEF_CMDNAME_STR(_index, _szName, _szEgName, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    ______E_FNENAME(ISAPI_NAME(_index, _szEgName)),

// 函数名, 这个给静态编译使用
static const LPCSTR g_cmdNamesisapi[] = { ISAPI_DEF(ISAPI_DEF_CMDNAME_STR) };

#endif
EXTERN_C INT WINAPI isapi_ProcessNotifyLib_isapi(INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
    int nRet = NR_OK;
    switch (nMsg)
    {
#ifndef __E_STATIC_LIB
    // 返回所有命令实现函数的的函数名称数组(char*[]), 支持静态编译的动态库必须处理
    case NL_GET_CMD_FUNC_NAMES:
    {
        return (INT)g_cmdNamesisapi;
    }
    // 返回处理系统通知的函数名称(PFN_NOTIFY_LIB函数名称), 支持静态编译的动态库必须处理
    case NL_GET_NOTIFY_LIB_FUNC_NAME:
    {
        return (INT)"isapi_ProcessNotifyLib_isapi";
    }
    // 返回静态库所依赖的其它静态库文件名列表(格式为\0分隔的文本,结尾两个\0), 支持静态编译的动态库必须处理
    // kernel32.lib user32.lib gdi32.lib 等常用的系统库不需要放在此列表中
    // 返回NULL或NR_ERR表示不指定依赖文件
    case NL_GET_DEPENDENT_LIBS:
    {
        return (INT)"\0\0";
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


