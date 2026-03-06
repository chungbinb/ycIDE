#pragma once
#include <string>
#include <vector>
#include <map>

class PinyinHelper {
public:
    // 获取单个汉字的拼音
    static std::wstring GetPinyin(wchar_t ch);
    
    // 获取字符串的完整拼音
    static std::wstring GetStringPinyin(const std::wstring& str);
    
    // 获取字符串的拼音首字母
    static std::wstring GetStringInitials(const std::wstring& str);

    // 获取字符串的所有可能拼音组合（处理多音字）
    static std::vector<std::wstring> GetStringPinyins(const std::wstring& str);
    
    // 获取字符串的所有可能首字母组合
    static std::vector<std::wstring> GetStringInitialsList(const std::wstring& str);

private:
    static void Initialize();
    static std::map<wchar_t, std::vector<std::wstring>> multiPinyinMap;
    static bool initialized;
};
