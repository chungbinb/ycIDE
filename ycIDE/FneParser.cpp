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
    // 如果 dataType 为 0，表示无返回值
    if (dataType == 0) {
        return L"无返回值";
    }
    
    // 检查系统基本类型（最高位为1表示系统类型）
    if (dataType == SDT_BYTE) return L"字节型";
    if (dataType == SDT_SHORT) return L"短整数型";
    if (dataType == SDT_INT) return L"整数型";
    if (dataType == SDT_INT64) return L"长整数型";
    if (dataType == SDT_FLOAT) return L"小数型";
    if (dataType == SDT_DOUBLE) return L"双精度小数型";
    if (dataType == SDT_BOOL) return L"逻辑型";
    if (dataType == SDT_DATE_TIME) return L"日期时间型";
    if (dataType == SDT_TEXT) return L"文本型";
    if (dataType == SDT_BIN) return L"字节集";
    if (dataType == SDT_SUB_PTR) return L"子程序指针";

    // 如果最高位为0但不是0，可能是自定义类型索引
    if ((dataType & 0x80000000) == 0) {
        // 非零的自定义类型索引，返回自定义类型
        return L"自定义类型";
    }

    // 处理一些特殊状态类型
    DWORD lowWord = LOWORD(dataType);
    DWORD highWord = HIWORD(dataType);

    // 检测自定义类型或特殊状态中的用户自定义类型
    if ((dataType & 0x40000000) != 0) {
        // 用户自定义类型
        return L"用户自定义类型";
    }

    // 可能是特殊状态中的对象数组
    if ((dataType & 0x20000000) != 0) {
        return L"数组";
    }

    return L"未知类型";
}

// 数据类型转换为带英文名的完整格式字符串（用于参数说明）
std::wstring FneParser::DataTypeToStringWithEnglish(DATA_TYPE dataType) {
    if (dataType == 0) {
        return L"无返回值";
    }
    
    if (dataType == SDT_BYTE) return L"字节型（byte）";
    if (dataType == SDT_SHORT) return L"短整数型（short）";
    if (dataType == SDT_INT) return L"整数型（int）";
    if (dataType == SDT_INT64) return L"长整数型（long）";
    if (dataType == SDT_FLOAT) return L"小数型（float）";
    if (dataType == SDT_DOUBLE) return L"双精度小数型（double）";
    if (dataType == SDT_BOOL) return L"逻辑型（bool）";
    if (dataType == SDT_DATE_TIME) return L"日期时间型（datetime）";
    if (dataType == SDT_TEXT) return L"文本型（string）";
    if (dataType == SDT_BIN) return L"字节集（bin）";
    if (dataType == SDT_SUB_PTR) return L"子程序指针（subptr）";
    
    if ((dataType & 0x80000000) == 0) {
        return L"自定义类型";
    }
    
    if ((dataType & 0x40000000) != 0) {
        return L"用户自定义类型";
    }
    
    if ((dataType & 0x20000000) != 0) {
        return L"数组";
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
        CreateDirectoryW(L"logs", NULL);
        std::wstring debugPath = L"logs\\fne_error.txt";
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
            CreateDirectoryW(L"logs", NULL);
            std::wstring debugPath = L"logs\\fne_error.txt";
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
        CreateDirectoryW(L"logs", NULL);
        std::wstring debugPath = L"logs\\fne_error.txt";
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

            // 原始命令索引
            cmdInfo.commandIndex = i;

            // 命令名称
            if (pCmdInfo->m_szName) {
                cmdInfo.name = UTF8ToUTF16(pCmdInfo->m_szName);
            }

            // 英文名称
            if (pCmdInfo->m_szEgName) {
                cmdInfo.englishName = UTF8ToUTF16(pCmdInfo->m_szEgName);
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

                    // 填充结构化参数信息
                    FneCommandParam param;
                    
                    // 参数名称
                    if (pArgInfo->m_szName) {
                        param.name = UTF8ToUTF16(pArgInfo->m_szName);
                    }
                    
                    // 参数类型
                    param.type = DataTypeToString(pArgInfo->m_dtType);
                    param.typeWithEnglish = DataTypeToStringWithEnglish(pArgInfo->m_dtType);
                    
                    // 参数说明
                    if (pArgInfo->m_szExplain) {
                        param.description = UTF8ToUTF16(pArgInfo->m_szExplain);
                    }
                    
                    // 参数标志
                    param.optional = (pArgInfo->m_dwState & AS_HAS_DEFAULT_VALUE) != 0 ||
                                     (pArgInfo->m_dwState & AS_DEFAULT_VALUE_IS_EMPTY) != 0;
                    param.isVariable = (pArgInfo->m_dwState & AS_RECEIVE_VAR) != 0;
                    param.isArray = (pArgInfo->m_dwState & AS_RECEIVE_ARRAY_DATA) != 0 ||
                                    (pArgInfo->m_dwState & AS_RECEIVE_VAR_ARRAY) != 0;
                    
                    // 通用型：接收所有类型数据的参数
                    if (pArgInfo->m_dwState & AS_RECEIVE_ALL_TYPE_DATA) {
                        param.type = L"通用型";
                        param.typeWithEnglish = L"通用型（all）";
                    }
                    
                    cmdInfo.params.push_back(param);

                    // 兼容旧格式：构建参数字符串
                    std::wstring paramStr;
                    if (!param.name.empty()) {
                        paramStr = param.name + L" : " + param.type;
                    } else {
                        paramStr = param.type;
                    }

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

    // 解析自定义数据类型和窗口组件
    if (pLibInfo->m_pDataType && pLibInfo->m_nDataTypeCount > 0) {
        for (int i = 0; i < pLibInfo->m_nDataTypeCount; i++) {
            PLIB_DATA_TYPE_INFO pDataTypeInfo = &pLibInfo->m_pDataType[i];
            
            // 是否窗口组件
            bool isWindowUnit = (pDataTypeInfo->m_dwState & LDT_WIN_UNIT) != 0;
            bool isHidden = (pDataTypeInfo->m_dwState & LDT_IS_HIDED) != 0;
            
            if (isWindowUnit && !isHidden) {
                // 解析窗口组件
                FneWindowUnitInfo unitInfo;
                
                // 组件名称
                if (pDataTypeInfo->m_szName) {
                    unitInfo.name = UTF8ToUTF16(pDataTypeInfo->m_szName);
                }
                
                // 英文名称
                if (pDataTypeInfo->m_szEgName) {
                    unitInfo.englishName = UTF8ToUTF16(pDataTypeInfo->m_szEgName);
                }
                
                // 说明
                if (pDataTypeInfo->m_szExplain) {
                    unitInfo.description = UTF8ToUTF16(pDataTypeInfo->m_szExplain);
                }
                
                // 所属支持库
                unitInfo.libraryName = libraryName;
                
                // 是否容器
                unitInfo.isContainer = (pDataTypeInfo->m_dwState & LDT_IS_CONTAINER) != 0;
                
                // 是否Tab控件
                unitInfo.isTabUnit = (pDataTypeInfo->m_dwState & LDT_IS_TAB_UNIT) != 0;
                
                // 是否可获取焦点
                unitInfo.canGetFocus = (pDataTypeInfo->m_dwState & LDT_CANNOT_GET_FOCUS) == 0;
                
                // 图标ID
                unitInfo.bitmapId = pDataTypeInfo->m_dwUnitBmpID;
                
                // 成员命令索引
                if (pDataTypeInfo->m_pnCmdsIndex && pDataTypeInfo->m_nCmdCount > 0) {
                    for (int j = 0; j < pDataTypeInfo->m_nCmdCount; j++) {
                        unitInfo.memberCommands.push_back(pDataTypeInfo->m_pnCmdsIndex[j]);
                    }
                }
                
                // 解析属性
                ParseWindowUnitProperties(pDataTypeInfo, unitInfo);
                
                // 解析事件
                ParseWindowUnitEvents(pDataTypeInfo, unitInfo);
                
                if (!unitInfo.name.empty()) {
                    windowUnits.push_back(unitInfo);
                    OutputDebugStringW((L"[FneParser] 窗口组件: " + unitInfo.name + 
                        L", 属性数: " + std::to_wstring(unitInfo.properties.size()) +
                        L", 事件数: " + std::to_wstring(unitInfo.events.size()) + L"\n").c_str());
                }
            }
            else if (!isHidden && !isWindowUnit) {
                // 普通数据类型
                FneDataTypeInfo dtInfo;
                
                if (pDataTypeInfo->m_szName) {
                    dtInfo.name = UTF8ToUTF16(pDataTypeInfo->m_szName);
                }
                if (pDataTypeInfo->m_szEgName) {
                    dtInfo.englishName = UTF8ToUTF16(pDataTypeInfo->m_szEgName);
                }
                if (pDataTypeInfo->m_szExplain) {
                    dtInfo.description = UTF8ToUTF16(pDataTypeInfo->m_szExplain);
                }
                dtInfo.isHidden = false;
                dtInfo.isWindowUnit = false;
                
                if (!dtInfo.name.empty()) {
                    dataTypes.push_back(dtInfo);
                }
            }
        }
    }

    return true;
}

// 解析窗口组件属性
void FneParser::ParseWindowUnitProperties(PLIB_DATA_TYPE_INFO pDataTypeInfo, FneWindowUnitInfo& unitInfo) {
    OutputDebugStringW((L"[ParseWindowUnitProperties] 组件: " + unitInfo.name + 
        L", m_pPropertyBegin=" + std::to_wstring((uintptr_t)pDataTypeInfo->m_pPropertyBegin) + 
        L", m_nPropertyCount=" + std::to_wstring(pDataTypeInfo->m_nPropertyCount) + L"\n").c_str());
    
    if (!pDataTypeInfo->m_pPropertyBegin || pDataTypeInfo->m_nPropertyCount <= 0) {
        return;
    }
    
    PUNIT_PROPERTY_RAW pProperties = (PUNIT_PROPERTY_RAW)pDataTypeInfo->m_pPropertyBegin;
    
    for (int i = 0; i < pDataTypeInfo->m_nPropertyCount; i++) {
        PUNIT_PROPERTY_RAW pProp = &pProperties[i];
        
        FnePropertyInfo propInfo;
        
        // 属性名称
        if (pProp->m_szName) {
            propInfo.name = UTF8ToUTF16(pProp->m_szName);
        }
        
        // 英文名称
        if (pProp->m_szEgName) {
            propInfo.englishName = UTF8ToUTF16(pProp->m_szEgName);
        }
        
        // 属性说明
        if (pProp->m_szExplain) {
            propInfo.description = UTF8ToUTF16(pProp->m_szExplain);
        }
        
        // 属性类型
        propInfo.type = static_cast<PropertyType>(pProp->m_shtType);
        
        // 是否只读
        propInfo.isReadOnly = (pProp->m_wState & UW_ONLY_READ) != 0;
        
        // 是否隐藏
        propInfo.isHidden = (pProp->m_wState & UW_IS_HIDED) != 0;
        
        // 解析选择选项
        if (pProp->m_szzPickStr) {
            const char* pStr = pProp->m_szzPickStr;
            while (*pStr) {
                std::wstring option = UTF8ToUTF16(pStr);
                if (!option.empty()) {
                    propInfo.pickOptions.push_back(option);
                }
                pStr += strlen(pStr) + 1;
            }
        }
        
        // 添加非隐藏属性
        if (!propInfo.isHidden && !propInfo.name.empty()) {
            unitInfo.properties.push_back(propInfo);
        }
    }
}

// 解析窗口组件事件
void FneParser::ParseWindowUnitEvents(PLIB_DATA_TYPE_INFO pDataTypeInfo, FneWindowUnitInfo& unitInfo) {
    if (!pDataTypeInfo->m_pEventBegin || pDataTypeInfo->m_nEventCount <= 0) {
        return;
    }
    
    PEVENT_INFO2_RAW pEvents = (PEVENT_INFO2_RAW)pDataTypeInfo->m_pEventBegin;
    
    for (int i = 0; i < pDataTypeInfo->m_nEventCount; i++) {
        PEVENT_INFO2_RAW pEvent = &pEvents[i];
        
        FneEventInfo eventInfo;
        
        // 事件名称
        if (pEvent->m_szName) {
            eventInfo.name = UTF8ToUTF16(pEvent->m_szName);
        }
        
        // 事件说明
        if (pEvent->m_szExplain) {
            eventInfo.description = UTF8ToUTF16(pEvent->m_szExplain);
        }
        
        // 是否隐藏
        eventInfo.isHidden = (pEvent->m_dwState & EV_IS_HIDED) != 0;
        
        // 是否键盘事件
        eventInfo.isKeyEvent = (pEvent->m_dwState & EV_IS_KEY_EVENT) != 0;
        
        // 返回类型
        eventInfo.returnsInt = (pEvent->m_dwState & EV_RETURN_INT) != 0;
        eventInfo.returnsBool = (pEvent->m_dwState & EV_RETURN_BOOL) != 0;
        
        // 解析事件参数
        if (pEvent->m_pEventArgInfo && pEvent->m_nArgCount > 0) {
            for (int j = 0; j < pEvent->m_nArgCount; j++) {
                PEVENT_ARG_INFO2_RAW pArg = &pEvent->m_pEventArgInfo[j];
                
                FneEventArgInfo argInfo;
                
                if (pArg->m_szName) {
                    argInfo.name = UTF8ToUTF16(pArg->m_szName);
                }
                if (pArg->m_szExplain) {
                    argInfo.description = UTF8ToUTF16(pArg->m_szExplain);
                }
                argInfo.dataType = DataTypeToString(pArg->m_dtDataType);
                argInfo.isByRef = (pArg->m_dwState & EAS_BY_REF) != 0;
                
                eventInfo.arguments.push_back(argInfo);
            }
        }
        
        // 添加非隐藏事件
        if (!eventInfo.isHidden && !eventInfo.name.empty()) {
            unitInfo.events.push_back(eventInfo);
        }
    }
}

// 根据名称查找窗口组件
const FneWindowUnitInfo* FneParser::FindWindowUnit(const std::wstring& name) const {
    for (const auto& unit : windowUnits) {
        if (unit.name == name) {
            return &unit;
        }
    }
    return nullptr;
}
