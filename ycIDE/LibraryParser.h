#pragma once
#include <string>
#include <vector>
#include <map>
#include "FneParser.h"

// 支持库命令参数
struct LibraryParameter {
    std::wstring name;              // 参数名称
    std::wstring type;              // 参数类型（如：逻辑型）
    std::wstring typeWithEnglish;   // 参数类型带英文（如：逻辑型（bool））
    std::wstring description;       // 参数说明
    bool optional;                  // 是否可选
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
    bool needsFlowLine;        // 是否需要流程线（流程控制命令）
    bool needsBrackets;        // 是否需要括号
};

// 支持库数据类型
struct LibraryDataType {
    std::wstring name;         // 中文名称
    std::wstring englishName;  // 英文名称
    std::wstring description;  // 说明
    std::wstring library;      // 所属支持库
};

// 窗口组件信息（供设计器使用）
struct WindowUnitInfo {
    std::wstring name;           // 组件名称
    std::wstring englishName;    // 英文名称
    std::wstring description;    // 组件说明
    std::wstring libraryName;    // 所属支持库
    std::wstring category;       // 组件分类
    bool isContainer;            // 是否容器
    std::vector<FnePropertyInfo> properties;  // 属性列表
    std::vector<FneEventInfo> events;         // 事件列表
};

// 支持库解析器
class LibraryParser {
public:
    static LibraryParser& GetInstance();
    
    // 加载 .fne 支持库文件（替代原来的 LoadLibraryFile）
    bool LoadFneLibrary(const std::wstring& fnePath);
    
    // 从 FNE 解析器添加命令
    void AddCommandFromFne(const std::wstring& cmdName, const std::wstring& description,
                          const std::wstring& returnType, const std::vector<std::wstring>& params,
                          const std::wstring& libraryName);
    
    // 获取所有命令
    const std::vector<LibraryCommand>& GetCommands() const { return commands; }
    
    // 获取所有数据类型
    const std::vector<LibraryDataType>& GetDataTypes() const { return dataTypes; }
    
    // 获取所有窗口组件
    const std::vector<WindowUnitInfo>& GetWindowUnits() const { return windowUnits; }
    
    // 获取所有数据类型名称（包括基础类型）
    std::vector<std::wstring> GetAllDataTypeNames() const;
    
    // 根据中文名或英文名查找命令
    const LibraryCommand* FindCommand(const std::wstring& name) const;
    
    // 根据名称查找窗口组件
    const WindowUnitInfo* FindWindowUnit(const std::wstring& name) const;
    
    // 获取命令补全列表
    std::vector<std::wstring> GetCompletions(const std::wstring& input) const;
    
    // 获取命令总数
    size_t GetCommandCount() const { return commands.size(); }
    
    // 获取窗口组件总数
    size_t GetWindowUnitCount() const { return windowUnits.size(); }
    
    // 获取最后的错误信息
    std::wstring GetLastError() const { return lastError_; }
    
private:
    LibraryParser() = default;
    
    std::vector<LibraryCommand> commands;
    std::vector<LibraryDataType> dataTypes;     // 支持库数据类型
    std::vector<WindowUnitInfo> windowUnits;    // 窗口组件列表
    std::map<std::wstring, size_t> nameIndex;   // 名称到命令索引的映射
    std::map<std::wstring, size_t> unitIndex;   // 名称到组件索引的映射
    std::wstring lastError_;  // 最后的错误信息
};
