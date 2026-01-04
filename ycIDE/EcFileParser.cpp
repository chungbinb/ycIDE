#include "EcFileParser.h"
#include <algorithm>

#undef min
#undef max

bool EcFileParser::ReadFile(const std::wstring& filePath, std::vector<BYTE>& data) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        lastError_ = L"无法打开文件: " + filePath;
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    data.resize(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(data.data()), size)) {
        lastError_ = L"读取文件失败";
        return false;
    }

    return true;
}

bool EcFileParser::CheckHeader(const std::vector<BYTE>& data) {
    if (data.size() < 8) {
        lastError_ = L"文件太小，不是有效的.ec模块文件";
        return false;
    }

    // 检查魔术头: CNWTEPRG
    const char* header = "CNWTEPRG";
    for (int i = 0; i < 8; i++) {
        if (data[i] != (BYTE)header[i]) {
            lastError_ = L"文件格式错误：不是有效的易语言模块文件\n";
            lastError_ += L"期望文件头: CNWTEPRG\n";
            lastError_ += L"实际文件头: ";
            for (int j = 0; j < 8 && j < (int)data.size(); j++) {
                wchar_t ch = (wchar_t)data[j];
                if (ch >= 32 && ch < 127) {
                    lastError_ += ch;
                } else {
                    lastError_ += L".";
                }
            }
            return false;
        }
    }

    return true;
}

std::wstring EcFileParser::ReadGBKString(const BYTE* data, size_t maxLen) {
    // 查找字符串结束符
    size_t len = 0;
    while (len < maxLen && data[len] != 0) {
        len++;
    }

    if (len == 0) {
        return L"";
    }

    // GBK转UTF-16
    int wlen = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)data, (int)len, NULL, 0);
    if (wlen == 0) {
        return L"";
    }

    std::wstring result(wlen, 0);
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)data, (int)len, &result[0], wlen);
    return result;
}

void EcFileParser::ParseModuleInfo(const std::vector<BYTE>& data) {
    // 改进的解析：搜索整个文件查找GBK编码的字符串
    // .ec文件中的命令名通常是连续的GBK字符串
    
    std::vector<std::wstring> foundStrings;
    
    // 遍历整个文件查找字符串
    for (size_t offset = 0; offset < data.size() - 3; offset++) {
        // 检测可能的GBK字符串起始
        // GBK汉字：0x81-0xFE + 0x40-0xFE
        // ASCII：0x20-0x7E
        if ((data[offset] >= 0x81 && data[offset] <= 0xFE) || 
            (data[offset] >= 0x41 && data[offset] <= 0x7A)) {  // A-Z a-z
            
            // 读取字符串
            size_t maxLen = data.size() - offset;
            if (maxLen > 200) maxLen = 200;  // 限制最大长度
            
            std::wstring str = ReadGBKString(&data[offset], maxLen);
            
            // 过滤有效的命令名：
            // 1. 长度在2-30之间（命令名通常不会太长）
            // 2. 不包含特殊控制字符和常见的非命令字符
            // 3. 主要由汉字、字母、数字组成
            // 4. 不包含句号、逗号、空格等（这些通常出现在描述中）
            if (str.length() >= 2 && str.length() <= 30) {
                bool valid = true;
                int chineseCount = 0;
                int alphaCount = 0;
                int digitCount = 0;
                
                for (wchar_t ch : str) {
                    // 排除控制字符和常见分隔符
                    if (ch < 0x20 || ch == 0x7F || ch == L' ' || ch == L',' || 
                        ch == L'.' || ch == L'、' || ch == L'，' || ch == L'。' ||
                        ch == L':' || ch == L'：' || ch == L';' || ch == L'；' ||
                        ch == L'(' || ch == L')' || ch == L'（' || ch == L'）' ||
                        ch == L'[' || ch == L']' || ch == L'【' || ch == L'】' ||
                        ch == L'<' || ch == L'>' || ch == L'《' || ch == L'》' ||
                        ch == L'/' || ch == L'\\' || ch == L'|' || ch == L'&' ||
                        ch == L'=' || ch == L'+' || ch == L'-' || ch == L'*') {
                        valid = false;
                        break;
                    }
                    if (ch >= 0x4E00 && ch <= 0x9FA5) chineseCount++;  // 汉字
                    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) alphaCount++;
                    if (ch >= '0' && ch <= '9') digitCount++;
                }
                
                // 命令名应该：
                // - 主要由汉字组成（至少2个汉字）或
                // - 主要由字母组成（至少3个字母）或
                // - 汉字+字母的组合
                if (valid) {
                    bool isValidCommand = false;
                    
                    // 纯汉字命令（如"读文件"、"写配置"）
                    if (chineseCount >= 2 && alphaCount == 0) {
                        isValidCommand = true;
                    }
                    // 纯字母命令（如"SaveFile"、"LoadData"）
                    else if (alphaCount >= 3 && chineseCount == 0) {
                        isValidCommand = true;
                    }
                    // 汉字+字母混合（如"取Json值"、"SetJSON"）
                    else if (chineseCount >= 1 && alphaCount >= 1) {
                        isValidCommand = true;
                    }
                    
                    if (isValidCommand) {
                        foundStrings.push_back(str);
                    }
                }
            }
        }
    }
    
    // 去重并整理
    std::vector<std::wstring> uniqueStrings;
    for (const auto& str : foundStrings) {
        if (std::find(uniqueStrings.begin(), uniqueStrings.end(), str) == uniqueStrings.end()) {
            uniqueStrings.push_back(str);
        }
    }
    
    // 第一个通常是模块名
    if (!uniqueStrings.empty()) {
        moduleName_ = uniqueStrings[0];
        
        // 其余的作为命令（跳过重复的模块名）
        for (size_t i = 1; i < uniqueStrings.size(); i++) {
            if (uniqueStrings[i] != moduleName_ && uniqueStrings[i].length() >= 2) {
                commands_.push_back(uniqueStrings[i]);
            }
        }
    }
    
    // 如果没找到模块名，使用默认名
    if (moduleName_.empty()) {
        moduleName_ = L"未知模块";
    }
}

bool EcFileParser::ParseFile(const std::wstring& filePath) {
    // 清空之前的数据
    moduleName_.clear();
    commands_.clear();
    lastError_.clear();

    // 读取文件
    std::vector<BYTE> data;
    if (!ReadFile(filePath, data)) {
        return false;
    }

    // 检查文件头
    if (!CheckHeader(data)) {
        return false;
    }

    // 解析模块信息（简化版）
    ParseModuleInfo(data);

    // 临时方案：由于完整解析.ec格式复杂，先提供基本信息
    if (commands_.empty()) {
        lastError_ = L"警告：无法完整解析模块文件。\n\n";
        lastError_ += L"当前实现是简化版解析器，仅支持基本信息提取。\n";
        lastError_ += L"要获得完整功能，需要：\n";
        lastError_ += L"1. 使用64位版本的ECodeParser.dll\n";
        lastError_ += L"2. 或实现完整的.ec文件格式解析\n\n";
        lastError_ += L"模块名称: " + moduleName_;
        return false;
    }

    return true;
}
