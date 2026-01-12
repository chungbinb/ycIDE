#pragma once
#include <windows.h>
#include <string>
#include <set>
#include <map>
#include <vector>

// 前向声明
class KeywordManager;
class ProjectManager;

// 符号类型
enum class SymbolType {
    Constant,       // 常量
    GlobalVariable, // 全局变量
    DataType,       // 自定义数据类型
    DllCommand,     // DLL命令
    Subroutine      // 子程序
};

// 符号信息
struct SymbolInfo {
    std::wstring name;          // 符号名称
    SymbolType type;            // 符号类型
    std::wstring filePath;      // 所在文件路径
    int lineNumber;             // 行号（可选）
    
    SymbolInfo() : type(SymbolType::Constant), lineNumber(-1) {}
    SymbolInfo(const std::wstring& n, SymbolType t, const std::wstring& file)
        : name(n), type(t), filePath(file), lineNumber(-1) {}
};

// 名称验证器
// 负责检测名称冲突、关键词冲突、命名规范等
class NameValidator {
private:
    KeywordManager* m_pKeywordManager;
    ProjectManager* m_pProjectManager;
    
    // 全局符号表（名称 -> 符号信息列表，支持跨文件查重）
    std::map<std::wstring, std::vector<SymbolInfo>> m_globalSymbols;
    
    // 缓存标志
    bool m_symbolsCached;
    
public:
    NameValidator();
    ~NameValidator();
    
    // === 管理器设置 ===
    void SetKeywordManager(KeywordManager* km) { m_pKeywordManager = km; }
    void SetProjectManager(ProjectManager* pm) { m_pProjectManager = pm; }
    
    // === 符号表管理 ===
    
    // 刷新全局符号表（从项目管理器扫描所有文件）
    void RefreshSymbolTable();
    
    // 添加符号
    void AddSymbol(const SymbolInfo& symbol);
    
    // 移除符号
    void RemoveSymbol(const std::wstring& name, SymbolType type, const std::wstring& filePath);
    
    // 移除文件的所有符号
    void RemoveFileSymbols(const std::wstring& filePath);
    
    // === 验证方法 ===
    
    // 验证名称是否有效（返回空字符串表示有效，否则返回错误信息）
    std::wstring ValidateName(const std::wstring& name, SymbolType type, const std::wstring& currentFile = L"");
    
    // 检查是否为关键词
    bool IsKeyword(const std::wstring& name);
    
    // 检查名称是否已存在（排除当前文件）
    bool IsNameExists(const std::wstring& name, SymbolType type, const std::wstring& currentFile = L"");
    
    // 获取名称冲突的符号列表
    std::vector<SymbolInfo> GetConflictingSymbols(const std::wstring& name, const std::wstring& currentFile = L"");
    
    // 生成唯一名称（如果名称冲突，自动添加数字后缀）
    std::wstring GenerateUniqueName(const std::wstring& baseName, SymbolType type, const std::wstring& currentFile = L"");
    
    // === 命名规范检查 ===
    
    // 检查名称是否符合易语言命名规范
    bool IsValidIdentifier(const std::wstring& name);
    
    // 检查名称长度是否合法
    bool IsValidLength(const std::wstring& name);
    
    // === 工具方法 ===
    
    // 获取符号类型的显示名称
    static std::wstring GetSymbolTypeName(SymbolType type);
    
private:
    // 从文件中提取符号
    void ExtractSymbolsFromFile(const std::wstring& filePath);
    
    // 解析 .ecl 常量文件
    void ParseConstantFile(const std::wstring& filePath);
    
    // 解析 .eal 全局变量文件
    void ParseGlobalVarFile(const std::wstring& filePath);
    
    // 解析 .edl 数据类型文件
    void ParseDataTypeFile(const std::wstring& filePath);
    
    // 解析 .ell DLL命令文件
    void ParseDllFile(const std::wstring& filePath);
};
