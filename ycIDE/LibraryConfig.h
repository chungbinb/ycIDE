#pragma once
#include <string>
#include <vector>
#include <map>

// 支持库配置项
struct LibraryConfigItem {
    std::wstring name;          // 支持库名称
    std::wstring filePath;      // 支持库文件路径
    bool loaded;                // 是否已加载
};

// 支持库配置管理器
class LibraryConfigManager {
public:
    static LibraryConfigManager& GetInstance();
    
    // 扫描支持库文件夹
    void ScanLibraryFolders();
    
    // 获取所有支持库配置
    const std::vector<LibraryConfigItem>& GetLibraries() const { return libraries; }
    
    // 设置支持库加载状态
    void SetLibraryLoaded(size_t index, bool loaded);
    
    // 加载选中的支持库
    int LoadSelectedLibraries();
    
    // 保存配置到文件
    bool SaveConfig();
    
    // 从文件加载配置
    bool LoadConfig();
    
    // 获取配置文件路径
    std::wstring GetConfigFilePath();
    
private:
    LibraryConfigManager() = default;
    
    std::vector<LibraryConfigItem> libraries;
    std::wstring configFilePath;
};
