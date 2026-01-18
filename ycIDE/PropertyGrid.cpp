#include "PropertyGrid.h"
#include "EditorContext.h"
#include "Theme.h"
#include <algorithm>

extern AppTheme g_CurrentTheme;

// PropertyGrid 构造函数
PropertyGrid::PropertyGrid(HWND hWnd)
    : m_hWnd(hWnd)
    , m_categorized(true)
    , m_alphabetic(false)
    , m_selectedRow(-1)
    , m_editingRow(-1)
    , m_hEditControl(nullptr)
    , m_hComboBox(nullptr)
    , m_scrollPos(0)
    , m_rowHeight(ROW_HEIGHT)
    , m_gdiplusToken(0)
{
    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

PropertyGrid::~PropertyGrid()
{
    if (m_gdiplusToken) {
        GdiplusShutdown(m_gdiplusToken);
    }
}

void PropertyGrid::OnPaint(HDC hdc)
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
    SolidBrush bgBrush(Color(g_CurrentTheme.editorBg));
    graphics.FillRectangle(&bgBrush, 0, 0, rect.right, rect.bottom);
    
    // 绘制标题栏
    SolidBrush headerBrush(Color(g_CurrentTheme.tableHeaderBg));
    graphics.FillRectangle(&headerBrush, 0, 0, rect.right, 30);
    
    // 绘制标题文字
    Font titleFont(L"微软雅黑", 11, FontStyleBold);
    SolidBrush textBrush(Color(g_CurrentTheme.text));
    
    std::wstring title = L"属性";
    if (!m_objectName.empty()) {
        title = m_objectType + L" - " + m_objectName;
    }
    graphics.DrawString(title.c_str(), -1, &titleFont, PointF(8, 6), &textBrush);
    
    // 绘制属性列表
    Font propFont(L"微软雅黑", 10);
    int y = 35;
    
    if (m_properties.empty()) {
        // 显示提示文字
        SolidBrush hintBrush(Color(128, 128, 128));
        graphics.DrawString(L"选择一个控件以查看其属性", -1, &propFont, 
                          PointF(10, (float)y), &hintBrush);
    } else {
        // 绘制属性
        for (const auto& prop : m_properties) {
            if (y > rect.bottom) break;
            
            // 绘制属性名
            RectF nameRect(4, (float)y, NAME_COLUMN_WIDTH - 8, (float)m_rowHeight);
            graphics.DrawString(prop.displayName.c_str(), -1, &propFont, nameRect, nullptr, &textBrush);
            
            // 绘制分隔线
            Pen linePen(Color(60, 60, 60));
            graphics.DrawLine(&linePen, NAME_COLUMN_WIDTH, y, NAME_COLUMN_WIDTH, y + m_rowHeight);
            
            // 绘制属性值
            RectF valueRect((float)NAME_COLUMN_WIDTH + 4, (float)y, 
                           (float)(rect.right - NAME_COLUMN_WIDTH - 8), (float)m_rowHeight);
            graphics.DrawString(prop.value.c_str(), -1, &propFont, valueRect, nullptr, &textBrush);
            
            // 绘制行分隔线
            graphics.DrawLine(&linePen, 0, y + m_rowHeight, rect.right, y + m_rowHeight);
            
            y += m_rowHeight;
        }
    }
    
    // 复制到屏幕
    BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
    
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
}

void PropertyGrid::OnSize(int width, int height)
{
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void PropertyGrid::OnLButtonDown(int x, int y)
{
    // TODO: 实现属性选择
}

void PropertyGrid::OnLButtonUp(int x, int y)
{
    // TODO
}

void PropertyGrid::OnMouseMove(int x, int y)
{
    // TODO
}

void PropertyGrid::OnLButtonDblClick(int x, int y)
{
    // TODO: 开始编辑属性
}

void PropertyGrid::OnVScroll(WPARAM wParam)
{
    // TODO: 滚动处理
}

void PropertyGrid::SetProperties(const std::vector<PropertyDef>& properties)
{
    m_properties = properties;
    m_propertyMap.clear();
    for (auto& prop : m_properties) {
        m_propertyMap[prop.name] = &prop;
    }
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void PropertyGrid::UpdateProperty(const std::wstring& name, const std::wstring& value)
{
    auto it = m_propertyMap.find(name);
    if (it != m_propertyMap.end()) {
        it->second->value = value;
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

std::wstring PropertyGrid::GetPropertyValue(const std::wstring& name) const
{
    auto it = m_propertyMap.find(name);
    if (it != m_propertyMap.end()) {
        return it->second->value;
    }
    return L"";
}

void PropertyGrid::Clear()
{
    m_properties.clear();
    m_propertyMap.clear();
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void PropertyGrid::SetObjectInfo(const std::wstring& objectType, const std::wstring& objectName)
{
    m_objectType = objectType;
    m_objectName = objectName;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void PropertyGrid::SetPropertyChangedCallback(PropertyChangedCallback callback)
{
    m_propertyChangedCallback = callback;
}

void PropertyGrid::SetCategorized(bool categorized)
{
    m_categorized = categorized;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void PropertyGrid::SetAlphabetic(bool alphabetic)
{
    m_alphabetic = alphabetic;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void PropertyGrid::SetSearchText(const std::wstring& text)
{
    m_searchText = text;
    InvalidateRect(m_hWnd, NULL, FALSE);
}
