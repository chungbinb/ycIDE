#include "SyntaxChecker.h"
#include "Keyword.h"
#include "LibraryParser.h"
#include <algorithm>

SyntaxChecker::SyntaxChecker()
    : keywordManager_(nullptr)
    , libraryParser_(nullptr)
{
}

SyntaxChecker::~SyntaxChecker() {
}

void SyntaxChecker::SetKeywordManager(KeywordManager* km) {
    keywordManager_ = km;
    InitializeLexer();
}

void SyntaxChecker::SetLibraryParser(LibraryParser* lp) {
    libraryParser_ = lp;
    InitializeLexer();
}

void SyntaxChecker::InitializeLexer() {
    lexer_.SetKeywordManager(keywordManager_);
    lexer_.SetLibraryParser(libraryParser_);
}

SyntaxCheckResult SyntaxChecker::Check(const std::wstring& source) {
    SyntaxCheckResult result;
    result.hasErrors = false;
    
    // 执行语法检查
    result.errors = parser_.CheckSyntax(source);
    result.hasErrors = !result.errors.empty();
    
    // 获取Token列表（可选，用于语法高亮）
    if (!result.hasErrors) {
        result.tokens = GetTokens(source);
    }
    
    return result;
}

std::vector<SyntaxError> SyntaxChecker::CheckLine(const std::wstring& line, int lineNumber) {
    std::vector<SyntaxError> errors;
    
    // 简单的单行检查
    lexer_.SetSource(line);
    
    Token token = lexer_.NextToken();
    while (token.type != EYTokenType::EOF_TOKEN) {
        if (token.type == EYTokenType::TOKEN_ERROR) {
            SyntaxError error(token.errorMsg, lineNumber, token.column);
            errors.push_back(error);
        }
        token = lexer_.NextToken();
    }
    
    return errors;
}

std::vector<Token> SyntaxChecker::GetTokens(const std::wstring& source) {
    lexer_.SetSource(source);
    return lexer_.ScanAll();
}

bool SyntaxChecker::IsValidIdentifier(const std::wstring& name) {
    if (name.empty()) return false;
    
    // 第一个字符必须是字母或下划线
    wchar_t first = name[0];
    bool validFirst = (first >= L'a' && first <= L'z') ||
                     (first >= L'A' && first <= L'Z') ||
                     (first >= 0x4E00 && first <= 0x9FFF) ||  // 中文字符
                     first == L'_';
    
    if (!validFirst) return false;
    
    // 后续字符可以是字母、数字或下划线
    for (size_t i = 1; i < name.length(); i++) {
        wchar_t ch = name[i];
        bool valid = (ch >= L'a' && ch <= L'z') ||
                    (ch >= L'A' && ch <= L'Z') ||
                    (ch >= L'0' && ch <= L'9') ||
                    (ch >= 0x4E00 && ch <= 0x9FFF) ||  // 中文字符
                    ch == L'_';
        
        if (!valid) return false;
    }
    
    return true;
}

bool SyntaxChecker::IsKeyword(const std::wstring& word) const {
    if (!keywordManager_) return false;
    return keywordManager_->GetKeyword(word) != nullptr;
}

bool SyntaxChecker::IsDataType(const std::wstring& word) const {
    if (!libraryParser_) return false;
    
    auto dataTypes = libraryParser_->GetAllDataTypeNames();
    return std::find(dataTypes.begin(), dataTypes.end(), word) != dataTypes.end();
}

bool SyntaxChecker::IsLibraryCommand(const std::wstring& word) const {
    if (!libraryParser_) return false;
    return libraryParser_->FindCommand(word) != nullptr;
}
