#include "PinyinHelper.h"
#include "PinyinData.h"    // 自动生成的完整汉字拼音数据表（覆盖 CJK 基本区全部 20992 个汉字）
#include <algorithm>

std::map<wchar_t, std::vector<std::wstring>> PinyinHelper::multiPinyinMap;
bool PinyinHelper::initialized = false;

void PinyinHelper::Initialize() {
    if (initialized) return;
    
    // 多音字映射（数据表只存了一个读音，这里补充多音字的其他读音）
    multiPinyinMap[L'调'] = {L"diao", L"tiao"};
    multiPinyinMap[L'重'] = {L"zhong", L"chong"};
    multiPinyinMap[L'长'] = {L"chang", L"zhang"};
    multiPinyinMap[L'行'] = {L"xing", L"hang"};
    multiPinyinMap[L'藏'] = {L"cang", L"zang"};
    multiPinyinMap[L'传'] = {L"chuan", L"zhuan"};
    multiPinyinMap[L'弹'] = {L"dan", L"tan"};
    
    initialized = true;
}

// 从数据表查询汉字拼音
static std::wstring LookupPinyinFromTable(wchar_t ch) {
    if (ch >= PINYIN_CJK_START && ch <= PINYIN_CJK_END) {
        uint16_t idx = g_CharPinyinIndex[ch - PINYIN_CJK_START];
        if (idx > 0 && idx <= PINYIN_SYLLABLE_COUNT) {
            const char* py = g_PinyinSyllables[idx];
            std::wstring result;
            while (*py) {
                result += (wchar_t)*py;
                py++;
            }
            return result;
        }
    }
    return L"";
}

std::wstring PinyinHelper::GetPinyin(wchar_t ch) {
    Initialize();
    return LookupPinyinFromTable(ch);
}

std::wstring PinyinHelper::GetStringPinyin(const std::wstring& str) {
    Initialize();
    std::wstring result;
    for (wchar_t ch : str) {
        std::wstring py = GetPinyin(ch);
        if (!py.empty()) {
            result += py;
        } else {
            result += ch;
        }
    }
    return result;
}

std::wstring PinyinHelper::GetStringInitials(const std::wstring& str) {
    Initialize();
    std::wstring result;
    for (wchar_t ch : str) {
        std::wstring py = GetPinyin(ch);
        if (!py.empty()) {
            result += py[0];
        } else {
            result += ch;
        }
    }
    return result;
}

std::vector<std::wstring> PinyinHelper::GetStringPinyins(const std::wstring& str) {
    Initialize();
    std::vector<std::wstring> results;
    results.push_back(L"");
    
    for (wchar_t ch : str) {
        std::vector<std::wstring> currentPinyins;
        
        // 检查多音字
        auto itMulti = multiPinyinMap.find(ch);
        if (itMulti != multiPinyinMap.end()) {
            currentPinyins = itMulti->second;
        } else {
            // 从数据表查询
            std::wstring py = LookupPinyinFromTable(ch);
            if (!py.empty()) {
                currentPinyins.push_back(py);
            } else {
                // 非汉字，原样保留
                currentPinyins.push_back(std::wstring(1, ch));
            }
        }
        
        std::vector<std::wstring> newResults;
        for (const auto& prefix : results) {
            for (const auto& py : currentPinyins) {
                newResults.push_back(prefix + py);
            }
        }
        results = newResults;
    }
    return results;
}

std::vector<std::wstring> PinyinHelper::GetStringInitialsList(const std::wstring& str) {
    Initialize();
    std::vector<std::wstring> results;
    results.push_back(L"");
    
    for (wchar_t ch : str) {
        std::vector<std::wstring> currentInitials;
        
        // 检查多音字
        auto itMulti = multiPinyinMap.find(ch);
        if (itMulti != multiPinyinMap.end()) {
            for (const auto& py : itMulti->second) {
                if (!py.empty()) {
                    std::wstring initial = py.substr(0, 1);
                    bool exists = false;
                    for (const auto& existing : currentInitials) {
                        if (existing == initial) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        currentInitials.push_back(initial);
                    }
                }
            }
        } else {
            // 从数据表查询
            std::wstring py = LookupPinyinFromTable(ch);
            if (!py.empty()) {
                currentInitials.push_back(py.substr(0, 1));
            } else {
                // 非汉字，原样保留
                currentInitials.push_back(std::wstring(1, ch));
            }
        }
        
        std::vector<std::wstring> newResults;
        for (const auto& prefix : results) {
            for (const auto& ini : currentInitials) {
                newResults.push_back(prefix + ini);
            }
        }
        results = newResults;
    }
    return results;
}
