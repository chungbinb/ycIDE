#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "FneParser.h"  // 需要 PropertyType 和 FnePropertyInfo

using namespace Gdiplus;

// 属性编辑器类型
enum class PropertyEditorType {
    Text,           // 文本框
    Number,         // 数字输入
    Boolean,        // 复选框
    Color,          // 颜色选择器
    Font,           // 字体选择器
    Enum,           // 下拉列表
    File,           // 文件选择器
    Image,          // 图片选择器
    Cursor,         // 鼠标指针选择器
    ReadOnly        // 只读（不可编辑）
};

// 属性定义
struct PropertyDef {
    std::wstring name;              // 属性名
    std::wstring displayName;       // 显示名称
    std::wstring category;          // 分类
    PropertyEditorType editorType;  // 编辑器类型
    std::wstring value;             // 当前值
    std::wstring defaultValue;      // 默认值
    std::vector<std::wstring> enumValues;  // 枚举值列表（用于Enum类型）
    std::wstring description;       // 属性描述
    bool readOnly;                  // 是否只读
    PropertyType fneType;           // FNE 属性类型（原始类型）
    
    PropertyDef() : editorType(PropertyEditorType::Text), readOnly(false), fneType(PropertyType::Text) {}
};

// 属性变更回调
using PropertyChangedCallback = std::function<void(const std::wstring& propName, const std::wstring& newValue)>;

class PropertyGrid
{
public:
    PropertyGrid(HWND hWnd);
    virtual ~PropertyGrid();
    
    // === 窗口消息处理 ===
    
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void OnPaint(HDC hdc);
    void OnSize(int width, int height);
    void OnLButtonDown(int x, int y);
    void OnLButtonUp(int x, int y);
    void OnMouseMove(int x, int y);
    void OnLButtonDblClick(int x, int y);
    void OnVScroll(WPARAM wParam);
    
    // === 属性管理 ===
    
    void SetProperties(const std::vector<PropertyDef>& properties);
    void UpdateProperty(const std::wstring& name, const std::wstring& value);
    std::wstring GetPropertyValue(const std::wstring& name) const;
    void Clear();
    
    // === 从支持库加载属性 ===
    
    void LoadPropertiesFromLibrary(const std::wstring& controlType, 
                                   const std::map<std::wstring, std::wstring>& currentValues);
    
    // === 辅助函数：将 FNE 属性类型转换为编辑器类型 ===
    
    static PropertyEditorType ConvertPropertyType(PropertyType fneType);
    static PropertyDef ConvertFneProperty(const FnePropertyInfo& fneProp, 
                                          const std::wstring& currentValue = L"");
    
    // === 对象信息 ===
    
    void SetObjectInfo(const std::wstring& objectType, const std::wstring& objectName);
    
    // === 回调 ===
    
    void SetPropertyChangedCallback(PropertyChangedCallback callback);
    
    // === 分类和排序 ===
    
    void SetCategorized(bool categorized);
    bool IsCategorized() const { return m_categorized; }
    void SetAlphabetic(bool alphabetic);
    bool IsAlphabetic() const { return m_alphabetic; }
    
    // === 搜索 ===
    
    void SetSearchText(const std::wstring& text);
    std::wstring GetSearchText() const { return m_searchText; }
    
    // === 编辑控制 ===
    
    void FinishEdit(bool apply);  // 完成编辑
    void ApplyEditWithoutFinish();  // 实时应用编辑但不结束编辑
    
private:
    HWND m_hWnd;
    
    // 属性数据
    std::vector<PropertyDef> m_properties;
    std::map<std::wstring, PropertyDef*> m_propertyMap;  // 快速查找
    
    // 对象信息
    std::wstring m_objectType;
    std::wstring m_objectName;
    
    // 回调
    PropertyChangedCallback m_propertyChangedCallback;
    
    // 显示模式
    bool m_categorized;   // 分类显示
    bool m_alphabetic;    // 按字母排序
    std::wstring m_searchText;  // 搜索文本
    
    // 编辑状态
    int m_selectedRow;    // 当前选中行
    int m_editingRow;     // 正在编辑的行
    HWND m_hEditControl;  // 编辑控件句柄
    HWND m_hComboBox;     // 下拉框句柄
    
    // 自绘下拉列表
    HWND m_hDropdownPopup;           // 自绘下拉列表窗口
    std::vector<std::wstring> m_dropdownItems;  // 下拉列表项
    int m_dropdownSelectedIndex;     // 当前选中项索引
    int m_dropdownHoverIndex;        // 鼠标悬停项索引
    int m_dropdownItemHeight;        // 列表项高度
    DWORD m_dropdownCloseTime;       // 下拉列表关闭时间
    int m_dropdownCloseRow;          // 下拉列表关闭时的行号
    
    // 滚动
    int m_scrollPos;
    int m_rowHeight;
    
    // 展开/折叠状态
    std::map<std::wstring, bool> m_expandedCategories;  // 分类的展开状态
    
    // GDI+
    ULONG_PTR m_gdiplusToken;
    
    // 布局常量
    static const int NAME_COLUMN_WIDTH = 90;
    static const int SPLITTER_WIDTH = 4;
    static const int ROW_HEIGHT = 24;
    static const int CATEGORY_ROW_HEIGHT = 28;
    static const int MARGIN = 4;
    
    // === 内部方法 ===
    
    // 渲染
    void DrawHeader(Graphics& g, const Rect& rect);
    void DrawProperties(Graphics& g);
    void DrawProperty(Graphics& g, const PropertyDef& prop, int y, bool isCategory = false);
    void DrawCategoryRow(Graphics& g, const std::wstring& category, int y, bool expanded);
    void DrawPropertyRow(Graphics& g, const PropertyDef& prop, int y, bool selected);
    void DrawValue(Graphics& g, const PropertyDef& prop, const Rect& valueRect);
    
    // 命中测试
    int HitTestRow(int y);
    bool HitTestCategoryExpander(int x, int y, int row);
    bool HitTestValueColumn(int x);
    
    // 编辑器
    void BeginEdit(int row);
    void CreateTextEditor(const PropertyDef& prop, const Rect& rect);
    void CreateComboEditor(const PropertyDef& prop, const Rect& rect);
    void CreateBooleanComboEditor(const PropertyDef& prop, const Rect& rect);
    void ToggleBooleanValue(PropertyDef& prop);
    void ShowColorPicker(PropertyDef& prop);
    void ShowFontPicker(PropertyDef& prop);
    void ShowFilePicker(PropertyDef& prop);
    void ShowResourcePicker(PropertyDef& prop);
    void ShowCustomDropdown(const PropertyDef& prop, const Rect& rect);
    void CloseCustomDropdown(bool apply);
    static LRESULT CALLBACK DropdownWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
    // 分类和过滤
    std::vector<std::wstring> GetCategories() const;
    std::vector<PropertyDef*> GetPropertiesInCategory(const std::wstring& category);
    std::vector<PropertyDef*> GetFilteredProperties();
    bool MatchesSearch(const PropertyDef& prop) const;
    
    // 布局计算
    int GetVisibleRowCount();
    int GetTotalRowHeight();
    Rect GetPropertyBounds(int row);
    
    // 滚动
    void UpdateScrollBar();
    void ScrollToRow(int row);
};
