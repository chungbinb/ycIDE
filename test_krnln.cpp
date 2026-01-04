#include <windows.h>
#include <iostream>
#include <string>

// 从 lib2.h 复制必要的定义
typedef struct {
    DWORD       m_dwLibFormatVer;
    const char* m_szGuid;
    int         m_nMajorVersion;
    int         m_nMinorVersion;
    int         m_nBuildNumber;
    int         m_nRqSysMajorVer;
    int         m_nRqSysMinorVer;
    int         m_nRqSysKrnlLibMajorVer;
    int         m_nRqSysKrnlLibMinorVer;
    const char* m_szName;
    int         m_nLanguage;
    const char* m_szExplain;
    DWORD       m_dwState;
    const char* m_szAuthor;
    const char* m_szZipCode;
    const char* m_szAddress;
    const char* m_szPhoto;
    const char* m_szFax;
    const char* m_szEmail;
    const char* m_szHomePage;
    const char* m_szOther;
    int         m_nDataTypeCount;
    void*       m_pDataType;
    int         m_nCategoryCount;
    const char* m_szzCategory;
    int         m_nCmdCount;
    void*       m_pBeginCmdInfo;
    void**      m_pCmdsFunc;
    void*       m_pfnRunAddInFn;
    const char* m_szzAddInFnInfo;
    void*       m_pfnNotify;
    void*       m_pfnSuperTemplate;
    const char* m_szzSuperTemplateInfo;
    int         m_nLibConstCount;
    void*       m_pLibConst;
    const char* m_szzDependFiles;
} LIB_INFO, * PLIB_INFO;

typedef PLIB_INFO(WINAPI* PFN_GET_LIB_INFO)();

int main() {
    std::wcout.imbue(std::locale("zh_CN.UTF-8"));
    
    // 加载 krnln.fne
    HMODULE hLib = LoadLibraryW(L"ycIDE\\x64\\Debug\\lib\\krnln.fne");
    if (!hLib) {
        DWORD err = GetLastError();
        std::wcout << L"LoadLibrary 失败，错误代码: " << err << std::endl;
        return 1;
    }
    
    std::wcout << L"LoadLibrary 成功！" << std::endl;
    
    // 尝试获取 GetNewInf
    PFN_GET_LIB_INFO pfnGetNewInf = (PFN_GET_LIB_INFO)GetProcAddress(hLib, "GetNewInf");
    if (!pfnGetNewInf) {
        // 尝试其他导出名称
        pfnGetNewInf = (PFN_GET_LIB_INFO)GetProcAddress(hLib, "_GetNewInf");
        if (!pfnGetNewInf) {
            pfnGetNewInf = (PFN_GET_LIB_INFO)GetProcAddress(hLib, "GetNewInf@0");
        }
    }
    
    if (!pfnGetNewInf) {
        std::wcout << L"GetProcAddress 失败，找不到 GetNewInf 导出" << std::endl;
        FreeLibrary(hLib);
        return 1;
    }
    
    std::wcout << L"GetProcAddress 成功！" << std::endl;
    
    // 调用 GetNewInf
    PLIB_INFO pLibInfo = pfnGetNewInf();
    if (!pLibInfo) {
        std::wcout << L"GetNewInf 返回 NULL" << std::endl;
        FreeLibrary(hLib);
        return 1;
    }
    
    std::wcout << L"GetNewInf 调用成功！" << std::endl;
    std::wcout << L"支持库格式版本: " << pLibInfo->m_dwLibFormatVer << std::endl;
    std::wcout << L"支持库名称: " << (pLibInfo->m_szName ? pLibInfo->m_szName : "NULL") << std::endl;
    std::wcout << L"命令数目: " << pLibInfo->m_nCmdCount << std::endl;
    std::wcout << L"数据类型数目: " << pLibInfo->m_nDataTypeCount << std::endl;
    std::wcout << L"类别数目: " << pLibInfo->m_nCategoryCount << std::endl;
    
    FreeLibrary(hLib);
    return 0;
}
