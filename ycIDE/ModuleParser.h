#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <Windows.h>

// 前向声明
struct LibraryCommand;

// ETAG 结构体（与易语言模块中的定义对应）
#pragma pack(push, 1)
struct ETAG {
    USHORT ID;
    BYTE Type1;
    BYTE Type2;
};
#pragma pack(pop)

// 易语言数据类型
enum class EDataType {
    Byte = 1,
    Short = 2,
    Int = 3,
    Long = 4,
    Float = 5,
    Double = 6,
    Bool = 7,
    Date = 8,
    Text = 9,
    ByteArray = 10,
    Custom = 100
};

// 变量属性
enum class EVariableAttr : USHORT {
    None = 0,
    Static = 1,
    Reference = 2,
    Array = 4,
    Optional = 8
};

// 子程序属性
enum class EMethodAttr {
    None = 0,
    Public = 1,
    Private = 2,
    Return = 4
};

// 事件类型
#define EVENT_METHOD    L"METHOD"      // 子程序
#define EVENT_DLL       L"DLL"         // DLL命令
#define EVENT_STRUCT    L"STRUCT"      // 数据类型
#define EVENT_GLOBAL    L"GLOBAL"      // 全局变量
#define EVENT_CONST     L"CONST"       // 常量
#define EVENT_CLASS     L"CLASS"       // 类
#define EVENT_RESOURCE  L"RESOURCE"    // 资源
#define EVENT_INFO      L"INFO"        // 模块信息
#define EVENT_LIB       L"LIB"         // 支持库
#define EVENT_EC        L"EC"          // 模块列表
#define EVENT_END       L"END"         // 解析结束

// 变量信息结构（简化版）
struct EVariable {
    std::wstring name;
    std::wstring type;
    std::wstring remark;
    bool isArray;
    bool isOptional;
    bool isReference;
};

// 子程序信息结构
struct EMethod {
    std::wstring name;
    std::wstring remark;
    std::wstring returnType;
    std::wstring className;  // 所属类名
    std::vector<EVariable> parameters;
    int attributes;
};

// DLL命令信息结构
struct EDllCommand {
    std::wstring showName;   // 显示名称
    std::wstring dllName;    // DLL名称
    std::wstring funcName;   // 函数名称
    std::wstring remark;     // 说明
    std::wstring returnType; // 返回值类型
    std::vector<EVariable> parameters;
};

// 数据类型信息结构
struct EDataStruct {
    std::wstring name;
    std::wstring remark;
    std::vector<EVariable> members;
};

// 模块信息结构
struct EModuleInfo {
    std::wstring projectName;
    std::wstring author;
    std::wstring remark;
    std::wstring version;
    std::wstring copyright;
};

// 模块解析器类
class ModuleParser {
public:
    ModuleParser();
    ~ModuleParser();

    // 初始化解析器
    bool Initialize(const std::wstring& eLangRootPath);
    
    // 加载并解析.ec模块文件
    bool LoadModule(const std::wstring& filePath, const std::wstring& password = L"");
    
    // 获取解析结果
    const std::vector<EMethod>& GetMethods() const { return methods_; }
    const std::vector<EDllCommand>& GetDllCommands() const { return dllCommands_; }
    const std::vector<EDataStruct>& GetDataTypes() const { return dataTypes_; }
    const EModuleInfo& GetModuleInfo() const { return moduleInfo_; }
    
    // 转换为LibraryCommand格式
    std::vector<LibraryCommand> ConvertToLibraryCommands(const std::wstring& moduleName);
    
    // 清空解析结果
    void Clear();
    
    // 获取最后的错误信息
    std::wstring GetLastError() const { return lastError_; }

private:
    // DLL函数指针类型定义（使用默认 __cdecl 调用约定）
    typedef void* (*PFN_ECodeParser_Create)(const wchar_t*);
    typedef void (*PFN_ECodeParser_Destroy)(void*);
    typedef void (*PFN_ECodeParser_AddEvent)(void*, const wchar_t*, void*);
    typedef int (*PFN_ECodeParser_Parse)(void*, const BYTE*, int, BOOL);
    typedef void* (*PFN_ECodeParser_FindEBase)(void*, int);
    typedef void (*PFN_FreePtr)(void*);
    typedef void (*PFN_FreeEBase)(void*);
    typedef wchar_t* (*PFN_Ptr2Str)(void*);
    typedef void* (*PFN_Ptr2Arr)(void*, int);
    
    // DLL模块句柄
    HMODULE hDll_;
    
    // DLL函数指针
    PFN_ECodeParser_Create pfnCreate_;
    PFN_ECodeParser_Destroy pfnDestroy_;
    PFN_ECodeParser_AddEvent pfnAddEvent_;
    PFN_ECodeParser_Parse pfnParse_;
    PFN_ECodeParser_FindEBase pfnFindEBase_;
    PFN_FreePtr pfnFreePtr_;
    PFN_FreeEBase pfnFreeEBase_;
    PFN_Ptr2Str pfnPtr2Str_;
    PFN_Ptr2Arr pfnPtr2Arr_;
    
    // 解析器实例
    void* parser_;
    
    // 解析结果
    std::vector<EMethod> methods_;
    std::vector<EDllCommand> dllCommands_;
    std::vector<EDataStruct> dataTypes_;
    std::vector<std::wstring> classNames_;
    EModuleInfo moduleInfo_;
    
    // 错误信息
    std::wstring lastError_;
    
    // 易语言安装根目录
    std::wstring eLangRoot_;
    
    // 加载DLL
    bool LoadDll();
    
    // 卸载DLL
    void UnloadDll();
    
    // 读取文件
    bool ReadFile(const std::wstring& filePath, std::vector<BYTE>& data);
    
    // 解密模块（如果需要）
    bool DecryptModule(std::vector<BYTE>& data, const std::wstring& password);
    
    // 事件回调函数（静态）
    static void __stdcall OnMethodEvent(void* parser, void* data);
    static void __stdcall OnDllEvent(void* parser, void* data);
    static void __stdcall OnStructEvent(void* parser, void* data);
    static void __stdcall OnGlobalEvent(void* parser, void* data);
    static void __stdcall OnConstEvent(void* parser, void* data);
    static void __stdcall OnClassEvent(void* parser, void* data);
    static void __stdcall OnResourceEvent(void* parser, void* data);
    static void __stdcall OnInfoEvent(void* parser, void* data);
    static void __stdcall OnLibEvent(void* parser, void* data);
    static void __stdcall OnECEvent(void* parser, void* data);
    static void __stdcall OnEndEvent(void* parser, void* data);
    
    // 获取当前实例（用于静态回调）
    static ModuleParser* currentInstance_;
    
    // 辅助函数：从指针读取字符串
    std::wstring ReadStringFromPtr(void* stringPtr);
    
    // 辅助函数：ETAG转整数
    static int ETagToInt(const ETAG& tag) {
        return tag.ID | (tag.Type1 << 16) | (tag.Type2 << 24);
    }
    
    // 辅助函数：整数转ETAG
    static ETAG IntToETag(int value) {
        ETAG tag;
        tag.ID = value & 0xFFFF;
        tag.Type1 = (value >> 16) & 0xFF;
        tag.Type2 = (value >> 24) & 0xFF;
        return tag;
    }
    
    // 辅助函数：数据类型转字符串
    std::wstring DataTypeToString(int dataType);
};
