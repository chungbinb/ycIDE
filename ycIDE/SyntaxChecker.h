#pragma once
#include "Lexer.h"
#include "Parser.h"
#include <vector>
#include <string>

// 前向声明
class KeywordManager;
class LibraryParser;

// 语法检查结果
struct SyntaxCheckResult {
    bool hasErrors;                           // 是否有错误
    std::vector<SyntaxError> errors;          // 错误列表
    std::vector<Token> tokens;                // Token列表（可选，用于语法高亮）
};

// 语法检查器
// 用于在编辑器中实时检查语法错误
class SyntaxChecker {
public:
    SyntaxChecker();
    ~SyntaxChecker();
    
    // 设置关键字和命令来源
    void SetKeywordManager(KeywordManager* km);
    void SetLibraryParser(LibraryParser* lp);
    
    // 检查代码语法
    SyntaxCheckResult Check(const std::wstring& source);
    
    // 检查单行代码（快速检查）
    std::vector<SyntaxError> CheckLine(const std::wstring& line, int lineNumber);
    
    // 获取Token列表（用于语法高亮）
    std::vector<Token> GetTokens(const std::wstring& source);
    
    // 检查是否是有效的标识符
    static bool IsValidIdentifier(const std::wstring& name);
    
    // 检查是否是关键字
    bool IsKeyword(const std::wstring& word) const;
    
    // 检查是否是数据类型
    bool IsDataType(const std::wstring& word) const;
    
    // 检查是否是支持库命令
    bool IsLibraryCommand(const std::wstring& word) const;
    
private:
    Lexer lexer_;
    Parser parser_;
    KeywordManager* keywordManager_;
    LibraryParser* libraryParser_;
    
    // 辅助方法
    void InitializeLexer();
};
