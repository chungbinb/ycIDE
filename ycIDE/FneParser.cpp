#include "FneParser.h"
#include <windows.h>

FneParser::FneParser() : hModule(NULL) {
}

FneParser::~FneParser() {
    if (hModule) {
        FreeLibrary(hModule);
        hModule = NULL;
    }
}

// UTF-8 转 UTF-16
std::wstring FneParser::UTF8ToUTF16(const char* utf8Str) {
    if (!utf8Str || utf8Str[0] == '\0') {
        return L"";
    }

    // 第一次调用获取需要的缓冲区大小
    int wideSize = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, NULL, 0);
    if (wideSize == 0) {
        return L"";
    }

    std::wstring wideStr(wideSize - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, &wideStr[0], wideSize);

    return wideStr;
}

// 数据类型转换为字符串
std::wstring FneParser::DataTypeToString(DATA_TYPE dataType) {
    // 如果最 HIWORD 最高位 0 表示系统基本类型
    if ((dataType & 0x80000000) == 0) {
        return L"自定义类型";
    }

    if (dataType == SDT_BYTE) return L"字节";
    if (dataType == SDT_SHORT) return L"短整数";
    if (dataType == SDT_INT) return L"整数";
    if (dataType == SDT_INT64) return L"长整数";
    if (dataType == SDT_FLOAT) return L"小数";
    if (dataType == SDT_DOUBLE) return L"双精度小数";
    if (dataType == SDT_BOOL) return L"逻辑";
    if (dataType == SDT_DATE_TIME) return L"日期时间";
    if (dataType == SDT_TEXT) return L"文本";
    if (dataType == SDT_BIN) return L"字节集";
    if (dataType == SDT_SUB_PTR) return L"子程序指针";

    // 处理一些特殊状态类型，这里获取原始值
    DWORD lowWord = LOWORD(dataType);
    DWORD highWord = HIWORD(dataType);

    // 检测自定义类型或特殊状态中的用户自定义类型
    if ((dataType & 0x40000000) != 0) {
        // 用户自定义类型
        return L"用户自定义类型";
    }

    // 可能是特殊状态中的对象数组（ LOWORD
    if ((dataType & 0x20000000) != 0) {
        return L"数组[]";
    }

    return L"未知类型";
}

// 加载 .fne 文件
bool FneParser::LoadFneFile(const std::wstring& filePath) {
    if (hModule) {
        FreeLibrary(hModule);
        hModule = NULL;
        commands.clear();
    }

    hModule = LoadLibraryW(filePath.c_str());
    if (!hModule) {
        DWORD error = GetLastError();
        std::wstring debugPath = filePath.substr(0, filePath.find_last_of(L"\\")) + L"\\..\\fne_error.txt";
        HANDLE hDebug = CreateFileW(debugPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hDebug != INVALID_HANDLE_VALUE) {
            SetFilePointer(hDebug, 0, NULL, FILE_END);
            std::wstring msg = L"LoadLibrary failed: " + filePath + L" Error: " + std::to_wstring(error) + L"\r\n";
            DWORD written;
            WriteFile(hDebug, msg.c_str(), static_cast<DWORD>(msg.length() * sizeof(wchar_t)), &written, NULL);
            CloseHandle(hDebug);
        }
        return false;
    }

    // 尝试多个可能的导出函数名
    PFN_GET_LIB_INFO pfnGetNewInf = nullptr;
    const char* functionNames[] = { "GetNewInf", "_GetNewInf", "GetLibInfo", "_GetLibInfo", "?GetNewInf@@YAPEAUtagLibInfo@@XZ" };
    
    for (const char* funcName : functionNames) {
        pfnGetNewInf = (PFN_GET_LIB_INFO)GetProcAddress(hModule, funcName);
        if (pfnGetNewInf) {
            // 找到了导出函数，记录成功的函数名
            std::wstring debugPath = filePath.substr(0, filePath.find_last_of(L"\\")) + L"\\..\\fne_error.txt";
            HANDLE hDebug = CreateFileW(debugPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hDebug != INVALID_HANDLE_VALUE) {
                SetFilePointer(hDebug, 0, NULL, FILE_END);
                std::wstring msg = L"SUCCESS: " + filePath + L" - Function: " + std::wstring(funcName, funcName + strlen(funcName)) + L"\r\n";
                DWORD written;
                WriteFile(hDebug, msg.c_str(), static_cast<DWORD>(msg.length() * sizeof(wchar_t)), &written, NULL);
                CloseHandle(hDebug);
            }
            break;
        }
    }
    
    if (!pfnGetNewInf) {
        std::wstring debugPath = filePath.substr(0, filePath.find_last_of(L"\\")) + L"\\..\\fne_error.txt";
        HANDLE hDebug = CreateFileW(debugPath.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hDebug != INVALID_HANDLE_VALUE) {
            SetFilePointer(hDebug, 0, NULL, FILE_END);
            std::wstring msg = L"GetProcAddress failed (tried all names): " + filePath + L"\r\n";
            DWORD written;
            WriteFile(hDebug, msg.c_str(), static_cast<DWORD>(msg.length() * sizeof(wchar_t)), &written, NULL);
            CloseHandle(hDebug);
        }
        FreeLibrary(hModule);
        hModule = NULL;
        return false;
    }

    PLIB_INFO pLibInfo = pfnGetNewInf();
    if (!pLibInfo) {
        FreeLibrary(hModule);
        hModule = NULL;
        return false;
    }

    // 获取支持库名称和基本信息
    if (pLibInfo->m_szName) {
        libraryName = UTF8ToUTF16(pLibInfo->m_szName);
    }
    if (pLibInfo->m_szExplain) {
        libraryDescription = UTF8ToUTF16(pLibInfo->m_szExplain);
    }

    // 解析命令类别列表
    std::vector<std::wstring> categories;
    if (pLibInfo->m_szzCategory && pLibInfo->m_nCategoryCount > 0) {
        const char* pCategory = pLibInfo->m_szzCategory;
        OutputDebugStringW((L"[FneParser] 类别总数: " + std::to_wstring(pLibInfo->m_nCategoryCount) + L"\n").c_str());
        for (int i = 0; i < pLibInfo->m_nCategoryCount; i++) {
            std::wstring catName = UTF8ToUTF16(pCategory);
            // 去掉类别名前面的 "0000" 前缀
            if (catName.length() > 4 && catName.substr(0, 4) == L"0000") {
                catName = catName.substr(4);
            }
            categories.push_back(catName);
            OutputDebugStringW((L"[FneParser] 类别[" + std::to_wstring(i+1) + L"]: " + catName + L"\n").c_str());
            // 移动到下一个类别字符串（双\0结束）
            pCategory += strlen(pCategory) + 1;
        }
    }

    // 解析所有命令信息
    if (pLibInfo->m_pBeginCmdInfo && pLibInfo->m_nCmdCount > 0) {
        for (int i = 0; i < pLibInfo->m_nCmdCount; i++) {
            PCMD_INFO pCmdInfo = &pLibInfo->m_pBeginCmdInfo[i];

            FneCommandInfo cmdInfo;

            // 命令名称
            if (pCmdInfo->m_szName) {
                cmdInfo.name = UTF8ToUTF16(pCmdInfo->m_szName);
            }

            // 命令说明
            if (pCmdInfo->m_szExplain) {
                cmdInfo.description = UTF8ToUTF16(pCmdInfo->m_szExplain);
            }

            // 返回值类型
            cmdInfo.returnType = DataTypeToString(pCmdInfo->m_dtRetValType);

            // 命令类别（索引从1开始）
            if (pCmdInfo->m_shtCategory > 0 && pCmdInfo->m_shtCategory <= (short)categories.size()) {
                cmdInfo.category = categories[pCmdInfo->m_shtCategory - 1];
            } else {
                cmdInfo.category = L"";
            }
            
            // 调试输出前几个命令的类别
            if (i < 20) {
                OutputDebugStringW((L"[FneParser] 命令: " + cmdInfo.name + L", 类别索引: " + std::to_wstring(pCmdInfo->m_shtCategory) + L", 类别: " + cmdInfo.category + L"\n").c_str());
            }

            // 是否隐藏
            cmdInfo.isHidden = (pCmdInfo->m_wState & CT_IS_HIDED) != 0;

            // 解析参数
            if (pCmdInfo->m_pBeginArgInfo && pCmdInfo->m_nArgCount > 0) {
                for (int j = 0; j < pCmdInfo->m_nArgCount; j++) {
                    PARG_INFO pArgInfo = &pCmdInfo->m_pBeginArgInfo[j];

                    std::wstring paramStr;

                    // 参数名称
                    if (pArgInfo->m_szName) {
                        paramStr = UTF8ToUTF16(pArgInfo->m_szName);
                    }

                    // 参数类型
                    std::wstring paramType = DataTypeToString(pArgInfo->m_dtType);
                    if (!paramStr.empty()) {
                        paramStr += L" : " + paramType;
                    }
                    else {
                        paramStr = paramType;
                    }

                    // 参数标志
                    if (pArgInfo->m_dwState & AS_HAS_DEFAULT_VALUE) {
                        paramStr += L" = " + std::to_wstring(pArgInfo->m_nDefault);
                    }
                    else if (pArgInfo->m_dwState & AS_DEFAULT_VALUE_IS_EMPTY) {
                        paramStr += L" [可选]";
                    }

                    if (pArgInfo->m_dwState & AS_RECEIVE_VAR) {
                        paramStr += L" [变量]";
                    }
                    else if (pArgInfo->m_dwState & AS_RECEIVE_VAR_ARRAY) {
                        paramStr += L" [变量或数组]";
                    }
                    else if (pArgInfo->m_dwState & AS_RECEIVE_ARRAY_DATA) {
                        paramStr += L" [数组]";
                    }

                    cmdInfo.parameters.push_back(paramStr);
                }
            }

            // 只添加非隐藏命令到命令列表
            if (!cmdInfo.isHidden) {
                commands.push_back(cmdInfo);
            }
        }
    }

    // 解析自定义数据类型
    if (pLibInfo->m_pDataType && pLibInfo->m_nDataTypeCount > 0) {
        for (int i = 0; i < pLibInfo->m_nDataTypeCount; i++) {
            PLIB_DATA_TYPE_INFO pDataTypeInfo = &pLibInfo->m_pDataType[i];
            
            FneDataTypeInfo dtInfo;
            
            // 数据类型名称
            if (pDataTypeInfo->m_szName) {
                dtInfo.name = UTF8ToUTF16(pDataTypeInfo->m_szName);
            }
            
            // 英文名称
            if (pDataTypeInfo->m_szEgName) {
                dtInfo.englishName = UTF8ToUTF16(pDataTypeInfo->m_szEgName);
            }
            
            // 说明
            if (pDataTypeInfo->m_szExplain) {
                dtInfo.description = UTF8ToUTF16(pDataTypeInfo->m_szExplain);
            }
            
            // 是否隐藏
            dtInfo.isHidden = (pDataTypeInfo->m_dwState & LDT_IS_HIDED) != 0;
            
            // 是否窗口组件
            dtInfo.isWindowUnit = (pDataTypeInfo->m_dwState & LDT_WIN_UNIT) != 0;
            
            // 只添加非隐藏且非窗口组件的数据类型
            if (!dtInfo.isHidden && !dtInfo.isWindowUnit && !dtInfo.name.empty()) {
                dataTypes.push_back(dtInfo);
            }
        }
    }

    return true;
}
