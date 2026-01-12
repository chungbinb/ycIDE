#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <map>
#include <vector>
#include "PropertyGrid.h"

using namespace Gdiplus;

// 前向声明
struct ControlInfo;
class FneParser;

// 控件属性定义
struct ControlPropertyInfo {
    std::wstring name;           // 属性名称（如"左边"）
    std::wstring englishName;    // 英文名称（如"left"）
    std::wstring dataType;       // 数据类型（如"整数型"）
    PropertyEditorType editorType;  // 编辑器类型
    std::vector<std::wstring> enumValues;  // 枚举值列表
    std::wstring defaultValue;   // 默认值
    
    ControlPropertyInfo() : editorType(PropertyEditorType::Text) {}
};

// 控件元数据（从krnln.fne加载）
struct ControlMetadata {
    std::wstring type;              // 控件类型（中文名，如"按钮"）
    std::wstring englishName;       // 英文名称（如"Button"）
    std::wstring displayName;       // 显示名称
    std::wstring category;          // 分类（如"基本组件"）
    std::wstring description;       // 说明
    bool isContainer;               // 是否为容器组件
    bool canAcceptFocus;            // 是否可接受输入焦点
    int defaultWidth;               // 默认宽度
    int defaultHeight;              // 默认高度
    std::vector<ControlPropertyInfo> properties;  // 属性列表
    
    ControlMetadata() : defaultWidth(80), defaultHeight(24), isContainer(false), canAcceptFocus(true) {}
};

class ControlRenderer
{
public:
    ControlRenderer();
    virtual ~ControlRenderer();
    
    // === 初始化 ===
    
    bool LoadControlsFromFne(const std::wstring& fnePath);
    bool LoadControlsFromKrnln();  // 加载krnln.fne中的窗口组件
    
    // === 渲染 ===
    
    void RenderControl(Graphics& g, const ControlInfo& control, bool selected = false);
    void RenderControlPreview(Graphics& g, const std::wstring& type, const Rect& bounds);
    
    // === 元数据访问 ===
    
    ControlMetadata GetMetadata(const std::wstring& type) const;
    std::vector<std::wstring> GetControlTypes() const;
    std::vector<std::wstring> GetControlCategories() const;
    std::vector<std::wstring> GetControlsInCategory(const std::wstring& category) const;
    bool HasControl(const std::wstring& type) const;
    
    // === 属性定义 ===
    
    std::vector<PropertyDef> GetPropertyDefs(const std::wstring& type) const;
    
private:
    std::map<std::wstring, ControlMetadata> m_controlMetadata;
    FneParser* m_fneParser;
    
    // === 从帮助文档解析控件信息 ===
    
    bool ParseControlsFromHelpFile(const std::wstring& helpFilePath);
    
    // === 渲染方法（通用） ===
    
    void RenderButton(Graphics& g, const ControlInfo& control);
    void RenderLabel(Graphics& g, const ControlInfo& control);
    void RenderEditBox(Graphics& g, const ControlInfo& control);
    void RenderCheckBox(Graphics& g, const ControlInfo& control);
    void RenderRadioButton(Graphics& g, const ControlInfo& control);
    void RenderComboBox(Graphics& g, const ControlInfo& control);
    void RenderListBox(Graphics& g, const ControlInfo& control);
    void RenderGroupBox(Graphics& g, const ControlInfo& control);
    void RenderPictureBox(Graphics& g, const ControlInfo& control);
    void RenderProgressBar(Graphics& g, const ControlInfo& control);
    void RenderSlider(Graphics& g, const ControlInfo& control);
    void RenderTabControl(Graphics& g, const ControlInfo& control);
    
    // 默认渲染（未知控件类型）
    void RenderDefault(Graphics& g, const ControlInfo& control);
    
    // 辅助方法
    std::wstring GetPropertyValue(const ControlInfo& control, const std::wstring& propName, const std::wstring& defaultValue = L"") const;
    Color ParseColor(const std::wstring& colorStr) const;
    Font* CreateFontFromString(const std::wstring& fontStr) const;
    
    // 注册内置控件（备用方案）
    void RegisterBuiltinControls();
    void RegisterControl(const ControlMetadata& metadata);
};

// 前向声明的结构体定义
#include "VisualDesigner.h"
