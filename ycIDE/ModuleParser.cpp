#include "ModuleParser.h"
#include "LibraryParser.h"
#include "PinyinHelper.h"
#include <fstream>
#include <sstream>

// 静态成员初始化
ModuleParser* ModuleParser::currentInstance_ = nullptr;

ModuleParser::ModuleParser()
    : hDll_(nullptr)
    , parser_(nullptr)
    , pfnCreate_(nullptr)
    , pfnDestroy_(nullptr)
    , pfnAddEvent_(nullptr)
    , pfnParse_(nullptr)
    , pfnFindEBase_(nullptr)
    , pfnFreePtr_(nullptr)
    , pfnFreeEBase_(nullptr)
    , pfnPtr2Str_(nullptr)
    , pfnPtr2Arr_(nullptr)
{
}

ModuleParser::~ModuleParser() {
    Clear();
    UnloadDll();
}

bool ModuleParser::Initialize(const std::wstring& eLangRootPath) {
    eLangRoot_ = eLangRootPath;
    
    if (!LoadDll()) {
        // lastError_ 已经在 LoadDll() 中设置
        return false;
    }
    
    return true;
}

bool ModuleParser::LoadDll() {
    std::wstring attemptedPaths;
    
    // 首先尝试从当前目录加载
    hDll_ = ::LoadLibraryW(L"ECodeParser.dll");
    attemptedPaths += L"1. 当前目录: ECodeParser.dll\n";
    
    if (!hDll_) {
        // 尝试从程序所在目录加载
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);
        std::wstring dllPath = exePath;
        size_t pos = dllPath.find_last_of(L"\\/");
        if (pos != std::wstring::npos) {
            dllPath = dllPath.substr(0, pos + 1) + L"ECodeParser.dll";
            hDll_ = ::LoadLibraryW(dllPath.c_str());
            attemptedPaths += L"2. 程序目录: " + dllPath + L"\n";
        }
    }
    
    if (!hDll_) {
        // 尝试从易语言模块查看器目录加载
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);
        std::wstring dllPath = exePath;
        size_t pos = dllPath.find_last_of(L"\\/");
        if (pos != std::wstring::npos) {
            dllPath = dllPath.substr(0, pos + 1) + L"..\\易语言模块查看器\\ECodeParser.dll";
            hDll_ = ::LoadLibraryW(dllPath.c_str());
            attemptedPaths += L"3. 查看器目录: " + dllPath + L"\n";
        }
    }
    
    if (!hDll_) {
        lastError_ = L"无法加载 ECodeParser.dll\n\n已尝试的路径：\n" + attemptedPaths + 
                     L"\n请确保 ECodeParser.dll 在上述任一位置。";
        return false;
    }
    
    // 获取函数指针
    // 尝试多种方式：按序号、按名称（带@）、按名称（不带@）、stdcall修饰名
    pfnCreate_ = (PFN_ECodeParser_Create)GetProcAddress(hDll_, (LPCSTR)1);  // 尝试序号1
    if (!pfnCreate_) pfnCreate_ = (PFN_ECodeParser_Create)GetProcAddress(hDll_, "ECodeParser_Create");
    if (!pfnCreate_) pfnCreate_ = (PFN_ECodeParser_Create)GetProcAddress(hDll_, "_ECodeParser_Create");
    if (!pfnCreate_) pfnCreate_ = (PFN_ECodeParser_Create)GetProcAddress(hDll_, "_ECodeParser_Create@4");
    
    pfnDestroy_ = (PFN_ECodeParser_Destroy)GetProcAddress(hDll_, (LPCSTR)2);  // 尝试序号2
    if (!pfnDestroy_) pfnDestroy_ = (PFN_ECodeParser_Destroy)GetProcAddress(hDll_, "ECodeParser_Destroy");
    if (!pfnDestroy_) pfnDestroy_ = (PFN_ECodeParser_Destroy)GetProcAddress(hDll_, "_ECodeParser_Destroy");
    if (!pfnDestroy_) pfnDestroy_ = (PFN_ECodeParser_Destroy)GetProcAddress(hDll_, "_ECodeParser_Destroy@4");
    
    pfnAddEvent_ = (PFN_ECodeParser_AddEvent)GetProcAddress(hDll_, (LPCSTR)3);  // 尝试序号3
    if (!pfnAddEvent_) pfnAddEvent_ = (PFN_ECodeParser_AddEvent)GetProcAddress(hDll_, "ECodeParser_AddEvent");
    if (!pfnAddEvent_) pfnAddEvent_ = (PFN_ECodeParser_AddEvent)GetProcAddress(hDll_, "_ECodeParser_AddEvent");
    if (!pfnAddEvent_) pfnAddEvent_ = (PFN_ECodeParser_AddEvent)GetProcAddress(hDll_, "_ECodeParser_AddEvent@12");
    
    pfnParse_ = (PFN_ECodeParser_Parse)GetProcAddress(hDll_, (LPCSTR)4);  // 尝试序号4
    if (!pfnParse_) pfnParse_ = (PFN_ECodeParser_Parse)GetProcAddress(hDll_, "ECodeParser_Parse");
    if (!pfnParse_) pfnParse_ = (PFN_ECodeParser_Parse)GetProcAddress(hDll_, "_ECodeParser_Parse");
    if (!pfnParse_) pfnParse_ = (PFN_ECodeParser_Parse)GetProcAddress(hDll_, "_ECodeParser_Parse@16");
    
    pfnFindEBase_ = (PFN_ECodeParser_FindEBase)GetProcAddress(hDll_, (LPCSTR)6);  // 尝试序号6
    if (!pfnFindEBase_) pfnFindEBase_ = (PFN_ECodeParser_FindEBase)GetProcAddress(hDll_, "ECodeParser_FindEBase");
    if (!pfnFindEBase_) pfnFindEBase_ = (PFN_ECodeParser_FindEBase)GetProcAddress(hDll_, "_ECodeParser_FindEBase");
    if (!pfnFindEBase_) pfnFindEBase_ = (PFN_ECodeParser_FindEBase)GetProcAddress(hDll_, "_ECodeParser_FindEBase@8");
    
    pfnFreePtr_ = (PFN_FreePtr)GetProcAddress(hDll_, (LPCSTR)11);  // 尝试序号11
    if (!pfnFreePtr_) pfnFreePtr_ = (PFN_FreePtr)GetProcAddress(hDll_, "FreePtr");
    if (!pfnFreePtr_) pfnFreePtr_ = (PFN_FreePtr)GetProcAddress(hDll_, "_FreePtr");
    if (!pfnFreePtr_) pfnFreePtr_ = (PFN_FreePtr)GetProcAddress(hDll_, "_FreePtr@4");
    
    pfnFreeEBase_ = (PFN_FreeEBase)GetProcAddress(hDll_, (LPCSTR)12);  // 尝试序号12
    if (!pfnFreeEBase_) pfnFreeEBase_ = (PFN_FreeEBase)GetProcAddress(hDll_, "FreeEBase");
    if (!pfnFreeEBase_) pfnFreeEBase_ = (PFN_FreeEBase)GetProcAddress(hDll_, "_FreeEBase");
    if (!pfnFreeEBase_) pfnFreeEBase_ = (PFN_FreeEBase)GetProcAddress(hDll_, "_FreeEBase@4");
    
    pfnPtr2Str_ = (PFN_Ptr2Str)GetProcAddress(hDll_, (LPCSTR)10);  // 尝试序号10
    if (!pfnPtr2Str_) pfnPtr2Str_ = (PFN_Ptr2Str)GetProcAddress(hDll_, "Ptr2Str");
    if (!pfnPtr2Str_) pfnPtr2Str_ = (PFN_Ptr2Str)GetProcAddress(hDll_, "_Ptr2Str");
    if (!pfnPtr2Str_) pfnPtr2Str_ = (PFN_Ptr2Str)GetProcAddress(hDll_, "_Ptr2Str@4");
    
    pfnPtr2Arr_ = (PFN_Ptr2Arr)GetProcAddress(hDll_, (LPCSTR)9);  // 尝试序号9
    if (!pfnPtr2Arr_) pfnPtr2Arr_ = (PFN_Ptr2Arr)GetProcAddress(hDll_, "Ptr2Arr");
    if (!pfnPtr2Arr_) pfnPtr2Arr_ = (PFN_Ptr2Arr)GetProcAddress(hDll_, "_Ptr2Arr");
    if (!pfnPtr2Arr_) pfnPtr2Arr_ = (PFN_Ptr2Arr)GetProcAddress(hDll_, "_Ptr2Arr@8");
    
    if (!pfnCreate_ || !pfnDestroy_ || !pfnAddEvent_ || !pfnParse_) {
        lastError_ = L"无法加载 ECodeParser.dll 的导出函数！\n\n";
        lastError_ += L"最可能的原因：\n";
        lastError_ += L"  当前 ECodeParser.dll 是 32 位版本\n";
        lastError_ += L"  而 ycIDE 编译为 64 位程序\n\n";
        lastError_ += L"解决方案：\n";
#ifdef _WIN64
        lastError_ += L"  1. 将 ycIDE 项目配置改为 x86 (推荐)\n";
        lastError_ += L"  2. 或获取 64 位版本的 ECodeParser.dll\n";
#else
        lastError_ += L"  1. 将 ycIDE 项目配置改为 x64\n";
        lastError_ += L"  2. 或获取 32 位版本的 ECodeParser.dll\n";
#endif
        lastError_ += L"\n缺少的函数：\n";
        if (!pfnCreate_) lastError_ += L"  - @ECodeParser_Create\n";
        if (!pfnDestroy_) lastError_ += L"  - @ECodeParser_Destroy\n";
        if (!pfnAddEvent_) lastError_ += L"  - @ECodeParser_AddEvent\n";
        if (!pfnParse_) lastError_ += L"  - @ECodeParser_Parse\n";
        UnloadDll();
        return false;
    }
    
    return true;
}

void ModuleParser::UnloadDll() {
    if (parser_ && pfnDestroy_) {
        pfnDestroy_(parser_);
        parser_ = nullptr;
    }
    
    if (hDll_) {
        FreeLibrary(hDll_);
        hDll_ = nullptr;
    }
    
    pfnCreate_ = nullptr;
    pfnDestroy_ = nullptr;
    pfnAddEvent_ = nullptr;
    pfnParse_ = nullptr;
    pfnFindEBase_ = nullptr;
    pfnFreePtr_ = nullptr;
    pfnFreeEBase_ = nullptr;
    pfnPtr2Str_ = nullptr;
    pfnPtr2Arr_ = nullptr;
}

bool ModuleParser::ReadFile(const std::wstring& filePath, std::vector<BYTE>& data) {
    HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ,
                               NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        lastError_ = L"无法打开文件: " + filePath;
        return false;
    }
    
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        CloseHandle(hFile);
        lastError_ = L"无法获取文件大小";
        return false;
    }
    
    data.resize(fileSize);
    DWORD bytesRead = 0;
    if (!::ReadFile(hFile, data.data(), fileSize, &bytesRead, NULL)) {
        CloseHandle(hFile);
        lastError_ = L"读取文件失败";
        return false;
    }
    
    CloseHandle(hFile);
    return true;
}

bool ModuleParser::DecryptModule(std::vector<BYTE>& data, const std::wstring& password) {
    // 检查是否为加密模块 (WTLE 标识)
    if (data.size() < 4) return false;
    
    if (data[0] == 0x57 && data[1] == 0x54 && data[2] == 0x4C && data[3] == 0x45) {
        // 这是加密模块
        if (password.empty()) {
            lastError_ = L"该模块已加密，需要提供密码";
            return false;
        }
        
        // TODO: 实现解密算法（需要移植易语言的解密代码）
        // 目前先返回失败，提示用户
        lastError_ = L"加密模块解密功能暂未实现，请使用未加密的模块";
        return false;
    }
    
    return true;
}

bool ModuleParser::LoadModule(const std::wstring& filePath, const std::wstring& password) {
    Clear();
    
    if (!hDll_) {
        lastError_ = L"解析器未初始化";
        return false;
    }
    
    // 读取文件
    std::vector<BYTE> data;
    if (!ReadFile(filePath, data)) {
        return false;
    }
    
    // 检查并解密（如果需要）
    if (!DecryptModule(data, password)) {
        return false;
    }
    
    // 创建解析器实例
    if (!parser_) {
        // 传递易语言根目录
        parser_ = pfnCreate_(eLangRoot_.c_str());
        if (!parser_) {
            lastError_ = L"创建解析器失败！\n\n";
            if (eLangRoot_.empty()) {
                lastError_ += L"原因：未找到易语言安装目录\n";
                lastError_ += L"解决方案：请安装易语言，或将易语言安装目录路径配置到 ycIDE 中";
            } else {
                lastError_ += L"易语言路径：" + eLangRoot_ + L"\n";
                lastError_ += L"请检查易语言是否正确安装";
            }
            return false;
        }
    }
    
    // 设置当前实例（用于回调）
    currentInstance_ = this;
    
    // 注册事件回调
    if (pfnAddEvent_) {
        pfnAddEvent_(parser_, EVENT_METHOD, (void*)&OnMethodEvent);
        pfnAddEvent_(parser_, EVENT_DLL, (void*)&OnDllEvent);
        pfnAddEvent_(parser_, EVENT_STRUCT, (void*)&OnStructEvent);
        pfnAddEvent_(parser_, EVENT_GLOBAL, (void*)&OnGlobalEvent);
        pfnAddEvent_(parser_, EVENT_CONST, (void*)&OnConstEvent);
        pfnAddEvent_(parser_, EVENT_CLASS, (void*)&OnClassEvent);
        pfnAddEvent_(parser_, EVENT_RESOURCE, (void*)&OnResourceEvent);
        pfnAddEvent_(parser_, EVENT_INFO, (void*)&OnInfoEvent);
        pfnAddEvent_(parser_, EVENT_LIB, (void*)&OnLibEvent);
        pfnAddEvent_(parser_, EVENT_EC, (void*)&OnECEvent);
        pfnAddEvent_(parser_, EVENT_END, (void*)&OnEndEvent);
    }
    
    // 执行解析
    if (!pfnParse_) {
        lastError_ = L"解析函数未初始化";
        return false;
    }
    
    int result = pfnParse_(parser_, data.data(), (int)data.size(), FALSE);
    
    if (result != 0) {
        lastError_ = L"解析模块失败！\n\n";
        lastError_ += L"错误代码：" + std::to_wstring(result) + L"\n";
        lastError_ += L"可能的原因：\n";
        lastError_ += L"1. 模块文件已损坏\n";
        lastError_ += L"2. 不支持的模块格式\n";
        lastError_ += L"3. 缺少必要的依赖";
        return false;
    }
    
    return true;
}

void ModuleParser::Clear() {
    methods_.clear();
    dllCommands_.clear();
    dataTypes_.clear();
    classNames_.clear();
    moduleInfo_ = EModuleInfo();
    lastError_.clear();
}

std::wstring ModuleParser::ReadStringFromPtr(void* stringPtr) {
    if (!stringPtr || !pfnPtr2Str_) {
        return L"";
    }
    
    wchar_t* strData = pfnPtr2Str_(stringPtr);
    if (!strData) {
        return L"";
    }
    
    std::wstring result(strData);
    
    if (pfnFreePtr_) {
        pfnFreePtr_(strData);
    }
    
    return result;
}

std::wstring ModuleParser::DataTypeToString(int dataType) {
    switch (dataType) {
        case 1: return L"字节型";
        case 2: return L"短整数型";
        case 3: return L"整数型";
        case 4: return L"长整数型";
        case 5: return L"小数型";
        case 6: return L"双精度小数型";
        case 7: return L"逻辑型";
        case 8: return L"日期时间型";
        case 9: return L"文本型";
        case 10: return L"字节集";
        default: return L"未知类型";
    }
}

// 事件回调实现
void __stdcall ModuleParser::OnMethodEvent(void* parser, void* data) {
    if (!currentInstance_ || !data) return;
    
    // 这里需要解析 ESection_Program_Method 结构
    // 由于结构较复杂，这里做简化处理
    // 实际使用时需要根据结构定义完整解析
    
    EMethod method;
    // TODO: 解析方法数据
    // method.name = ...;
    // method.remark = ...;
    // method.returnType = ...;
    
    currentInstance_->methods_.push_back(method);
}

void __stdcall ModuleParser::OnDllEvent(void* parser, void* data) {
    if (!currentInstance_ || !data) return;
    
    // 解析 ESection_Program_Dll 结构
    EDllCommand dllCmd;
    // TODO: 解析DLL命令数据
    
    currentInstance_->dllCommands_.push_back(dllCmd);
}

void __stdcall ModuleParser::OnStructEvent(void* parser, void* data) {
    if (!currentInstance_ || !data) return;
    
    // 解析数据类型
    EDataStruct dataStruct;
    // TODO: 解析数据类型
    
    currentInstance_->dataTypes_.push_back(dataStruct);
}

void __stdcall ModuleParser::OnGlobalEvent(void* parser, void* data) {
    // 全局变量事件
}

void __stdcall ModuleParser::OnConstEvent(void* parser, void* data) {
    // 常量事件
}

void __stdcall ModuleParser::OnClassEvent(void* parser, void* data) {
    if (!currentInstance_ || !data) return;
    
    // 解析类名
    // TODO: 解析类名数据
}

void __stdcall ModuleParser::OnResourceEvent(void* parser, void* data) {
    // 资源事件
}

void __stdcall ModuleParser::OnInfoEvent(void* parser, void* data) {
    if (!currentInstance_ || !data) return;
    
    // 解析模块信息
    // TODO: 解析 ESection_UserInfo 结构
}

void __stdcall ModuleParser::OnLibEvent(void* parser, void* data) {
    // 支持库依赖事件
}

void __stdcall ModuleParser::OnECEvent(void* parser, void* data) {
    // 模块列表事件
}

void __stdcall ModuleParser::OnEndEvent(void* parser, void* data) {
    // 解析结束事件
}

std::vector<LibraryCommand> ModuleParser::ConvertToLibraryCommands(const std::wstring& moduleName) {
    std::vector<LibraryCommand> commands;
    
    // 转换子程序
    for (const auto& method : methods_) {
        LibraryCommand cmd;
        cmd.chineseName = method.name;
        cmd.library = moduleName;
        cmd.category = method.className.empty() ? L"模块命令" : method.className;
        cmd.returnType = method.returnType;
        cmd.description = method.remark;
        
        // 转换参数
        for (const auto& param : method.parameters) {
            LibraryParameter libParam;
            libParam.name = param.name;
            libParam.type = param.type;
            libParam.description = param.remark;
            libParam.optional = param.isOptional;
            cmd.parameters.push_back(libParam);
        }
        
        // 生成拼音
        cmd.pinyin = PinyinHelper::GetStringPinyins(cmd.chineseName);
        cmd.initials = PinyinHelper::GetStringInitialsList(cmd.chineseName);
        
        commands.push_back(cmd);
    }
    
    // 转换DLL命令
    for (const auto& dllCmd : dllCommands_) {
        LibraryCommand cmd;
        cmd.chineseName = dllCmd.showName;
        cmd.library = moduleName;
        cmd.category = L"DLL命令";
        cmd.returnType = dllCmd.returnType;
        cmd.description = dllCmd.remark;
        
        // 转换参数
        for (const auto& param : dllCmd.parameters) {
            LibraryParameter libParam;
            libParam.name = param.name;
            libParam.type = param.type;
            libParam.description = param.remark;
            libParam.optional = param.isOptional;
            cmd.parameters.push_back(libParam);
        }
        
        // 生成拼音
        cmd.pinyin = PinyinHelper::GetStringPinyins(cmd.chineseName);
        cmd.initials = PinyinHelper::GetStringInitialsList(cmd.chineseName);
        
        commands.push_back(cmd);
    }
    
    return commands;
}
