#include "Keyword.h"
#include "PinyinHelper.h"
#include <algorithm>

KeywordManager& KeywordManager::GetInstance() {
    static KeywordManager instance;
    return instance;
}

void KeywordManager::Initialize() {
    keywords.clear();
    aliasMap.clear();
    
    // 所有关键词现在都从支持库加载，不再使用内置关键词
}

std::vector<CompletionItem> KeywordManager::GetCompletions(const std::wstring& input) {
    std::vector<CompletionItem> completions;
    
    if (input.empty()) return completions;
    
    std::wstring lowerInput = input;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::towlower);
    
    // 辅助函数：计算匹配分数
    auto CalculateScore = [&](const std::wstring& chinese, const std::vector<std::wstring>& pinyin, const std::vector<std::wstring>& initials, const std::vector<std::wstring>* aliases = nullptr) -> int {
        int score = 0;
        
        // 1. 中文匹配
        size_t pos = chinese.find(input);
        if (pos != std::wstring::npos) {
            if (pos == 0) {
                if (chinese == input) score = 1000; // 完全匹配
                else score = 500; // 前缀匹配
            } else {
                score = 100; // 包含匹配
            }
            // 长度惩罚：越短越好
            score -= (int)(chinese.length() - input.length());
            return score;
        }
        
        // 2. 全拼匹配
        for (const auto& py : pinyin) {
            if (!py.empty()) {
                pos = py.find(lowerInput);
                if (pos != std::wstring::npos) {
                    int currentScore = 0;
                    if (pos == 0) {
                        if (py == lowerInput) currentScore = 800; // 全拼完全匹配
                        else currentScore = 400; // 全拼前缀匹配
                    } else {
                        currentScore = 50; // 全拼包含匹配
                    }
                    // 长度惩罚
                    currentScore -= (int)(py.length() - lowerInput.length());
                    if (currentScore > score) score = currentScore;
                }
            }
        }
        
        // 3. 首字母匹配
        for (const auto& ini : initials) {
            if (!ini.empty()) {
                pos = ini.find(lowerInput);
                if (pos != std::wstring::npos) {
                    int currentScore = 0;
                    if (pos == 0) {
                        if (ini == lowerInput) currentScore = 600; // 首字母完全匹配
                        else currentScore = 300; // 首字母前缀匹配
                    } else {
                        currentScore = 20; // 首字母包含匹配
                    }
                    // 长度惩罚
                    currentScore -= (int)(ini.length() - lowerInput.length());
                    if (currentScore > score) score = currentScore;
                }
            }
        }
        
        // 4. 别名匹配（仅针对关键字）
        if (aliases) {
            for (const auto& alias : *aliases) {
                pos = alias.find(lowerInput);
                if (pos != std::wstring::npos) {
                    int currentScore = 0;
                    if (pos == 0) {
                        if (alias == lowerInput) currentScore = 900; // 别名完全匹配
                        else currentScore = 450; // 别名前缀匹配
                    } else {
                        currentScore = 80; // 别名包含匹配
                    }
                    currentScore -= (int)(alias.length() - lowerInput.length());
                    if (currentScore > score) score = currentScore;
                }
            }
        }
        
        return score;
    };
    
    // 首先搜索关键词
    for (const auto& kw : keywords) {
        int score = CalculateScore(kw.chinese, kw.pinyin, kw.initials, &kw.aliases);
        if (score > 0) {
            // 过滤掉结束命令
            if (kw.chinese == L"如果结束" || kw.chinese == L"如果真结束") {
                continue;
            }
            
            CompletionItem item;
            item.displayText = kw.chinese;
            item.insertText = kw.template_;
            item.description = kw.description;
            item.type = kw.type;
            item.isLibraryCommand = false;
            item.score = score;
            completions.push_back(item);
        }
    }
    
    // 然后搜索支持库命令
    auto& libParser = LibraryParser::GetInstance();
    for (const auto& cmd : libParser.GetCommands()) {
        int score = CalculateScore(cmd.chineseName, cmd.pinyin, cmd.initials);
        
        // 检查英文名
        if (!cmd.englishName.empty()) {
            std::wstring lowerEnglish = cmd.englishName;
            std::transform(lowerEnglish.begin(), lowerEnglish.end(), lowerEnglish.begin(), ::towlower);
            size_t pos = lowerEnglish.find(lowerInput);
            if (pos != std::wstring::npos) {
                int currentScore = 0;
                if (pos == 0) {
                    if (lowerEnglish == lowerInput) currentScore = 700; // 英文完全匹配
                    else currentScore = 350; // 英文前缀匹配
                } else {
                    currentScore = 30; // 英文包含匹配
                }
                currentScore -= (int)(lowerEnglish.length() - lowerInput.length());
                if (currentScore > score) score = currentScore;
            }
        }
        
        if (score > 0) {
            CompletionItem item;
            item.displayText = cmd.chineseName;
            
            // 如果有英文名，显示英文名
            if (!cmd.englishName.empty()) {
                item.displayText += L" (" + cmd.englishName + L")";
            }
            
            // 构建插入文本（包含参数提示）
            item.insertText = cmd.chineseName;
            if (!cmd.parameters.empty()) {
                item.insertText += L"(";
                for (size_t i = 0; i < cmd.parameters.size(); i++) {
                    if (i > 0) item.insertText += L", ";
                    item.insertText += cmd.parameters[i].name;
                }
                item.insertText += L")";
            }
            else {
                item.insertText += L"()";
            }
            
            // 设置描述
            item.description = cmd.description;
            if (!cmd.returnType.empty() && cmd.returnType != L"无返回值") {
                item.description = L"返回：" + cmd.returnType + L" - " + cmd.description;
            }
            
            item.type = KW_OTHER;
            item.isLibraryCommand = true;
            item.score = score;
            completions.push_back(item);
        }
    }
    
    // 根据分数排序
    std::sort(completions.begin(), completions.end(), [](const CompletionItem& a, const CompletionItem& b) {
        return a.score > b.score;
    });
    
    return completions;
}

bool KeywordManager::IsAlias(const std::wstring& text) {
    std::wstring lowerText = text;
    std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::towlower);
    return aliasMap.find(lowerText) != aliasMap.end();
}

std::wstring KeywordManager::GetChineseKeyword(const std::wstring& alias) {
    std::wstring lowerAlias = alias;
    std::transform(lowerAlias.begin(), lowerAlias.end(), lowerAlias.begin(), ::towlower);
    
    auto it = aliasMap.find(lowerAlias);
    if (it != aliasMap.end()) {
        return it->second;
    }
    return L"";
}

const Keyword* KeywordManager::GetKeyword(const std::wstring& chinese) {
    for (const auto& kw : keywords) {
        if (kw.chinese == chinese) {
            return &kw;
        }
    }
    return nullptr;
}

bool KeywordManager::NeedsFlowLine(const std::wstring& chinese) {
    const Keyword* kw = GetKeyword(chinese);
    return kw && kw->needsFlowLine;
}

bool KeywordManager::LoadLibraryCommands(const std::wstring& libraryFilePath) {
    return LibraryParser::GetInstance().LoadLibraryFile(libraryFilePath);
}

const LibraryCommand* KeywordManager::GetLibraryCommand(const std::wstring& name) {
    return LibraryParser::GetInstance().FindCommand(name);
}
