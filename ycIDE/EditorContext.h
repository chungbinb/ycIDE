#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <functional>

// 前向声明
class ProjectManager;
class LibraryParser;
class KeywordManager;
class NameValidator;
struct AppTheme;

// 文件类型枚举
enum class EditorFileType {
    Unknown = 0,
    EycCode = 1,        // .eyc 代码文件
    EllDll = 2,         // .ell DLL声明文件
    EclConstant = 3,    // .ecl 常量文件
    EalGlobalVar = 4,   // .eal 全局变量文件
    EdlDataType = 5,    // .edl 自定义数据类型文件
    EformDesigner = 6   // .eform 窗体设计文件
};

// 数据变更通知回调
using DataChangeCallback = std::function<void(EditorFileType type, const std::wstring& name)>;

// 编辑器共享上下文类
// 管理所有编辑器共享的资源和状态
class EditorContext {
private:
    static EditorContext* s_instance;
    
    // 管理器引用
    ProjectManager* m_pProjectManager;
    LibraryParser* m_pLibraryParser;
    KeywordManager* m_pKeywordManager;
    NameValidator* m_pNameValidator;
    
    // 主题设置
    AppTheme* m_pTheme;
    
    // 事件总线 - 数据变更通知
    std::vector<DataChangeCallback> m_dataChangeCallbacks;
    
    // 全局设置
    std::wstring m_workspaceRoot;
    int m_globalFontSize;
    bool m_enableAutoComplete;
    bool m_enableRealTimeValidation;
    
public:
    // 单例模式
    static EditorContext* GetInstance();
    static void Initialize();
    static void Cleanup();
    
    EditorContext();
    ~EditorContext();
    
    // === 管理器访问 ===
    ProjectManager* GetProjectManager() const { return m_pProjectManager; }
    LibraryParser* GetLibraryParser() const { return m_pLibraryParser; }
    KeywordManager* GetKeywordManager() const { return m_pKeywordManager; }
    NameValidator* GetNameValidator() const { return m_pNameValidator; }
    
    void SetProjectManager(ProjectManager* pm) { m_pProjectManager = pm; }
    void SetLibraryParser(LibraryParser* lp) { m_pLibraryParser = lp; }
    void SetKeywordManager(KeywordManager* km) { m_pKeywordManager = km; }
    void SetNameValidator(NameValidator* nv) { m_pNameValidator = nv; }
    
    // === 主题管理 ===
    AppTheme* GetTheme() const { return m_pTheme; }
    void SetTheme(AppTheme* theme) { m_pTheme = theme; }
    
    // === 事件总线 ===
    void RegisterDataChangeCallback(DataChangeCallback callback);
    void NotifyDataChanged(EditorFileType type, const std::wstring& name);
    
    // === 全局设置 ===
    std::wstring GetWorkspaceRoot() const { return m_workspaceRoot; }
    void SetWorkspaceRoot(const std::wstring& root) { m_workspaceRoot = root; }
    
    int GetGlobalFontSize() const { return m_globalFontSize; }
    void SetGlobalFontSize(int size) { m_globalFontSize = size; }
    
    bool IsAutoCompleteEnabled() const { return m_enableAutoComplete; }
    void SetAutoCompleteEnabled(bool enabled) { m_enableAutoComplete = enabled; }
    
    bool IsRealTimeValidationEnabled() const { return m_enableRealTimeValidation; }
    void SetRealTimeValidationEnabled(bool enabled) { m_enableRealTimeValidation = enabled; }
    
    // === 工具方法 ===
    
    // 根据文件扩展名获取文件类型
    static EditorFileType GetFileTypeFromExtension(const std::wstring& filePath);
    
    // 获取文件类型的默认扩展名
    static std::wstring GetDefaultExtension(EditorFileType type);
    
    // 获取文件类型的显示名称
    static std::wstring GetFileTypeName(EditorFileType type);
};
