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
extern HWND hMainWnd;  // 主窗口句柄

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
    , m_isDocked(false)
    , m_displayMode(MODE_LIST)
    , m_hoveredButton(BTN_NONE)
    , m_buttonPressed(false)
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
    
    // 绘制边框
    Pen borderPen(ColorRefToGdiplus(g_CurrentTheme.border), 1);
    graphics.DrawRectangle(&borderPen, 0, 0, rect.right - 1, rect.bottom - 1);
    
    // 绘制自定义标题栏
    SolidBrush headerBrush(ColorRefToGdiplus(g_CurrentTheme.tableHeaderBg));
    graphics.FillRectangle(&headerBrush, 1, 1, rect.right - 2, TITLEBAR_HEIGHT);
    
    // 标题文字
    Font titleFont(L"微软雅黑", 10, FontStyleBold);
    SolidBrush textBrush(ColorRefToGdiplus(g_CurrentTheme.text));
    graphics.DrawString(L"组件箱", -1, &titleFont, PointF(8, 5), &textBrush);
    
    // ===== 绘制标题栏按钮 =====
    int btnY = (TITLEBAR_HEIGHT - TITLEBAR_BUTTON_SIZE) / 2;
    int closeX = rect.right - TITLEBAR_BUTTON_SIZE - 6;
    int dockX = closeX - TITLEBAR_BUTTON_SIZE - 4;
    int viewModeX = dockX - TITLEBAR_BUTTON_SIZE - 4;  // 显示模式按钮
    
    // 显示模式切换按钮
    if (m_hoveredButton == BTN_VIEW_MODE) {
        SolidBrush hoverBrush(ColorRefToGdiplus(g_CurrentTheme.explorerTabHover));
        graphics.FillRectangle(&hoverBrush, viewModeX - 2, btnY - 2, 
                              TITLEBAR_BUTTON_SIZE + 4, TITLEBAR_BUTTON_SIZE + 4);
    }
    // 绘制显示模式图标
    Pen viewPen(ColorRefToGdiplus(g_CurrentTheme.text), 1.5f);
    if (m_displayMode == MODE_LIST) {
        // 列表模式图标 - 三条横线
        graphics.DrawLine(&viewPen, viewModeX + 4, btnY + 5, viewModeX + 16, btnY + 5);
        graphics.DrawLine(&viewPen, viewModeX + 4, btnY + 10, viewModeX + 16, btnY + 10);
        graphics.DrawLine(&viewPen, viewModeX + 4, btnY + 15, viewModeX + 16, btnY + 15);
    } else {
        // 图标模式图标 - 四个方块
        graphics.DrawRectangle(&viewPen, viewModeX + 4, btnY + 4, 6, 6);
        graphics.DrawRectangle(&viewPen, viewModeX + 12, btnY + 4, 6, 6);
        graphics.DrawRectangle(&viewPen, viewModeX + 4, btnY + 12, 6, 6);
        graphics.DrawRectangle(&viewPen, viewModeX + 12, btnY + 12, 6, 6);
    }
    
    // 停靠/浮动按钮
    if (m_hoveredButton == BTN_DOCK) {
        SolidBrush hoverBrush(ColorRefToGdiplus(g_CurrentTheme.explorerTabHover));
        graphics.FillRectangle(&hoverBrush, dockX - 2, btnY - 2, 
                              TITLEBAR_BUTTON_SIZE + 4, TITLEBAR_BUTTON_SIZE + 4);
    }
    // 绘制图钉图标
    Pen pinPen(ColorRefToGdiplus(g_CurrentTheme.text), 1.5f);
    if (m_isDocked) {
        // 已停靠 - 绘制向下的图钉 (锁定状态)
        graphics.DrawLine(&pinPen, dockX + 10, btnY + 4, dockX + 10, btnY + 14);
        graphics.DrawLine(&pinPen, dockX + 6, btnY + 8, dockX + 14, btnY + 8);
        graphics.DrawEllipse(&pinPen, dockX + 7, btnY + 12, 6, 6);
    } else {
        // 浮动 - 绘制斜向的图钉 (解锁状态)
        graphics.DrawLine(&pinPen, dockX + 6, btnY + 6, dockX + 14, btnY + 14);
        graphics.DrawLine(&pinPen, dockX + 4, btnY + 10, dockX + 10, btnY + 4);
        graphics.DrawEllipse(&pinPen, dockX + 11, btnY + 11, 5, 5);
    }
    
    // 关闭按钮
    if (m_hoveredButton == BTN_CLOSE) {
        SolidBrush hoverBrush(Color(196, 43, 28));  // 红色悬停保持不变
        graphics.FillRectangle(&hoverBrush, closeX - 2, btnY - 2, 
                              TITLEBAR_BUTTON_SIZE + 4, TITLEBAR_BUTTON_SIZE + 4);
    }
    // 绘制 X 图标
    Pen closePen(m_hoveredButton == BTN_CLOSE ? Color(255, 255, 255) : ColorRefToGdiplus(g_CurrentTheme.text), 1.5f);
    graphics.DrawLine(&closePen, closeX + 5, btnY + 5, closeX + 15, btnY + 15);
    graphics.DrawLine(&closePen, closeX + 15, btnY + 5, closeX + 5, btnY + 15);
    
    // 绘制标题栏底部分隔线
    Pen sepPen(ColorRefToGdiplus(g_CurrentTheme.border), 1);
    graphics.DrawLine(&sepPen, 0, TITLEBAR_HEIGHT, rect.right, TITLEBAR_HEIGHT);
    
    // 设置裁剪区域，确保控件列表不会绘制到标题栏上
    graphics.SetClip(Rect(0, TITLEBAR_HEIGHT + 1, rect.right, rect.bottom - TITLEBAR_HEIGHT - 1));
    
    // 绘制控件列表
    Font itemFont(L"微软雅黑", 10);
    int y = TITLEBAR_HEIGHT + 5 - m_scrollPos;
    int itemIndex = 0;
    
    if (m_displayMode == MODE_ICON) {
        // ===== 图标模式 =====
        const int ICON_ITEM_SIZE = 48;  // 每个图标项的大小
        const int ICON_PADDING = 4;     // 图标间距
        int itemsPerRow = (rect.right - ICON_PADDING * 2) / (ICON_ITEM_SIZE + ICON_PADDING);
        if (itemsPerRow < 1) itemsPerRow = 1;
        
        for (auto& cat : m_categories) {
            if (y > rect.bottom) break;
            
            // 绘制分类标题
            if (y + CATEGORY_HEIGHT > 0) {
                SolidBrush catBrush(ColorRefToGdiplus(g_CurrentTheme.tableHeaderBg));
                graphics.FillRectangle(&catBrush, 0, y, rect.right, CATEGORY_HEIGHT);
                
                std::wstring arrow = cat.expanded ? L"▼" : L"▶";
                graphics.DrawString(arrow.c_str(), -1, &itemFont, PointF(4, (float)y + 4), &textBrush);
                graphics.DrawString(cat.name.c_str(), -1, &itemFont, PointF(20, (float)y + 4), &textBrush);
            }
            y += CATEGORY_HEIGHT;
            
            if (cat.expanded) {
                int col = 0;
                int rowStartY = y;
                
                for (auto item : cat.items) {
                    int itemX = ICON_PADDING + col * (ICON_ITEM_SIZE + ICON_PADDING);
                    int itemY = rowStartY;
                    
                    if (itemY > rect.bottom) break;
                    
                    if (itemY + ICON_ITEM_SIZE > 0) {
                        bool isSelected = (item->type == m_selectedType);
                        bool isHovered = (itemIndex == m_hoveredIndex);
                        
                        // 绘制背景
                        if (isSelected) {
                            SolidBrush selBrush(ColorRefToGdiplus(g_CurrentTheme.activityBarIndicator));
                            graphics.FillRectangle(&selBrush, itemX, itemY, ICON_ITEM_SIZE, ICON_ITEM_SIZE);
                        } else if (isHovered) {
                            SolidBrush hoverBrush(ColorRefToGdiplus(g_CurrentTheme.explorerTabHover));
                            graphics.FillRectangle(&hoverBrush, itemX, itemY, ICON_ITEM_SIZE, ICON_ITEM_SIZE);
                        }
                        
                        // 绘制边框
                        Pen itemBorderPen(ColorRefToGdiplus(g_CurrentTheme.border), 1);
                        graphics.DrawRectangle(&itemBorderPen, itemX, itemY, ICON_ITEM_SIZE - 1, ICON_ITEM_SIZE - 1);
                        
                        // 绘制控件名（截断显示）
                        Font iconFont(L"微软雅黑", 8);
                        std::wstring name = item->displayName;
                        if (name.length() > 4) {
                            name = name.substr(0, 4) + L"..";
                        }
                        RectF nameRect((float)itemX, (float)itemY + ICON_ITEM_SIZE - 16, (float)ICON_ITEM_SIZE, 16);
                        StringFormat sf;
                        sf.SetAlignment(StringAlignmentCenter);
                        sf.SetLineAlignment(StringAlignmentCenter);
                        graphics.DrawString(name.c_str(), -1, &iconFont, nameRect, &sf, &textBrush);
                    }
                    
                    col++;
                    if (col >= itemsPerRow) {
                        col = 0;
                        rowStartY += ICON_ITEM_SIZE + ICON_PADDING;
                    }
                    itemIndex++;
                }
                
                // 更新y到下一分类
                int rows = ((int)cat.items.size() + itemsPerRow - 1) / itemsPerRow;
                y = rowStartY + (col > 0 ? ICON_ITEM_SIZE + ICON_PADDING : 0);
                if (col == 0 && !cat.items.empty()) {
                    y = rowStartY;
                }
            }
        }
    } else {
        // ===== 列表模式 =====
        for (auto& cat : m_categories) {
            if (y > rect.bottom) break;
            
            // 绘制分类标题
            if (y + CATEGORY_HEIGHT > 0) {
                SolidBrush catBrush(ColorRefToGdiplus(g_CurrentTheme.tableHeaderBg));
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
                            SolidBrush selBrush(ColorRefToGdiplus(g_CurrentTheme.activityBarIndicator));
                            graphics.FillRectangle(&selBrush, 0, y, rect.right, ITEM_HEIGHT);
                        } else if (isHovered) {
                            SolidBrush hoverBrush(ColorRefToGdiplus(g_CurrentTheme.explorerTabHover));
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
    // 先检查是否点击了标题栏按钮
    TitlebarButton btn = HitTestTitlebarButton(x, y);
    if (btn != BTN_NONE) {
        m_buttonPressed = true;
        return;  // 标题栏按钮的处理在 OnLButtonUp 中完成
    }
    
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    
    // 检查点击位置
    int clickY = y + m_scrollPos - (TITLEBAR_HEIGHT + 5);
    int itemY = 0;
    
    if (m_displayMode == MODE_ICON) {
        // ===== 图标模式点击处理 =====
        const int ICON_ITEM_SIZE = 48;
        const int ICON_PADDING = 4;
        int itemsPerRow = (rect.right - ICON_PADDING * 2) / (ICON_ITEM_SIZE + ICON_PADDING);
        if (itemsPerRow < 1) itemsPerRow = 1;
        
        for (auto& cat : m_categories) {
            // 检查是否点击了分类标题
            if (clickY >= itemY && clickY < itemY + CATEGORY_HEIGHT) {
                cat.expanded = !cat.expanded;
                InvalidateRect(m_hWnd, NULL, FALSE);
                return;
            }
            itemY += CATEGORY_HEIGHT;
            
            if (cat.expanded) {
                int col = 0;
                int rowStartY = itemY;
                
                for (auto item : cat.items) {
                    int itemX = ICON_PADDING + col * (ICON_ITEM_SIZE + ICON_PADDING);
                    int itemEndX = itemX + ICON_ITEM_SIZE;
                    int itemEndY = rowStartY + ICON_ITEM_SIZE;
                    
                    // 检查是否点击在此图标项内
                    if (x >= itemX && x < itemEndX && clickY >= rowStartY && clickY < itemEndY) {
                        m_selectedType = item->type;
                        InvalidateRect(m_hWnd, NULL, FALSE);
                        
                        OutputDebugStringW((L"[ControlToolbox] 选中组件: " + m_selectedType + L"\n").c_str());
                        
                        if (hMainWnd) {
                            PostMessage(hMainWnd, WM_COMMAND, 
                                       MAKEWPARAM(0, WM_TOOLBOX_SELECTION_CHANGED), 
                                       (LPARAM)m_selectedType.c_str());
                        }
                        return;
                    }
                    
                    col++;
                    if (col >= itemsPerRow) {
                        col = 0;
                        rowStartY += ICON_ITEM_SIZE + ICON_PADDING;
                    }
                }
                
                // 更新 itemY 到下一分类
                int rows = ((int)cat.items.size() + itemsPerRow - 1) / itemsPerRow;
                itemY = rowStartY + (col > 0 ? ICON_ITEM_SIZE + ICON_PADDING : 0);
                if (col == 0 && !cat.items.empty()) {
                    itemY = rowStartY;
                }
            }
        }
    } else {
        // ===== 列表模式点击处理 =====
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
                        
                        OutputDebugStringW((L"[ControlToolbox] 选中组件: " + m_selectedType + L"\n").c_str());
                        
                        if (hMainWnd) {
                            OutputDebugStringW(L"[ControlToolbox] 发送消息到主窗口\n");
                            PostMessage(hMainWnd, WM_COMMAND, 
                                       MAKEWPARAM(0, WM_TOOLBOX_SELECTION_CHANGED), 
                                       (LPARAM)m_selectedType.c_str());
                        } else {
                            OutputDebugStringW(L"[ControlToolbox] 主窗口句柄为空!\n");
                        }
                        return;
                    }
                    itemY += ITEM_HEIGHT;
                }
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
    // 检查标题栏按钮悬停
    TitlebarButton oldHoveredButton = m_hoveredButton;
    m_hoveredButton = HitTestTitlebarButton(x, y);
    
    if (m_hoveredButton != oldHoveredButton) {
        // 只重绘标题栏区域
        RECT titleRect;
        GetClientRect(m_hWnd, &titleRect);
        titleRect.bottom = TITLEBAR_HEIGHT + 1;
        InvalidateRect(m_hWnd, &titleRect, FALSE);
    }
    
    // 检查控件项悬停
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
            // 跳过窗口类型（窗口不在组件箱中选择）
            if (controlType == L"窗口" || controlType == L"窗体" || controlType == L"启动窗口" ||
                controlType == L"Window" || controlType == L"Form") {
                ToolboxDebugLog(L"    跳过窗口类型: " + controlType);
                continue;
            }
            
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
    // 图标模式需要同时检测 x 坐标，这里只返回基于 y 的大概索引
    // 具体的图标模式命中测试在 OnLButtonDown 中处理
    if (m_displayMode == MODE_ICON) {
        // 图标模式的命中测试在 OnLButtonDown 中用 x,y 一起处理
        return -1;
    }
    
    int clickY = y + m_scrollPos - (TITLEBAR_HEIGHT + 5);
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
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    
    int height = 0;
    
    if (m_displayMode == MODE_ICON) {
        // 图标模式计算高度
        const int ICON_ITEM_SIZE = 48;
        const int ICON_PADDING = 4;
        int itemsPerRow = (rect.right - ICON_PADDING * 2) / (ICON_ITEM_SIZE + ICON_PADDING);
        if (itemsPerRow < 1) itemsPerRow = 1;
        
        for (auto& cat : m_categories) {
            height += CATEGORY_HEIGHT;
            if (cat.expanded) {
                int rows = ((int)cat.items.size() + itemsPerRow - 1) / itemsPerRow;
                height += rows * (ICON_ITEM_SIZE + ICON_PADDING);
            }
        }
    } else {
        // 列表模式计算高度
        for (auto& cat : m_categories) {
            height += CATEGORY_HEIGHT;
            if (cat.expanded) {
                height += (int)cat.items.size() * ITEM_HEIGHT;
            }
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
    si.nPage = rect.bottom - (TITLEBAR_HEIGHT + 5);
    si.nPos = m_scrollPos;
    
    SetScrollInfo(m_hWnd, SB_VERT, &si, TRUE);
}

// 标题栏按钮命中测试
ControlToolbox::TitlebarButton ControlToolbox::HitTestTitlebarButton(int x, int y)
{
    // 只在标题栏区域内检测
    if (y < 0 || y > TITLEBAR_HEIGHT) {
        return BTN_NONE;
    }
    
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    
    int btnY = (TITLEBAR_HEIGHT - TITLEBAR_BUTTON_SIZE) / 2;
    int closeX = rect.right - TITLEBAR_BUTTON_SIZE - 6;
    int dockX = closeX - TITLEBAR_BUTTON_SIZE - 4;
    int viewModeX = dockX - TITLEBAR_BUTTON_SIZE - 4;
    
    // 检测关闭按钮
    if (x >= closeX - 2 && x <= closeX + TITLEBAR_BUTTON_SIZE + 2 &&
        y >= btnY - 2 && y <= btnY + TITLEBAR_BUTTON_SIZE + 2) {
        return BTN_CLOSE;
    }
    
    // 检测停靠按钮
    if (x >= dockX - 2 && x <= dockX + TITLEBAR_BUTTON_SIZE + 2 &&
        y >= btnY - 2 && y <= btnY + TITLEBAR_BUTTON_SIZE + 2) {
        return BTN_DOCK;
    }
    
    // 检测显示模式按钮
    if (x >= viewModeX - 2 && x <= viewModeX + TITLEBAR_BUTTON_SIZE + 2 &&
        y >= btnY - 2 && y <= btnY + TITLEBAR_BUTTON_SIZE + 2) {
        return BTN_VIEW_MODE;
    }
    
    return BTN_NONE;
}

void ControlToolbox::OnLButtonUp(int x, int y)
{
    if (m_buttonPressed) {
        m_buttonPressed = false;
        
        TitlebarButton btn = HitTestTitlebarButton(x, y);
        if (btn == BTN_VIEW_MODE) {
            // 切换显示模式
            ToggleDisplayMode();
            // 发送显示模式切换消息（通知主窗口可能需要调整宽度）
            if (hMainWnd) {
                PostMessage(hMainWnd, WM_COMMAND, 
                           MAKEWPARAM(0, WM_TOOLBOX_VIEW_MODE), 
                           (LPARAM)m_hWnd);
            }
        } else if (btn == BTN_DOCK) {
            // 发送停靠/浮动切换消息
            if (hMainWnd) {
                PostMessage(hMainWnd, WM_COMMAND, 
                           MAKEWPARAM(0, WM_TOOLBOX_DOCK_TOGGLE), 
                           (LPARAM)m_hWnd);
            }
        } else if (btn == BTN_CLOSE) {
            // 发送关闭消息
            if (hMainWnd) {
                PostMessage(hMainWnd, WM_COMMAND, 
                           MAKEWPARAM(0, WM_TOOLBOX_CLOSE), 
                           (LPARAM)m_hWnd);
            }
        }
    }
}