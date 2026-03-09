#include "Lexer.h"
#include <map>
#include <cctype>
#include "Keyword.h"
#include "LibraryParser.h"

// Token类型名称映射（用于调试）
std::wstring Token::GetTypeName() const {
    static const std::map<EYTokenType, std::wstring> typeNames = {
        {EYTokenType::KEYWORD_IF, L"KEYWORD_IF"},
        {EYTokenType::KEYWORD_ELSE, L"KEYWORD_ELSE"},
        {EYTokenType::KEYWORD_WHILE, L"KEYWORD_WHILE"},
        {EYTokenType::KEYWORD_FOR, L"KEYWORD_FOR"},
        {EYTokenType::IDENTIFIER, L"IDENTIFIER"},
        {EYTokenType::NUMBER, L"NUMBER"},
        {EYTokenType::STRING, L"STRING"},
        {EYTokenType::OP_ASSIGN, L"OP_ASSIGN"},
        {EYTokenType::OP_PLUS, L"OP_PLUS"},
        {EYTokenType::OP_MINUS, L"OP_MINUS"},
        {EYTokenType::COMMENT, L"COMMENT"},
        {EYTokenType::NEWLINE, L"NEWLINE"},
        {EYTokenType::EOF_TOKEN, L"EOF"},
        {EYTokenType::TOKEN_ERROR, L"ERROR"},
        // 可以继续添加其他类型...
    };
    
    auto it = typeNames.find(type);
    if (it != typeNames.end()) {
        return it->second;
    }
    return L"UNKNOWN";
}

bool Token::IsKeyword() const {
    return type >= EYTokenType::KEYWORD_IF && type <= EYTokenType::KEYWORD_CONST;
}

bool Token::IsOperator() const {
    return type >= EYTokenType::OP_ASSIGN && type <= EYTokenType::OP_NOT;
}

bool Token::IsDataType() const {
    return type >= EYTokenType::TYPE_BYTE && type <= EYTokenType::TYPE_DATE;
}

// ========== Lexer 实现 ==========

Lexer::Lexer()
    : position_(0)
    , line_(1)
    , column_(1)
    , tokenStart_(0)
    , tokenStartLine_(1)
    , tokenStartColumn_(1)
    , keywordManager_(nullptr)
    , libraryParser_(nullptr)
{
}

Lexer::~Lexer() {
}

void Lexer::SetSource(const std::wstring& source) {
    source_ = source;
    Reset();
}

void Lexer::Reset() {
    position_ = 0;
    line_ = 1;
    column_ = 1;
    tokenStart_ = 0;
    tokenStartLine_ = 1;
    tokenStartColumn_ = 1;
}

wchar_t Lexer::Peek() const {
    if (IsAtEnd()) return L'\0';
    return source_[position_];
}

wchar_t Lexer::PeekNext() const {
    if (position_ + 1 >= (int)source_.length()) return L'\0';
    return source_[position_ + 1];
}

wchar_t Lexer::Advance() {
    if (IsAtEnd()) return L'\0';
    
    wchar_t c = source_[position_++];
    
    if (c == L'\n') {
        line_++;
        column_ = 1;
    } else {
        column_++;
    }
    
    return c;
}

bool Lexer::Match(wchar_t expected) {
    if (IsAtEnd()) return false;
    if (source_[position_] != expected) return false;
    Advance();
    return true;
}

void Lexer::SkipWhitespace() {
    while (!IsAtEnd()) {
        wchar_t c = Peek();
        if (c == L' ' || c == L'\t' || c == L'\r') {
            Advance();
        } else {
            break;
        }
    }
}

bool Lexer::IsDigit(wchar_t c) const {
    return c >= L'0' && c <= L'9';
}

bool Lexer::IsLetter(wchar_t c) const {
    return (c >= L'a' && c <= L'z') || 
           (c >= L'A' && c <= L'Z') || 
           IsChineseChar(c);
}

bool Lexer::IsChineseChar(wchar_t c) const {
    // 常用汉字Unicode范围
    return (c >= 0x4E00 && c <= 0x9FFF) ||  // CJK统一汉字
           (c >= 0x3400 && c <= 0x4DBF) ||  // CJK扩展A
           (c >= 0x20000 && c <= 0x2A6DF);  // CJK扩展B
}

bool Lexer::IsIdentifierStart(wchar_t c) const {
    return IsLetter(c) || c == L'_';
}

bool Lexer::IsIdentifierPart(wchar_t c) const {
    return IsLetter(c) || IsDigit(c) || c == L'_';
}

Token Lexer::MakeToken(EYTokenType type, const std::wstring& value) {
    std::wstring tokenValue = value.empty() 
        ? source_.substr(tokenStart_, position_ - tokenStart_)
        : value;
    
    return Token(type, tokenValue, tokenStartLine_, tokenStartColumn_, 
                 tokenStart_, position_ - tokenStart_);
}

Token Lexer::MakeErrorToken(const std::wstring& message) {
    Token token = MakeToken(EYTokenType::TOKEN_ERROR);
    token.errorMsg = message;
    return token;
}

EYTokenType Lexer::GetKeywordType(const std::wstring& word) const {
    // 基础关键字（内置,不需要从支持库加载）
    static const std::map<std::wstring, EYTokenType> builtinKeywords = {
        // 控制流程关键字
        {L"如果", EYTokenType::KEYWORD_IF},
        {L"如果真", EYTokenType::KEYWORD_IF},
        {L"否则", EYTokenType::KEYWORD_ELSE},
        {L"判断循环首", EYTokenType::KEYWORD_WHILE},
        {L"计次循环首", EYTokenType::KEYWORD_FOR},
        {L"到循环尾", EYTokenType::KEYWORD_DO},
        {L"跳出循环", EYTokenType::KEYWORD_BREAK},
        {L"继续循环", EYTokenType::KEYWORD_CONTINUE},
        {L"返回", EYTokenType::KEYWORD_RETURN},
        
        // 声明关键字
        {L"子程序", EYTokenType::KEYWORD_SUB},
        {L"变量", EYTokenType::KEYWORD_VAR},
        {L"局部变量", EYTokenType::KEYWORD_VAR},
        {L"参数", EYTokenType::KEYWORD_VAR},
        {L"常量", EYTokenType::KEYWORD_CONST},
        
        // 逻辑值
        {L"真", EYTokenType::KEYWORD_TRUE},
        {L"假", EYTokenType::KEYWORD_FALSE},
        
        // 基础数据类型（内置，不依赖支持库）
        {L"字节型", EYTokenType::TYPE_BYTE},
        {L"短整数型", EYTokenType::TYPE_SHORT},
        {L"整数型", EYTokenType::TYPE_INT},
        {L"长整数型", EYTokenType::TYPE_LONG},
        {L"小数型", EYTokenType::TYPE_FLOAT},
        {L"双精度小数型", EYTokenType::TYPE_DOUBLE},
        {L"逻辑型", EYTokenType::TYPE_BOOL},
        {L"文本型", EYTokenType::TYPE_TEXT},
        {L"日期时间型", EYTokenType::TYPE_DATE},
    };
    
    // 先检查内置关键字
    auto it = builtinKeywords.find(word);
    if (it != builtinKeywords.end()) {
        return it->second;
    }
    
    // 检查是否是数据类型（从支持库获取）
    if (libraryParser_) {
        auto dataTypes = libraryParser_->GetAllDataTypeNames();
        for (const auto& typeName : dataTypes) {
            if (typeName == word) {
                // 根据类型名返回对应的Token类型
                if (word == L"字节型") return EYTokenType::TYPE_BYTE;
                if (word == L"短整数型") return EYTokenType::TYPE_SHORT;
                if (word == L"整数型") return EYTokenType::TYPE_INT;
                if (word == L"长整数型") return EYTokenType::TYPE_LONG;
                if (word == L"小数型") return EYTokenType::TYPE_FLOAT;
                if (word == L"双精度小数型") return EYTokenType::TYPE_DOUBLE;
                if (word == L"逻辑型") return EYTokenType::TYPE_BOOL;
                if (word == L"文本型") return EYTokenType::TYPE_TEXT;
                if (word == L"日期时间型") return EYTokenType::TYPE_DATE;
                // 其他自定义数据类型也返回通用类型
                return EYTokenType::TYPE_INT;  // 默认作为类型处理
            }
        }
    }
    
    // 检查是否是支持库命令（从KeywordManager获取）
    if (keywordManager_) {
        const auto* keyword = keywordManager_->GetKeyword(word);
        if (keyword) {
            // 根据关键词类型返回Token类型
            switch (keyword->type) {
                case KW_CONTROL: return EYTokenType::KEYWORD_IF;  // 控制流程
                case KW_STATEMENT: return EYTokenType::KEYWORD_VAR; // 语句
                default: break;
            }
        }
    }
    
    // 检查支持库命令
    if (libraryParser_) {
        const auto* cmd = libraryParser_->FindCommand(word);
        if (cmd) {
            // 支持库命令当作标识符处理，在语法分析阶段再判断
            return EYTokenType::IDENTIFIER;
        }
    }
    
    return EYTokenType::IDENTIFIER;
}

Token Lexer::ScanNumber() {
    // 扫描整数部分
    while (IsDigit(Peek())) {
        Advance();
    }
    
    // 检查小数点
    if (Peek() == L'.' && IsDigit(PeekNext())) {
        Advance(); // 消耗'.'
        while (IsDigit(Peek())) {
            Advance();
        }
    }
    
    // 检查科学计数法 (如 1.5e10)
    if (Peek() == L'e' || Peek() == L'E') {
        Advance();
        if (Peek() == L'+' || Peek() == L'-') {
            Advance();
        }
        while (IsDigit(Peek())) {
            Advance();
        }
    }
    
    return MakeToken(EYTokenType::NUMBER);
}

Token Lexer::ScanString() {
    // 易语言字符串用引号括起来："字符串"
    wchar_t quote = Advance(); // 消耗开始引号
    
    std::wstring value;
    while (!IsAtEnd() && Peek() != quote) {
        if (Peek() == L'\n') {
            return MakeErrorToken(L"字符串不能跨行");
        }
        
        // 处理转义字符
        if (Peek() == L'\\') {
            Advance();
            if (!IsAtEnd()) {
                wchar_t escaped = Advance();
                switch (escaped) {
                    case L'n': value += L'\n'; break;
                    case L't': value += L'\t'; break;
                    case L'r': value += L'\r'; break;
                    case L'\\': value += L'\\'; break;
                    case L'"': value += L'"'; break;
                    default: value += escaped; break;
                }
            }
        } else {
            value += Advance();
        }
    }
    
    if (IsAtEnd()) {
        return MakeErrorToken(L"字符串未闭合");
    }
    
    Advance(); // 消耗结束引号
    return MakeToken(EYTokenType::STRING, value);
}

Token Lexer::ScanIdentifierOrKeyword() {
    while (IsIdentifierPart(Peek())) {
        Advance();
    }
    
    std::wstring text = source_.substr(tokenStart_, position_ - tokenStart_);
    EYTokenType type = GetKeywordType(text);
    
    return MakeToken(type);
}

Token Lexer::ScanDotKeyword() {
    // 消耗点号
    Advance();
    
    // 扫描后面的标识符
    while (IsIdentifierPart(Peek())) {
        Advance();
    }
    
    // 获取不带点的关键字部分
    std::wstring fullText = source_.substr(tokenStart_, position_ - tokenStart_);
    std::wstring keyword = fullText.substr(1); // 去掉点号
    
    // 处理易语言声明关键字
    if (keyword == L"版本") {
        // .版本 声明，跳过整行
        while (!IsAtEnd() && Peek() != L'\n') {
            Advance();
        }
        return MakeToken(EYTokenType::COMMENT); // 当作注释跳过
    }
    if (keyword == L"程序集") {
        // .程序集 声明，跳过整行
        while (!IsAtEnd() && Peek() != L'\n') {
            Advance();
        }
        return MakeToken(EYTokenType::COMMENT); // 当作注释跳过
    }
    if (keyword == L"子程序") {
        return MakeToken(EYTokenType::KEYWORD_SUB, keyword);
    }
    if (keyword == L"变量" || keyword == L"局部变量" || keyword == L"参数") {
        return MakeToken(EYTokenType::KEYWORD_VAR, keyword);
    }
    if (keyword == L"常量") {
        return MakeToken(EYTokenType::KEYWORD_CONST, keyword);
    }
    if (keyword == L"如果" || keyword == L"如果真") {
        return MakeToken(EYTokenType::KEYWORD_IF, keyword);
    }
    if (keyword == L"否则") {
        return MakeToken(EYTokenType::KEYWORD_ELSE, keyword);
    }
    if (keyword == L"判断循环首") {
        return MakeToken(EYTokenType::KEYWORD_WHILE, keyword);
    }
    if (keyword == L"计次循环首") {
        return MakeToken(EYTokenType::KEYWORD_FOR, keyword);
    }
    if (keyword == L"到循环尾") {
        return MakeToken(EYTokenType::KEYWORD_DO, keyword);
    }
    if (keyword == L"跳出循环") {
        return MakeToken(EYTokenType::KEYWORD_BREAK, keyword);
    }
    if (keyword == L"继续循环") {
        return MakeToken(EYTokenType::KEYWORD_CONTINUE, keyword);
    }
    if (keyword == L"返回") {
        return MakeToken(EYTokenType::KEYWORD_RETURN, keyword);
    }
    
    // 其他 .xxx 格式作为标识符处理
    return MakeToken(EYTokenType::IDENTIFIER, keyword);
}

Token Lexer::ScanComment() {
    // 易语言注释：'这是注释 或 //这是注释
    wchar_t first = Peek();
    
    if (first == L'\'') {
        // 单引号注释，到行尾
        Advance();  // 消耗单引号
        while (!IsAtEnd() && Peek() != L'\n') {
            Advance();
        }
        return MakeToken(EYTokenType::COMMENT);
    }
    else if (first == L'/' && PeekNext() == L'/') {
        // 双斜杠注释，到行尾
        Advance(); // 第一个/
        Advance(); // 第二个/
        while (!IsAtEnd() && Peek() != L'\n') {
            Advance();
        }
        return MakeToken(EYTokenType::COMMENT);
    }
    
    // 不应该到这里，但为了安全还是要前进
    Advance();
    return MakeErrorToken(L"未知的注释格式");
}

Token Lexer::ScanOperator() {
    wchar_t c = Peek();
    
    switch (c) {
        case L'+': Advance(); return MakeToken(EYTokenType::OP_PLUS);
        case L'-': Advance(); return MakeToken(EYTokenType::OP_MINUS);
        case L'*':
        case L'×': Advance(); return MakeToken(EYTokenType::OP_MULTIPLY);
        case L'/':
        case L'÷': 
            // 需要区分除法和注释
            if (c == L'/' && PeekNext() == L'/') {
                return ScanComment();
            }
            Advance(); 
            return MakeToken(EYTokenType::OP_DIVIDE);
        case L'%': Advance(); return MakeToken(EYTokenType::OP_MOD);
        case L'^': Advance(); return MakeToken(EYTokenType::OP_POWER);
        case L'\\': Advance(); return MakeToken(EYTokenType::OP_DIVIDE);  // 反斜杠也当作除法（易语言兼容）
        
        case L'=':
            Advance();
            return MakeToken(EYTokenType::OP_EQ);
            
        case L'<':
            Advance();
            if (Match(L'=')) return MakeToken(EYTokenType::OP_LTE);
            if (Match(L'>')) return MakeToken(EYTokenType::OP_NEQ);
            return MakeToken(EYTokenType::OP_LT);
            
        case L'>':
            Advance();
            if (Match(L'=')) return MakeToken(EYTokenType::OP_GTE);
            return MakeToken(EYTokenType::OP_GT);
            
        case L'≤': Advance(); return MakeToken(EYTokenType::OP_LTE);  // Unicode 小于等于
        case L'≥': Advance(); return MakeToken(EYTokenType::OP_GTE);  // Unicode 大于等于
        case L'≠': Advance(); return MakeToken(EYTokenType::OP_NEQ);  // Unicode 不等于
        
        case L'(': Advance(); return MakeToken(EYTokenType::LPAREN);
        case L')': Advance(); return MakeToken(EYTokenType::RPAREN);
        case L'[': Advance(); return MakeToken(EYTokenType::LBRACKET);
        case L']': Advance(); return MakeToken(EYTokenType::RBRACKET);
        case L'{': Advance(); return MakeToken(EYTokenType::LBRACE);
        case L'}': Advance(); return MakeToken(EYTokenType::RBRACE);
        case L',': 
        case L'，':  // 中文逗号也支持
            Advance(); return MakeToken(EYTokenType::COMMA);
        case L'.': 
        case L'。':  // 中文句号作为成员访问符
            Advance(); return MakeToken(EYTokenType::DOT);
        // 冒号在易语言中无语法意义，已在NextToken中被跳过
        case L';': 
        case L'；':  // 中文分号
            Advance(); return MakeToken(EYTokenType::SEMICOLON);
        case L'#':   // 井号 - 易语言中可能用于特殊用途
            Advance(); return MakeToken(EYTokenType::IDENTIFIER, L"#");  // 暂时当作标识符
        case L'&':   // 按位与
            Advance(); return MakeToken(EYTokenType::OP_AND);
        case L'|':   // 按位或
            Advance(); return MakeToken(EYTokenType::OP_OR);
        case L'!':   // 逻辑非
            Advance();
            if (Match(L'=')) return MakeToken(EYTokenType::OP_NEQ);  // !=
            return MakeToken(EYTokenType::OP_NOT);
        case L'@':   // @ 符号
            Advance(); return MakeToken(EYTokenType::IDENTIFIER, L"@");
        case L'_':   // 下划线开头
            return ScanIdentifierOrKeyword();
            
        default:
            // 重要：必须 Advance() 跳过未知字符，否则会导致无限循环
            Advance();
            return MakeErrorToken(L"未知的运算符: " + std::wstring(1, c));
    }
}

// 判断是否为应该忽略的特殊字符
// 包括零宽字符和易语言中无语法意义的字符
static bool IsIgnorableChar(wchar_t c) {
    // 零宽字符
    if (c == 0x200B  // ZERO WIDTH SPACE
        || c == 0x200C  // ZERO WIDTH NON-JOINER
        || c == 0x200D  // ZERO WIDTH JOINER
        || c == 0x2060  // WORD JOINER
        || c == 0xFEFF  // ZERO WIDTH NO-BREAK SPACE (BOM)
        || c == 0x00AD  // SOFT HYPHEN
        || c == 0x180E  // MONGOLIAN VOWEL SEPARATOR
        || c == 0x202A  // LEFT-TO-RIGHT EMBEDDING
        || c == 0x202B  // RIGHT-TO-LEFT EMBEDDING
        || c == 0x202C  // POP DIRECTIONAL FORMATTING
        || c == 0x202D  // LEFT-TO-RIGHT OVERRIDE
        || c == 0x202E) // RIGHT-TO-LEFT OVERRIDE
    {
        return true;
    }
    // 冒号在易语言中通常无语法意义，可能是用户从其他地方复制代码时带入的格式字符
    if (c == L':' || c == L'：') {
        return true;
    }
    return false;
}

Token Lexer::NextToken() {
    // 跳过空白字符和可忽略字符（但不跳过换行）
    while (!IsAtEnd()) {
        wchar_t c = Peek();
        if (c == L' ' || c == L'\t' || c == L'\r' || IsIgnorableChar(c)) {
            Advance();
        } else {
            break;
        }
    }
    
    // 记录Token开始位置
    tokenStart_ = position_;
    tokenStartLine_ = line_;
    tokenStartColumn_ = column_;
    
    if (IsAtEnd()) {
        return MakeToken(EYTokenType::EOF_TOKEN);
    }
    
    wchar_t c = Peek();
    
    // 换行符
    if (c == L'\n') {
        Advance();
        return MakeToken(EYTokenType::NEWLINE);
    }
    
    // 注释
    if (c == L'\'' || (c == L'/' && PeekNext() == L'/')) {
        return ScanComment();
    }
    
    // 数字
    if (IsDigit(c)) {
        return ScanNumber();
    }
    
    // 字符串
    if (c == L'"' || c == L'"' || c == L'"') {
        return ScanString();
    }
    
    // 以 . 开头的关键字（易语言语法：.子程序, .变量, .版本 等）
    if (c == L'.') {
        // 检查是否是 .关键字 格式
        if (position_ + 1 < (int)source_.length() && IsIdentifierStart(source_[position_ + 1])) {
            return ScanDotKeyword();
        }
    }
    
    // 标识符或关键字
    if (IsIdentifierStart(c)) {
        return ScanIdentifierOrKeyword();
    }
    
    // 运算符和分隔符
    return ScanOperator();
}

Token Lexer::PeekToken() {
    // 保存当前状态
    int savedPos = position_;
    int savedLine = line_;
    int savedCol = column_;
    
    Token token = NextToken();
    
    // 恢复状态
    position_ = savedPos;
    line_ = savedLine;
    column_ = savedCol;
    
    return token;
}

std::vector<Token> Lexer::ScanAll() {
    std::vector<Token> tokens;
    Reset();
    
    while (!IsAtEnd()) {
        Token token = NextToken();
        tokens.push_back(token);
        
        if (token.type == EYTokenType::EOF_TOKEN || token.type == EYTokenType::TOKEN_ERROR) {
            break;
        }
    }
    
    return tokens;
}
