#include "ControlToolbox.h"
#include "EditorContext.h"
#include "ControlRenderer.h"
#include "LibraryParser.h"
#include "Theme.h"
#include <cstdio>

// 防止 Windows 头文件中的 min/max 宏与 std::min/std::max 冲突
#ifndef NOMINMAX
#define NOMINMAX
#endif
#undef min
#undef max

extern AppTheme g_CurrentTheme;

// 将 Windows COLORREF (BGR) 转换为 GDI+ Color (ARGB)
inline Gdiplus::Color ColorRefToGdiplus(COLORREF cr) {
    return Gdiplus::Color(255, GetRValue(cr), GetGValue(cr), GetBValue(cr));
}

// 调试日志函数
static void ToolboxDebugLog(const std::wstring& msg) {
    static bool firstWrite = true;
    static std::wstring logPath;
    
    if (firstWrite) {
        // 创建 logs 目录
        CreateDirectoryW(L"logs", NULL);
        logPath = L"logs\\toolbox_debug.txt";
    }
    
    FILE* f = nullptr;
    _wfopen_s(&f, logPath.c_str(), firstWrite ? L"w" : L"a");
    if (f) {
        fwprintf(f, L"%s\n", msg.c_str());
        fclose(f);
        firstWrite = false;
    }
    OutputDebugStringW((msg + L"\n").c_str());
}

// ControlToolbox 构造函数
ControlToolbox::ControlToolbox(HWND hWnd)
    : m_hWnd(hWnd)
    , m_hoveredIndex(-1)
    , m_scrollPos(0)
    , m_gdiplusToken(0)
{
    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    
    // 先尝试从支持库加载控件
    LoadControlsFromLibrary();
    
    // 如果支持库没有控件，使用默认控件
    if (m_categories.empty()) {
        AddDefaultControls();
    }
}

ControlToolbox::~ControlToolbox()
{
    // 清理工具箱项
    for (auto& cat : m_categories) {
        for (auto item : cat.items) {
            delete item;
        }
    }
    
    if (m_gdiplusToken) {
        GdiplusShutdown(m_gdiplusToken);
    }
}

void ControlToolbox::AddDefaultControls()
{
    // 添加常用控件分类
    ToolboxCategory commonCat;
    commonCat.name = L"常用控件";
    commonCat.expanded = true;
    
    auto addItem = [&](const wchar_t* type, const wchar_t* name) {
        ToolboxItem* item = new ToolboxItem();
        item->type = type;
        item->displayName = name;
        item->category = commonCat.name;
        commonCat.items.push_back(item);
        m_itemMap[type] = item;
    };
    
    addItem(L"标签", L"标签");
    addItem(L"编辑框", L"编辑框");
    addItem(L"按钮", L"按钮");
    addItem(L"图片框", L"图片框");
    addItem(L"画板", L"画板");
    addItem(L"外形框", L"外形框");
    addItem(L"选择框", L"选择框");
    addItem(L"单选框", L"单选框");
    addItem(L"组合框", L"组合框");
    addItem(L"列表框", L"列表框");
    addItem(L"横向滚动条", L"横向滚动条");
    addItem(L"纵向滚动条", L"纵向滚动条");
    addItem(L"进度条", L"进度条");
    addItem(L"滑块条", L"滑块条");
    addItem(L"选择夹", L"选择夹");
    addItem(L"分组框", L"分组框");
    addItem(L"时钟", L"时钟");
    
    m_categories.push_back(commonCat);
    
    // 添加容器控件分类
    ToolboxCategory containerCat;
    containerCat.name = L"容器控件";
    containerCat.expanded = true;
    
    auto addContainerItem = [&](const wchar_t* type, const wchar_t* name) {
        ToolboxItem* item = new ToolboxItem();
        item->type = type;
        item->displayName = name;
        item->category = containerCat.name;
        containerCat.items.push_back(item);
        m_itemMap[type] = item;
    };
    
    addContainerItem(L"面板", L"面板");
    addContainerItem(L"分隔条", L"分隔条");
    
    m_categories.push_back(containerCat);
}

// 从支持库加载控件
void ControlToolbox::LoadControlsFromLibrary()
{
    const auto& windowUnits = LibraryParser::GetInstance().GetWindowUnits();
    
    if (windowUnits.empty()) {
        OutputDebugStringW(L"[ControlToolbox] 支持库中没有窗口组件\n");
        return;
    }
    
    OutputDebugStringW((L"[ControlToolbox] 从支持库加载 " + std::to_wstring(windowUnits.size()) + L" 个窗口组件\n").c_str());
    
    for (const auto& unit : windowUnits) {
        // 跳过窗口类型（窗口不在组件箱中选择）
        if (unit.name == L"窗口" || unit.name == L"窗体" || unit.name == L"启动窗口") {
            continue;
        }
        
        // 查找或创建分类
        ToolboxCategory* category = FindOrCreateCategory(unit.category);
        
        // 创建工具箱项
        ToolboxItem* item = new ToolboxItem();
        item->type = unit.name;
        item->displayName = unit.name;
        item->category = unit.category;
        
        category->items.push_back(item);
        m_itemMap[unit.name] = item;
        
        OutputDebugStringW((L"[ControlToolbox] 添加组件: " + unit.name + L" -> " + unit.category + L"\n").c_str());
    }
}

// 查找或创建分类
ToolboxCategory* ControlToolbox::FindOrCreateCategory(const std::wstring& categoryName)
{
    // 查找现有分类
    for (auto& cat : m_categories) {
        if (cat.name == categoryName) {
            return &cat;
        }
    }
    
    // 创建新分类
    ToolboxCategory newCat;
    newCat.name = categoryName;
    newCat.expanded = true;
    m_categories.push_back(newCat);
    
    return &m_categories.back();
}

void ControlToolbox::OnPaint(HDC hdc)
{
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    
    // 双缓冲绘制
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
    
    Graphics graphics(memDC);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);
    graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
    
    // 填充背景
    SolidBrush bgBrush(ColorRefToGdiplus(g_CurrentTheme.editorBg));
    graphics.FillRectangle(&bgBrush, 0, 0, rect.right, rect.bottom);
    
    // 绘制标题栏（用于拖动）
    SolidBrush headerBrush(ColorRefToGdiplus(g_CurrentTheme.tableHeaderBg));
    graphics.FillRectangle(&headerBrush, 0, 0, rect.right, 30);
    
    Font titleFont(L"微软雅黑", 11, FontStyleBold);
    SolidBrush textBrush(ColorRefToGdiplus(g_CurrentTheme.text));
    graphics.DrawString(L"组件箱", -1, &titleFont, PointF(8, 6), &textBrush);
    
    // 绘制控件列表
    Font itemFont(L"微软雅黑", 10);
    int y = 35 - m_scrollPos;
    int itemIndex = 0;
    
    for (auto& cat : m_categories) {
        if (y > rect.bottom) break;
        
        // 绘制分类标题
        if (y + CATEGORY_HEIGHT > 0) {
            SolidBrush catBrush(Color(50, 50, 50));
            graphics.FillRectangle(&catBrush, 0, y, rect.right, CATEGORY_HEIGHT);
            
            // 绘制展开/折叠图标
            std::wstring arrow = cat.expanded ? L"▼" : L"▶";
            graphics.DrawString(arrow.c_str(), -1, &itemFont, PointF(4, (float)y + 4), &textBrush);
            
            // 绘制分类名
            graphics.DrawString(cat.name.c_str(), -1, &itemFont, PointF(20, (float)y + 4), &textBrush);
        }
        y += CATEGORY_HEIGHT;
        
        if (cat.expanded) {
            for (auto item : cat.items) {
                if (y > rect.bottom) break;
                
                if (y + ITEM_HEIGHT > 0) {
                    // 选中或悬停高亮
                    bool isSelected = (item->type == m_selectedType);
                    bool isHovered = (itemIndex == m_hoveredIndex);
                    
                    if (isSelected) {
                        SolidBrush selBrush(Color(0, 122, 204));
                        graphics.FillRectangle(&selBrush, 0, y, rect.right, ITEM_HEIGHT);
                    } else if (isHovered) {
                        SolidBrush hoverBrush(Color(60, 60, 60));
                        graphics.FillRectangle(&hoverBrush, 0, y, rect.right, ITEM_HEIGHT);
                    }
                    
                    // 绘制控件名
                    graphics.DrawString(item->displayName.c_str(), -1, &itemFont, 
                                      PointF(MARGIN + ICON_SIZE + 4, (float)y + 6), &textBrush);
                }
                
                y += ITEM_HEIGHT;
                itemIndex++;
            }
        }
    }
    
    // 复制到屏幕
    BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
    
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
}

void ControlToolbox::OnSize(int width, int height)
{
    UpdateScrollBar();
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void ControlToolbox::OnLButtonDown(int x, int y)
{
    // 检查点击位置
    int clickY = y + m_scrollPos - 35;
    int itemY = 0;
    
    for (auto& cat : m_categories) {
        // 检查是否点击了分类标题
        if (clickY >= itemY && clickY < itemY + CATEGORY_HEIGHT) {
            cat.expanded = !cat.expanded;
            InvalidateRect(m_hWnd, NULL, FALSE);
            return;
        }
        itemY += CATEGORY_HEIGHT;
        
        if (cat.expanded) {
            for (auto item : cat.items) {
                if (clickY >= itemY && clickY < itemY + ITEM_HEIGHT) {
                    m_selectedType = item->type;
                    InvalidateRect(m_hWnd, NULL, FALSE);
                    
                    // 向主窗口发送选择变更通知
                    HWND hMainWnd = GetAncestor(m_hWnd, GA_ROOT);
                    if (hMainWnd) {
                        PostMessage(hMainWnd, WM_COMMAND, 
                                   MAKEWPARAM(0, WM_TOOLBOX_SELECTION_CHANGED), 
                                   (LPARAM)m_selectedType.c_str());
                    }
                    return;
                }
                itemY += ITEM_HEIGHT;
            }
        }
    }
}

void ControlToolbox::OnLButtonDblClick(int x, int y)
{
    // 双击可以直接在设计器中创建控件
    // TODO: 通知设计器创建控件
}

void ControlToolbox::OnMouseMove(int x, int y)
{
    int oldHovered = m_hoveredIndex;
    m_hoveredIndex = HitTestItem(y);
    
    if (m_hoveredIndex != oldHovered) {
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void ControlToolbox::OnVScroll(WPARAM wParam)
{
    int action = LOWORD(wParam);
    int oldPos = m_scrollPos;
    
    switch (action) {
        case SB_LINEUP:
            m_scrollPos = std::max(0, m_scrollPos - ITEM_HEIGHT);
            break;
        case SB_LINEDOWN:
            m_scrollPos += ITEM_HEIGHT;
            break;
        case SB_PAGEUP:
            m_scrollPos = std::max(0, m_scrollPos - 100);
            break;
        case SB_PAGEDOWN:
            m_scrollPos += 100;
            break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
            m_scrollPos = HIWORD(wParam);
            break;
    }
    
    // 限制滚动范围
    int maxScroll = GetTotalHeight() - 100;
    if (maxScroll < 0) maxScroll = 0;
    m_scrollPos = std::min(m_scrollPos, maxScroll);
    
    if (m_scrollPos != oldPos) {
        SetScrollPos(m_hWnd, SB_VERT, m_scrollPos, TRUE);
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void ControlToolbox::OnMouseWheel(WPARAM wParam)
{
    int delta = GET_WHEEL_DELTA_WPARAM(wParam);
    int oldPos = m_scrollPos;
    
    // 每次滚动3行
    int scrollAmount = (ITEM_HEIGHT * 3) * delta / WHEEL_DELTA;
    m_scrollPos -= scrollAmount;
    
    // 限制滚动范围
    if (m_scrollPos < 0) m_scrollPos = 0;
    int maxScroll = GetTotalHeight() - 100;
    if (maxScroll < 0) maxScroll = 0;
    m_scrollPos = std::min(m_scrollPos, maxScroll);
    
    if (m_scrollPos != oldPos) {
        SetScrollPos(m_hWnd, SB_VERT, m_scrollPos, TRUE);
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void ControlToolbox::LoadFromRenderer(ControlRenderer* renderer)
{
    ToolboxDebugLog(L"========== LoadFromRenderer 开始 ==========");
    
    if (!renderer) {
        ToolboxDebugLog(L"错误: renderer 为 nullptr!");
        return;
    }
    
    // 清空现有分类
    ToolboxDebugLog(L"清空现有分类，当前有 " + std::to_wstring(m_categories.size()) + L" 个分类");
    for (auto& cat : m_categories) {
        for (auto item : cat.items) {
            delete item;
        }
        cat.items.clear();
    }
    m_categories.clear();
    
    // 获取所有分类
    auto categories = renderer->GetControlCategories();
    ToolboxDebugLog(L"从 renderer 获取到 " + std::to_wstring(categories.size()) + L" 个分类");
    
    for (const auto& catName : categories) {
        ToolboxDebugLog(L"\n处理分类: " + catName);
        
        ToolboxCategory category;
        category.name = catName;
        category.expanded = true;
        
        // 首先添加"指针"选项用于取消选择
        ToolboxItem* pointerItem = new ToolboxItem();
        pointerItem->type = L"";
        pointerItem->displayName = L"↖ 指针";
        pointerItem->category = catName;
        pointerItem->icon = nullptr;
        category.items.push_back(pointerItem);
        
        // 获取该分类下的所有控件
        auto controls = renderer->GetControlsInCategory(catName);
        ToolboxDebugLog(L"  该分类有 " + std::to_wstring(controls.size()) + L" 个控件");
        
        for (const auto& controlType : controls) {
            auto metadata = renderer->GetMetadata(controlType);
            
            ToolboxItem* item = new ToolboxItem();
            item->type = controlType;
            item->displayName = metadata.displayName.empty() ? controlType : metadata.displayName;
            item->category = catName;
            item->icon = nullptr;  // TODO: 加载图标
            
            category.items.push_back(item);
            ToolboxDebugLog(L"    添加控件: " + controlType);
        }
        
        if (!category.items.empty()) {
            m_categories.push_back(category);
            ToolboxDebugLog(L"  分类已添加到列表");
        } else {
            ToolboxDebugLog(L"  分类为空，跳过");
        }
    }
    
    ToolboxDebugLog(L"\n最终结果: " + std::to_wstring(m_categories.size()) + L" 个分类");
    for (const auto& cat : m_categories) {
        ToolboxDebugLog(L"  - " + cat.name + L": " + std::to_wstring(cat.items.size()) + L" 个控件");
    }
    
    ToolboxDebugLog(L"========== LoadFromRenderer 结束 ==========");
    
    // 刷新显示
    InvalidateRect(m_hWnd, NULL, FALSE);
}

std::wstring ControlToolbox::GetSelectedControlType() const
{
    return m_selectedType;
}

void ControlToolbox::ClearSelection()
{
    m_selectedType.clear();
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void ControlToolbox::SetSearchText(const std::wstring& text)
{
    m_searchText = text;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void ControlToolbox::ExpandCategory(const std::wstring& category, bool expand)
{
    for (auto& cat : m_categories) {
        if (cat.name == category) {
            cat.expanded = expand;
            break;
        }
    }
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void ControlToolbox::ExpandAll()
{
    for (auto& cat : m_categories) {
        cat.expanded = true;
    }
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void ControlToolbox::CollapseAll()
{
    for (auto& cat : m_categories) {
        cat.expanded = false;
    }
    InvalidateRect(m_hWnd, NULL, FALSE);
}

int ControlToolbox::HitTestItem(int y)
{
    int clickY = y + m_scrollPos - 35;
    int itemY = 0;
    int index = 0;
    
    for (auto& cat : m_categories) {
        itemY += CATEGORY_HEIGHT;
        
        if (cat.expanded) {
            for (auto item : cat.items) {
                if (clickY >= itemY && clickY < itemY + ITEM_HEIGHT) {
                    return index;
                }
                itemY += ITEM_HEIGHT;
                index++;
            }
        }
    }
    
    return -1;
}

int ControlToolbox::GetTotalHeight()
{
    int height = 0;
    for (auto& cat : m_categories) {
        height += CATEGORY_HEIGHT;
        if (cat.expanded) {
            height += (int)cat.items.size() * ITEM_HEIGHT;
        }
    }
    return height;
}

void ControlToolbox::UpdateScrollBar()
{
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    
    SCROLLINFO si = {0};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
    si.nMin = 0;
    si.nMax = GetTotalHeight();
    si.nPage = rect.bottom - 35;
    si.nPos = m_scrollPos;
    
    SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
}