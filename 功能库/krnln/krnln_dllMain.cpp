#include "include_krnln_header.h"
#include "elib/fnshare.h"
#include "elib/lang.h"
#pragma comment(lib, "legacy_stdio_definitions.lib")

// 系统核心支持库主入口
// 自动生成于 2026-01-04 15:09:00

EXTERN_C INT_PTR WINAPI krnln_ProcessNotifyLib(INT nMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

#ifndef __E_STATIC_LIB
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}

#define KRNLN_DEF_CMD_PTR(_index, _szName, _szEgName, _szFuncId, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    KRNLN_NAME(_index, _szFuncId),

PFN_EXECUTE_CMD g_cmdInfo_krnln_global_var_fun[] = { KRNLN_DEF(KRNLN_DEF_CMD_PTR) };

static LIB_INFO g_LibInfo_krnln_global_var = 
{
    /*m_dwLibFormatVer*/        LIB_FORMAT_VER,
    /*m_szGuid*/                "D09F2340818511D396F6AE4C17150413",  // 系统核心支持库GUID
    /*m_nMajorVersion*/         5,
    /*m_nMinorVersion*/         7,
    /*m_nBuildNumber*/          0,

    /*m_nRqSysMajorVer*/        3,
    /*m_nRqSysMinorVer*/        7,
    /*m_nRqSysKrnlLibMajorVer*/ 3,
    /*m_nRqSysKrnlLibMinorVer*/ 0,

    /*m_szName*/                "系统核心支持库",
    /*m_nLanguage*/             __GBK_LANG_VER,
    /*m_szExplain*/             "本支持库是易语言的核心库，为系统本身和每个易程序提供必需的功能支持。支持Unicode和x64。",
    /*m_dwState*/               _LIB_OS(OS_ALL),

    /*m_szAuthor*/              "大有吴涛易语言软件公司",
    /*m_szZipCode*/             "116001",
    /*m_szAddress*/             "辽宁省大连市中山区人民路55号亚太国际金融中心",
    /*m_szPhoto*/               "+86(0411)39895831",
    /*m_szFax*/                 "+86(0411)39895834",
    /*m_szEmail*/               "service@dywt.com.cn",
    /*m_szHomePage*/            "http://www.eyuyan.com",
    /*m_szOther*/               "支持Unicode和x64位重编译版本",

    /*m_nDataTypeCount*/        g_DataType_krnln_global_var_count,
    /*g_DataType_xxx*/          g_DataType_krnln_global_var,

    /*m_nCategoryCount*/        22,
    /*m_szzCategory*/           "0000流程控制\0""0000算术运算\0""0000逻辑比较\0""0000位运算\0""0000变量操作\0""0000数组操作\0""0000环境存取\0""0000拼音处理\0""0000文本操作\0""0000时间操作\0""0000数值转换\0""0000字节集操作\0""0000磁盘操作\0""0000文件读写\0""0000系统处理\0""0000媒体播放\0""0000程序调试\0""0000其他\0""0000数据库\0""0000网络通信\0""0000控制台操作\0""0000易包\0""\0",

    /*m_nCmdCount*/             g_cmdInfo_krnln_global_var_count,
    /*m_pBeginCmdInfo*/         g_cmdInfo_krnln_global_var,
    /*m_pCmdsFunc*/             g_cmdInfo_krnln_global_var_fun,

    /*m_pfnRunAddInFn*/         NULL,
    /*m_szzAddInFnInfo*/        NULL,

    /*m_pfnNotify*/             krnln_ProcessNotifyLib,

    /*m_pfnSuperTemplate*/      NULL,
    /*m_szzSuperTemplateInfo*/  NULL,

    /*m_nLibConstCount*/        g_ConstInfo_krnln_global_var_count,
    /*m_pLibConst*/             g_ConstInfo_krnln_global_var,

    /*m_szzDependFiles*/        NULL
};

EXTERN_C PLIB_INFO WINAPI GetNewInf()
{
    return &g_LibInfo_krnln_global_var;
}

#define KRNLN_DEF_CMDNAME_STR(_index, _szName, _szEgName, _szFuncId, _szExplain, _shtCategory, _wState, _dtRetValType, _wReserved, _shtUserLevel, _shtBitmapIndex, _shtBitmapCount, _nArgCount, _pBeginArgInfo) \
    ______E_FNENAME(KRNLN_NAME(_index, _szFuncId)),

static const LPCSTR g_cmdNamesKrnln[] = { KRNLN_DEF(KRNLN_DEF_CMDNAME_STR) };

#endif

EXTERN_C INT_PTR WINAPI krnln_ProcessNotifyLib(INT nMsg, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    INT_PTR nRet = NR_OK;
    switch (nMsg)
    {
#ifndef __E_STATIC_LIB
    case NL_GET_CMD_FUNC_NAMES:
        return (INT_PTR)g_cmdNamesKrnln;
    case NL_GET_NOTIFY_LIB_FUNC_NAME:
        return (INT_PTR)"krnln_ProcessNotifyLib";
    case NL_GET_DEPENDENT_LIBS:
        return (INT_PTR)"\0\0";
#endif
    case NL_SYS_NOTIFY_FUNCTION:
        ProcessNotifyLib(nMsg, dwParam1, dwParam2);
        break;
    case NL_FREE_LIB_DATA:
        break;
    case NL_UNLOAD_FROM_IDE:
        break;
    default:
        nRet = NR_ERR;
        break;
    }
    return nRet;
}
