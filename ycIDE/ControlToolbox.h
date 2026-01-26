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
    void OnLButtonUp(int x, int y);
    void OnLButtonDblClick(int x, int y);
    void OnMouseMove(int x, int y);
    void OnVScroll(WPARAM wParam);
    void OnMouseWheel(WPARAM wParam);
    
    // === 停靠/浮动状态 ===
    bool IsDocked() const { return m_isDocked; }
    void SetDocked(bool docked) { m_isDocked = docked; }
    
    // 显示模式
    enum DisplayMode { MODE_LIST = 0, MODE_ICON };
    DisplayMode GetDisplayMode() const { return m_displayMode; }
    void SetDisplayMode(DisplayMode mode) { m_displayMode = mode; InvalidateRect(m_hWnd, NULL, FALSE); }
    void ToggleDisplayMode() { m_displayMode = (m_displayMode == MODE_LIST) ? MODE_ICON : MODE_LIST; InvalidateRect(m_hWnd, NULL, FALSE); }
    bool IsIconMode() const { return m_displayMode == MODE_ICON; }
    
    // 标题栏按钮命中测试
    enum TitlebarButton { BTN_NONE = 0, BTN_VIEW_MODE, BTN_DOCK, BTN_CLOSE };
    TitlebarButton HitTestTitlebarButton(int x, int y);
    
    // 标题栏按钮通知消息
    static const UINT WM_TOOLBOX_DOCK_TOGGLE = 0x2001;  // 停靠/浮动切换
    static const UINT WM_TOOLBOX_CLOSE = 0x2002;        // 关闭
    static const UINT WM_TOOLBOX_VIEW_MODE = 0x2003;    // 显示模式切换
    
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
    
    // === 通知消息 ===
    // 当选择控件类型时发送WM_COMMAND消息到父窗口
    // WPARAM: MAKEWPARAM(0, WM_TOOLBOX_SELECTION_CHANGED)
    // LPARAM: 指向控件类型字符串的指针
    static const UINT WM_TOOLBOX_SELECTION_CHANGED = 0x2000;
    
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
    
    // 停靠/浮动状态
    bool m_isDocked;
    
    // 显示模式
    DisplayMode m_displayMode;
    
    // 标题栏按钮悬停状态
    TitlebarButton m_hoveredButton;
    bool m_buttonPressed;
    
    // GDI+
    ULONG_PTR m_gdiplusToken;
    
    // 布局常量
    static const int TITLEBAR_HEIGHT = 28;  // 自定义标题栏高度
    static const int TITLEBAR_BUTTON_SIZE = 20;  // 标题栏按钮大小
    static const int ITEM_HEIGHT = 32;
    static const int CATEGORY_HEIGHT = 28;
    static const int ICON_SIZE = 24;
    static const int MARGIN = 8;
    
    // === 内部方法 ===
    
    // 初始化
    void AddDefaultControls();
    void LoadControlsFromLibrary();  // 从支持库加载控件
    
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
    
    // 辅助
    ToolboxCategory* FindOrCreateCategory(const std::wstring& categoryName);
};
