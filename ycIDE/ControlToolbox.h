#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <map>

using namespace Gdiplus;

class ControlRenderer;

// 工具箱项
struct ToolboxItem {
    std::wstring type;          // 控件类型
    std::wstring displayName;   // 显示名称
    std::wstring category;      // 分类
    Image* icon;                // 图标（可选）
    
    ToolboxItem() : icon(nullptr) {}
    ~ToolboxItem() {
        if (icon) delete icon;
    }
};

// 工具箱分类
struct ToolboxCategory {
    std::wstring name;
    bool expanded;
    std::vector<ToolboxItem*> items;
    
    ToolboxCategory() : expanded(true) {}
};

class ControlToolbox
{
public:
    ControlToolbox(HWND hWnd);
    virtual ~ControlToolbox();
    
    // === 窗口消息处理 ===
    
    LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void OnPaint(HDC hdc);
    void OnSize(int width, int height);
    void OnLButtonDown(int x, int y);
    void OnLButtonDblClick(int x, int y);
    void OnMouseMove(int x, int y);
    void OnVScroll(WPARAM wParam);
    
    // === 初始化 ===
    
    void LoadFromRenderer(ControlRenderer* renderer);
    
    // === 选择 ===
    
    std::wstring GetSelectedControlType() const;
    void ClearSelection();
    
    // === 搜索 ===
    
    void SetSearchText(const std::wstring& text);
    std::wstring GetSearchText() const { return m_searchText; }
    
    // === 分类管理 ===
    
    void ExpandCategory(const std::wstring& category, bool expand);
    void ExpandAll();
    void CollapseAll();
    
private:
    HWND m_hWnd;
    
    // 工具箱数据
    std::vector<ToolboxCategory> m_categories;
    std::map<std::wstring, ToolboxItem*> m_itemMap;  // 快速查找
    
    // 选择状态
    std::wstring m_selectedType;
    int m_hoveredIndex;
    
    // 搜索
    std::wstring m_searchText;
    
    // 滚动
    int m_scrollPos;
    
    // GDI+
    ULONG_PTR m_gdiplusToken;
    
    // 布局常量
    static const int ITEM_HEIGHT = 32;
    static const int CATEGORY_HEIGHT = 28;
    static const int ICON_SIZE = 24;
    static const int MARGIN = 8;
    
    // === 内部方法 ===
    
    // 渲染
    void DrawToolbox(Graphics& g);
    void DrawCategory(Graphics& g, const ToolboxCategory& category, int& y);
    void DrawItem(Graphics& g, const ToolboxItem& item, int y, bool selected, bool hovered);
    
    // 命中测试
    int HitTestItem(int y);
    bool HitTestCategoryExpander(int x, int y);
    ToolboxItem* GetItemAt(int index);
    
    // 过滤
    std::vector<ToolboxItem*> GetFilteredItems();
    bool MatchesSearch(const ToolboxItem& item) const;
    
    // 布局
    int GetTotalHeight();
    void UpdateScrollBar();
};
