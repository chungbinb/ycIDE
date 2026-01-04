#pragma once
#include <string>
#include <vector>
#include <map>

// 支持库命令参数
struct LibraryParameter {
    std::wstring name;         // 参数名称
    std::wstring type;         // 参数类型
    std::wstring description;  // 参数说明
    bool optional;             // 是否可选
};

// 支持库命令
struct LibraryCommand {
    std::wstring chineseName;  // 中文名称
    std::wstring englishName;  // 英文名称
    std::wstring category;     // 命令类别
    std::wstring returnType;   // 返回值类型
    std::wstring description;  // 命令说明
    std::wstring library;      // 所属支持库
    std::vector<LibraryParameter> parameters;  // 参数列表
    std::vector<std::wstring> pinyin;       // 全拼（支持多音字）
    std::vector<std::wstring> initials;     // 首字母（支持多音字）
};

// 支持库解析器
class LibraryParser {
public:
    static LibraryParser& GetInstance();
    
    // 加载支持库文本文件
    bool LoadLibraryFile(const std::wstring& filePath);
    
    // 模块加载功能已移除
    // bool LoadModuleFile(const std::wstring& filePath, const std::wstring& password = L"");
    
    // 从 FNE 解析器添加命令
    void AddCommandFromFne(const std::wstring& cmdName, const std::wstring& description,
                          const std::wstring& returnType, const std::vector<std::wstring>& params,
                          const std::wstring& libraryName);
    
    // 获取所有命令
    const std::vector<LibraryCommand>& GetCommands() const { return commands; }
    
    // 根据中文名或英文名查找命令
    const LibraryCommand* FindCommand(const std::wstring& name) const;
    
    // 获取命令补全列表
    std::vector<std::wstring> GetCompletions(const std::wstring& input) const;
    
    // 获取命令总数
    size_t GetCommandCount() const { return commands.size(); }
    
    // 获取最后的错误信息
    std::wstring GetLastError() const { return lastError_; }
    
private:
    LibraryParser() = default;
    
    std::vector<LibraryCommand> commands;
    std::map<std::wstring, size_t> nameIndex;  // 名称到命令索引的映射
    std::wstring lastError_;  // 最后的错误信息
};
