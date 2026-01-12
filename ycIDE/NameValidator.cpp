#include "NameValidator.h"
#include "Keyword.h"
#include "ProjectManager.h"
#include <algorithm>
#include <fstream>
#include <sstream>

// 构造/析构
NameValidator::NameValidator()
    : m_pKeywordManager(nullptr)
    , m_pProjectManager(nullptr)
    , m_symbolsCached(false) {
}

NameValidator::~NameValidator() {
}

// === 符号表管理 ===

void NameValidator::RefreshSymbolTable() {
    m_globalSymbols.clear();
    m_symbolsCached = false;
    
    if (!m_pProjectManager) {
        return;
    }
    
    // TODO: 从项目管理器获取所有文件并解析
    // 目前简化实现，后续完善
    
    m_symbolsCached = true;
}

void NameValidator::AddSymbol(const SymbolInfo& symbol) {
    m_globalSymbols[symbol.name].push_back(symbol);
}

void NameValidator::RemoveSymbol(const std::wstring& name, SymbolType type, const std::wstring& filePath) {
    auto it = m_globalSymbols.find(name);
    if (it == m_globalSymbols.end()) {
        return;
    }
    
    // 移除匹配的符号
    auto& symbols = it->second;
    symbols.erase(
        std::remove_if(symbols.begin(), symbols.end(),
            [&](const SymbolInfo& s) {
                return s.type == type && s.filePath == filePath;
            }),
        symbols.end()
    );
    
    // 如果列表为空，删除整个条目
    if (symbols.empty()) {
        m_globalSymbols.erase(it);
    }
}

void NameValidator::RemoveFileSymbols(const std::wstring& filePath) {
    // 遍历所有符号，移除匹配文件的符号
    for (auto it = m_globalSymbols.begin(); it != m_globalSymbols.end(); ) {
        auto& symbols = it->second;
        symbols.erase(
            std::remove_if(symbols.begin(), symbols.end(),
                [&](const SymbolInfo& s) {
                    return s.filePath == filePath;
                }),
            symbols.end()
        );
        
        if (symbols.empty()) {
            it = m_globalSymbols.erase(it);
        } else {
            ++it;
        }
    }
}

// === 验证方法 ===

std::wstring NameValidator::ValidateName(const std::wstring& name, SymbolType type, const std::wstring& currentFile) {
    // 检查空名称
    if (name.empty()) {
        return L"名称不能为空";
    }
    
    // 检查长度
    if (!IsValidLength(name)) {
        return L"名称长度超出限制（最大255字符）";
    }
    
    // 检查是否为标识符
    if (!IsValidIdentifier(name)) {
        return L"名称包含非法字符";
    }
    
    // 检查是否为关键词
    if (IsKeyword(name)) {
        return L"不能使用关键词作为名称";
    }
    
    // 检查名称冲突
    if (IsNameExists(name, type, currentFile)) {
        std::vector<SymbolInfo> conflicts = GetConflictingSymbols(name, currentFile);
        if (!conflicts.empty()) {
            std::wstring msg = L"名称已存在于: ";
            for (size_t i = 0; i < conflicts.size() && i < 3; i++) {
                if (i > 0) msg += L", ";
                msg += conflicts[i].filePath;
            }
            return msg;
        }
    }
    
    return L""; // 验证通过
}

bool NameValidator::IsKeyword(const std::wstring& name) {
    if (!m_pKeywordManager) {
        return false;
    }
    
    // 检查是否为关键词或别名
    return m_pKeywordManager->IsAlias(name);
}

bool NameValidator::IsNameExists(const std::wstring& name, SymbolType type, const std::wstring& currentFile) {
    auto it = m_globalSymbols.find(name);
    if (it == m_globalSymbols.end()) {
        return false;
    }
    
    // 检查是否有来自其他文件的同名符号
    for (const auto& symbol : it->second) {
        if (symbol.type == type && symbol.filePath != currentFile) {
            return true;
        }
    }
    
    return false;
}

std::vector<SymbolInfo> NameValidator::GetConflictingSymbols(const std::wstring& name, const std::wstring& currentFile) {
    std::vector<SymbolInfo> conflicts;
    
    auto it = m_globalSymbols.find(name);
    if (it == m_globalSymbols.end()) {
        return conflicts;
    }
    
    // 返回来自其他文件的符号
    for (const auto& symbol : it->second) {
        if (symbol.filePath != currentFile) {
            conflicts.push_back(symbol);
        }
    }
    
    return conflicts;
}

std::wstring NameValidator::GenerateUniqueName(const std::wstring& baseName, SymbolType type, const std::wstring& currentFile) {
    // 如果基础名称不冲突，直接返回
    if (!IsNameExists(baseName, type, currentFile)) {
        return baseName;
    }
    
    // 提取基础名称（去除数字后缀）
    std::wstring cleanBaseName = baseName;
    size_t lastNonDigit = cleanBaseName.find_last_not_of(L"0123456789");
    if (lastNonDigit != std::wstring::npos && lastNonDigit < cleanBaseName.length() - 1) {
        cleanBaseName = cleanBaseName.substr(0, lastNonDigit + 1);
    }
    
    // 尝试添加数字后缀
    for (int index = 1; index < 10000; index++) {
        std::wstring newName = cleanBaseName + std::to_wstring(index);
        if (!IsNameExists(newName, type, currentFile)) {
            return newName;
        }
    }
    
    // 理论上不应该到达这里
    return baseName + L"_new";
}

// === 命名规范检查 ===

bool NameValidator::IsValidIdentifier(const std::wstring& name) {
    if (name.empty()) {
        return false;
    }
    
    // 易语言标识符规则：
    // 1. 可以包含中文、字母、数字、下划线
    // 2. 不能以数字开头
    // 3. 不能包含特殊符号和空格
    
    // 检查第一个字符不能是数字
    wchar_t first = name[0];
    if (first >= L'0' && first <= L'9') {
        return false;
    }
    
    // 检查所有字符
    for (wchar_t ch : name) {
        bool isLetter = (ch >= L'a' && ch <= L'z') || (ch >= L'A' && ch <= L'Z');
        bool isDigit = (ch >= L'0' && ch <= L'9');
        bool isUnderscore = (ch == L'_');
        bool isChinese = (ch >= 0x4E00 && ch <= 0x9FA5); // 基本汉字范围
        
        if (!isLetter && !isDigit && !isUnderscore && !isChinese) {
            // 检查是否为其他有效Unicode字符（扩展汉字等）
            if (ch < 128) {
                // ASCII范围内的非法字符
                return false;
            }
        }
    }
    
    return true;
}

bool NameValidator::IsValidLength(const std::wstring& name) {
    // 易语言标识符最大长度为255
    return name.length() > 0 && name.length() <= 255;
}

// === 工具方法 ===

std::wstring NameValidator::GetSymbolTypeName(SymbolType type) {
    switch (type) {
        case SymbolType::Constant:       return L"常量";
        case SymbolType::GlobalVariable: return L"全局变量";
        case SymbolType::DataType:       return L"数据类型";
        case SymbolType::DllCommand:     return L"DLL命令";
        case SymbolType::Subroutine:     return L"子程序";
        default:                         return L"未知";
    }
}

// === 私有方法 ===

void NameValidator::ExtractSymbolsFromFile(const std::wstring& filePath) {
    // 根据文件扩展名调用相应的解析方法
    size_t dotPos = filePath.find_last_of(L'.');
    if (dotPos == std::wstring::npos) {
        return;
    }
    
    std::wstring ext = filePath.substr(dotPos);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".ecl") {
        ParseConstantFile(filePath);
    } else if (ext == L".eal") {
        ParseGlobalVarFile(filePath);
    } else if (ext == L".edl") {
        ParseDataTypeFile(filePath);
    } else if (ext == L".ell") {
        ParseDllFile(filePath);
    }
}

void NameValidator::ParseConstantFile(const std::wstring& filePath) {
    // TODO: 解析常量文件，提取常量名
    // 暂时简化实现
}

void NameValidator::ParseGlobalVarFile(const std::wstring& filePath) {
    // TODO: 解析全局变量文件，提取变量名
    // 暂时简化实现
}

void NameValidator::ParseDataTypeFile(const std::wstring& filePath) {
    // TODO: 解析数据类型文件，提取类型名
    // 暂时简化实现
}

void NameValidator::ParseDllFile(const std::wstring& filePath) {
    // 解析 DLL 文件，提取命令名
    std::wifstream file(filePath);
    if (!file.is_open()) {
        return;
    }
    
    std::wstring line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        
        // 简单解析 .DLL命令 行
        if (line.find(L".DLL命令") == 0) {
            size_t pos = line.find(L' ');
            if (pos != std::wstring::npos) {
                std::wstring params = line.substr(pos + 1);
                size_t commaPos = params.find(L',');
                if (commaPos != std::wstring::npos) {
                    std::wstring commandName = params.substr(0, commaPos);
                    
                    // 去除首尾空格
                    size_t start = commandName.find_first_not_of(L" \t");
                    size_t end = commandName.find_last_not_of(L" \t");
                    if (start != std::wstring::npos) {
                        commandName = commandName.substr(start, end - start + 1);
                        
                        SymbolInfo symbol(commandName, SymbolType::DllCommand, filePath);
                        symbol.lineNumber = lineNumber;
                        AddSymbol(symbol);
                    }
                }
            }
        }
    }
    
    file.close();
}
