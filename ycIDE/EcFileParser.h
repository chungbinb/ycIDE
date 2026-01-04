#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>

// 简化的.ec文件解析器 - 直接解析二进制文件
// 不依赖ECodeParser.dll
class EcFileParser {
public:
    EcFileParser() = default;
    ~EcFileParser() = default;

    // 解析.ec文件，提取命令列表
    bool ParseFile(const std::wstring& filePath);
    
    // 获取模块名称
    std::wstring GetModuleName() const { return moduleName_; }
    
    // 获取命令列表（简化版，只包含命令名称）
    const std::vector<std::wstring>& GetCommands() const { return commands_; }
    
    // 获取最后的错误信息
    std::wstring GetLastError() const { return lastError_; }

private:
    // 读取文件到内存
    bool ReadFile(const std::wstring& filePath, std::vector<BYTE>& data);
    
    // 检查文件头
    bool CheckHeader(const std::vector<BYTE>& data);
    
    // 解析模块信息（简化版）
    void ParseModuleInfo(const std::vector<BYTE>& data);
    
    // 查找字符串（GBK编码转换为宽字符）
    std::wstring ReadGBKString(const BYTE* data, size_t maxLen);
    
    std::wstring moduleName_;
    std::vector<std::wstring> commands_;
    std::wstring lastError_;
};
