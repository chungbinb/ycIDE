#pragma once

#include <string>
#include <vector>

// 前向声明
class KeywordManager;
class LibraryParser;

// Token 类型枚举
enum class EYTokenType {
    // 关键字
    KEYWORD_IF,          // 如果
    KEYWORD_ELSE,        // 否则
    KEYWORD_WHILE,       // 判断循环首
    KEYWORD_FOR,         // 计次循环首
    KEYWORD_DO,          // 到循环尾
    KEYWORD_BREAK,       // 跳出循环
    KEYWORD_CONTINUE,    // 继续循环
    KEYWORD_RETURN,      // 返回
    KEYWORD_SUB,         // 子程序
    KEYWORD_VAR,         // 变量
    KEYWORD_CONST,       // 常量
    KEYWORD_TRUE,        // 真
    KEYWORD_FALSE,       // 假
    
    // 数据类型关键字
    TYPE_BYTE,           // 字节型
    TYPE_SHORT,          // 短整数型
    TYPE_INT,            // 整数型
    TYPE_LONG,           // 长整数型
    TYPE_FLOAT,          // 小数型
    TYPE_DOUBLE,         // 双精度小数型
    TYPE_BOOL,           // 逻辑型
    TYPE_TEXT,           // 文本型
    TYPE_DATE,           // 日期时间型
    
    // 标识符和字面量
    IDENTIFIER,          // 标识符（变量名、函数名等）
    NUMBER,              // 数字字面量
    STRING,              // 字符串字面量
    
    // 运算符
    OP_ASSIGN,           // = (赋值)
    OP_PLUS,             // + (加)
    OP_MINUS,            // - (减)
    OP_MULTIPLY,         // × 或 * (乘)
    OP_DIVIDE,           // ÷ 或 / (除)
    OP_MOD,              // % (取余)
    OP_POWER,            // ^ (幂运算)
    
    // 比较运算符
    OP_EQ,               // = (等于)
    OP_NEQ,              // ≠ 或 <> (不等于)
    OP_GT,               // > (大于)
    OP_LT,               // < (小于)
    OP_GTE,              // >= (大于等于)
    OP_LTE,              // <= (小于等于)
    
    // 逻辑运算符
    OP_AND,              // 并且 或 且
    OP_OR,               // 或者 或 或
    OP_NOT,              // 非
    
    // 分隔符
    LPAREN,              // (
    RPAREN,              // )
    LBRACKET,            // [
    RBRACKET,            // ]
    LBRACE,              // {
    RBRACE,              // }
    COMMA,               // ,
    DOT,                 // .
    COLON,               // :
    SEMICOLON,           // ;
    
    // 特殊
    COMMENT,             // 注释
    NEWLINE,             // 换行
    WHITESPACE,          // 空白
    EOF_TOKEN,           // 文件结束
    UNKNOWN,             // 未知token
    TOKEN_ERROR          // 错误token
};

// Token 结构
struct Token {
    EYTokenType type;          // Token类型
    std::wstring value;      // Token的原始值
    int line;                // 行号（从1开始）
    int column;              // 列号（从1开始）
    int position;            // 在源代码中的位置（字符索引）
    int length;              // Token长度
    std::wstring errorMsg;   // 错误信息（当type为TOKEN_ERROR时）
    
    Token(EYTokenType t = EYTokenType::UNKNOWN, 
          const std::wstring& v = L"",
          int ln = 0, int col = 0, int pos = 0, int len = 0)
        : type(t), value(v), line(ln), column(col), position(pos), length(len) {}
    
    // 获取Token类型的字符串表示（用于调试）
    std::wstring GetTypeName() const;
    
    // 判断是否是关键字
    bool IsKeyword() const;
    
    // 判断是否是运算符
    bool IsOperator() const;
    
    // 判断是否是数据类型
    bool IsDataType() const;
};

// 词法分析器
class Lexer {
public:
    Lexer();
    ~Lexer();
    
    // 设置源代码
    void SetSource(const std::wstring& source);
    
    // 设置关键字和命令来源（可选，不设置则使用硬编码关键字）
    void SetKeywordManager(KeywordManager* km) { keywordManager_ = km; }
    void SetLibraryParser(LibraryParser* lp) { libraryParser_ = lp; }
    
    // 获取下一个Token
    Token NextToken();
    
    // 预览下一个Token（不移动位置）
    Token PeekToken();
    
    // 获取当前位置
    int GetPosition() const { return position_; }
    
    // 获取当前行号
    int GetLine() const { return line_; }
    
    // 获取当前列号
    int GetColumn() const { return column_; }
    
    // 重置到开头
    void Reset();
    
    // 一次性扫描所有Token（用于语法高亮等）
    std::vector<Token> ScanAll();
    
    // 检查是否到达文件末尾
    bool IsAtEnd() const { return position_ >= (int)source_.length(); }
    
private:
    std::wstring source_;    // 源代码
    int position_;           // 当前位置
    int line_;               // 当前行号
    
    // 外部数据源
    KeywordManager* keywordManager_;   // 关键字管理器（可选）
    LibraryParser* libraryParser_;     // 支持库解析器（可选）
    int column_;             // 当前列号
    int tokenStart_;         // 当前Token开始位置
    int tokenStartLine_;     // 当前Token开始行号
    int tokenStartColumn_;   // 当前Token开始列号
    
    // 辅助方法
    wchar_t Peek() const;                      // 查看当前字符
    wchar_t PeekNext() const;                  // 查看下一个字符
    wchar_t Advance();                         // 前进一个字符
    bool Match(wchar_t expected);              // 匹配并前进
    void SkipWhitespace();                     // 跳过空白字符
    
    // 扫描各种Token
    Token ScanNumber();                        // 扫描数字
    Token ScanString();                        // 扫描字符串
    Token ScanIdentifierOrKeyword();           // 扫描标识符或关键字
    Token ScanComment();                       // 扫描注释
    Token ScanOperator();                      // 扫描运算符
    
    // 辅助判断方法
    bool IsDigit(wchar_t c) const;            // 是否是数字
    bool IsLetter(wchar_t c) const;           // 是否是字母（含中文）
    bool IsChineseChar(wchar_t c) const;      // 是否是中文字符
    bool IsIdentifierStart(wchar_t c) const;  // 是否可以作为标识符开头
    bool IsIdentifierPart(wchar_t c) const;   // 是否可以作为标识符一部分
    
    // 关键字识别
    EYTokenType GetKeywordType(const std::wstring& word) const;
    
    // 创建Token
    Token MakeToken(EYTokenType type, const std::wstring& value = L"");
    Token MakeErrorToken(const std::wstring& message);
};
