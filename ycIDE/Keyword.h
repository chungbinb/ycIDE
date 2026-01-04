#pragma once
#include <string>
#include <vector>
#include <map>
#include "LibraryParser.h"

// 关键词类型
enum KeywordType {
    KW_CONTROL,      // 控制流程（如果、判断、循环等）
    KW_STATEMENT,    // 语句（变量、参数等）
    KW_OPERATOR,     // 操作符
    KW_TYPE,         // 数据类型
    KW_OTHER         // 其他
};

// 关键词结构
struct Keyword {
    std::wstring chinese;        // 中文关键词
    std::vector<std::wstring> aliases;  // 英文别名（如 ruguo, rg, if）
    KeywordType type;            // 类型
    std::wstring template_;      // 自动补全模板（如 "如果()"）
    std::wstring description;    // 描述
    bool needsBrackets;          // 是否需要括号
    bool needsFlowLine;          // 是否需要绘制流程线
    std::vector<std::wstring> pinyin;         // 全拼（支持多音字）
    std::vector<std::wstring> initials;       // 首字母（支持多音字）
};

// 自动补全建议
struct CompletionItem {
    std::wstring displayText;    // 显示文本
    std::wstring insertText;     // 插入文本
    std::wstring description;    // 描述
    KeywordType type;            // 类型
    bool isLibraryCommand;       // 是否来自支持库
    int score = 0;               // 匹配分数（用于排序）
};

// 关键词管理器
class KeywordManager {
public:
    static KeywordManager& GetInstance();
    
    // 初始化关键词库
    void Initialize();
    
    // 根据输入获取补全建议
    std::vector<CompletionItem> GetCompletions(const std::wstring& input);
    
    // 检查是否是关键词别名
    bool IsAlias(const std::wstring& text);
    
    // 获取关键词的中文形式
    std::wstring GetChineseKeyword(const std::wstring& alias);
    
    // 获取关键词信息
    const Keyword* GetKeyword(const std::wstring& chinese);
    
    // 检查关键词是否需要流程线
    bool NeedsFlowLine(const std::wstring& chinese);
    
    // 加载支持库命令
    bool LoadLibraryCommands(const std::wstring& libraryFilePath);
    
    // 获取支持库命令信息
    const LibraryCommand* GetLibraryCommand(const std::wstring& name);
    
private:
    KeywordManager() {}
    KeywordManager(const KeywordManager&) = delete;
    KeywordManager& operator=(const KeywordManager&) = delete;
    
    std::vector<Keyword> keywords;
    std::map<std::wstring, std::wstring> aliasMap;  // 别名 -> 中文映射
};
