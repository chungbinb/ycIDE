#include "EditorContext.h"
#include "ProjectManager.h"
#include "LibraryParser.h"
#include "Keyword.h"
#include "NameValidator.h"
#include "Theme.h"
#include <algorithm>

// 静态成员初始化
EditorContext* EditorContext::s_instance = nullptr;

// 单例访问
EditorContext* EditorContext::GetInstance() {
    if (!s_instance) {
        s_instance = new EditorContext();
    }
    return s_instance;
}

void EditorContext::Initialize() {
    if (!s_instance) {
        s_instance = new EditorContext();
    }
}

void EditorContext::Cleanup() {
    if (s_instance) {
        delete s_instance;
        s_instance = nullptr;
    }
}

// 构造/析构
EditorContext::EditorContext()
    : m_pProjectManager(nullptr)
    , m_pLibraryParser(nullptr)
    , m_pKeywordManager(nullptr)
    , m_pNameValidator(nullptr)
    , m_pTheme(nullptr)
    , m_globalFontSize(14)
    , m_enableAutoComplete(true)
    , m_enableRealTimeValidation(true) {
}

EditorContext::~EditorContext() {
    // 注意：管理器对象不由 EditorContext 拥有，不在这里删除
}

// === 事件总线 ===

void EditorContext::RegisterDataChangeCallback(DataChangeCallback callback) {
    m_dataChangeCallbacks.push_back(callback);
}

void EditorContext::NotifyDataChanged(EditorFileType type, const std::wstring& name) {
    // 通知所有注册的监听器
    for (const auto& callback : m_dataChangeCallbacks) {
        callback(type, name);
    }
}

// === 工具方法 ===

EditorFileType EditorContext::GetFileTypeFromExtension(const std::wstring& filePath) {
    // 获取文件扩展名
    size_t dotPos = filePath.find_last_of(L'.');
    if (dotPos == std::wstring::npos) {
        return EditorFileType::Unknown;
    }
    
    std::wstring ext = filePath.substr(dotPos);
    
    // 转换为小写
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == L".eyc") {
        return EditorFileType::EycCode;
    } else if (ext == L".ell") {
        return EditorFileType::EllDll;
    } else if (ext == L".ecs") {
        return EditorFileType::EcsConstant;
    } else if (ext == L".egv") {
        return EditorFileType::EgvGlobalVar;
    } else if (ext == L".edt") {
        return EditorFileType::EdtDataType;
    } else if (ext == L".efw") {
        return EditorFileType::EfwDesigner;
    }
    
    return EditorFileType::Unknown;
}

std::wstring EditorContext::GetDefaultExtension(EditorFileType type) {
    switch (type) {
        case EditorFileType::EycCode:       return L".eyc";
        case EditorFileType::EllDll:        return L".ell";
        case EditorFileType::EcsConstant:   return L".ecs";
        case EditorFileType::EgvGlobalVar:  return L".egv";
        case EditorFileType::EdtDataType:   return L".edt";
        case EditorFileType::EfwDesigner: return L".efw";
        default:                            return L"";
    }
}

std::wstring EditorContext::GetFileTypeName(EditorFileType type) {
    switch (type) {
        case EditorFileType::EycCode:       return L"代码文件";
        case EditorFileType::EllDll:        return L"DLL声明文件";
        case EditorFileType::EcsConstant:   return L"常量文件";
        case EditorFileType::EgvGlobalVar:  return L"全局变量文件";
        case EditorFileType::EdtDataType:   return L"数据类型文件";
        case EditorFileType::EfwDesigner: return L"窗体设计文件";
        default:                            return L"未知文件";
    }
}
