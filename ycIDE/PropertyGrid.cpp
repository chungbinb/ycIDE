#include "PropertyGrid.h"
#include "EditorContext.h"
#include "LibraryParser.h"
#include "Theme.h"
#include <algorithm>

extern AppTheme g_CurrentTheme;

// 辅助函数：将 COLORREF 转换为 GDI+ Color
inline Color ColorRefToColor(COLORREF cr) {
    return Color(255, GetRValue(cr), GetGValue(cr), GetBValue(cr));
}

// PropertyGrid 构造函数
PropertyGrid::PropertyGrid(HWND hWnd)
    : m_hWnd(hWnd)
    , m_categorized(true)
    , m_alphabetic(false)
    , m_selectedRow(-1)
    , m_editingRow(-1)
    , m_hEditControl(nullptr)
    , m_hComboBox(nullptr)
    , m_hDropdownPopup(nullptr)
    , m_dropdownSelectedIndex(-1)
    , m_dropdownHoverIndex(-1)
    , m_dropdownItemHeight(22)
    , m_dropdownCloseTime(0)
    , m_dropdownCloseRow(-1)
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
    SolidBrush bgBrush(ColorRefToColor(g_CurrentTheme.editorBg));
    graphics.FillRectangle(&bgBrush, 0, 0, rect.right, rect.bottom);
    
    // 绘制标题栏
    SolidBrush headerBrush(ColorRefToColor(g_CurrentTheme.tableHeaderBg));
    graphics.FillRectangle(&headerBrush, 0, 0, rect.right, 30);
    
    // 绘制标题文字
    Font titleFont(L"微软雅黑", 11, FontStyleBold);
    SolidBrush textBrush(ColorRefToColor(g_CurrentTheme.text));
    
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
        // 禁用状态的灰色画刷
        SolidBrush disabledBrush(Color(128, 128, 128));
        
        // 绘制属性
        for (const auto& prop : m_properties) {
            if (y > rect.bottom) break;
            
            // 根据是否只读选择文本颜色
            SolidBrush* currentBrush = prop.readOnly ? &disabledBrush : &textBrush;
            
            // 绘制属性名
            RectF nameRect(4, (float)y, NAME_COLUMN_WIDTH - 8, (float)m_rowHeight);
            graphics.DrawString(prop.displayName.c_str(), -1, &propFont, nameRect, nullptr, currentBrush);
            
            // 绘制分隔线
            Pen linePen(Color(60, 60, 60));
            graphics.DrawLine(&linePen, NAME_COLUMN_WIDTH, y, NAME_COLUMN_WIDTH, y + m_rowHeight);
            
            // 检查是否需要绘制选择按钮（资源类属性）
            bool needsPickButton = (prop.editorType == PropertyEditorType::Image ||
                                   prop.editorType == PropertyEditorType::File ||
                                   prop.editorType == PropertyEditorType::Font ||
                                   prop.fneType == PropertyType::Picture ||
                                   prop.fneType == PropertyType::Icon ||
                                   prop.fneType == PropertyType::ImageList ||
                                   prop.fneType == PropertyType::Music);
            
            int buttonWidth = needsPickButton ? 20 : 0;
            
            // 绘制属性值
            RectF valueRect((float)NAME_COLUMN_WIDTH + 4, (float)y, 
                           (float)(rect.right - NAME_COLUMN_WIDTH - 8 - buttonWidth), (float)m_rowHeight);
            graphics.DrawString(prop.value.c_str(), -1, &propFont, valueRect, nullptr, currentBrush);
            
            // 绘制选择按钮
            if (needsPickButton && !prop.readOnly) {
                int btnX = rect.right - buttonWidth - 2;
                int btnY = y + 2;
                int btnH = m_rowHeight - 4;
                
                // 按钮背景
                SolidBrush btnBrush(Color(60, 60, 60));
                graphics.FillRectangle(&btnBrush, btnX, btnY, buttonWidth - 2, btnH);
                
                // 按钮边框
                Pen btnPen(Color(80, 80, 80));
                graphics.DrawRectangle(&btnPen, btnX, btnY, buttonWidth - 2, btnH);
                
                // 按钮文字 "..."
                Font btnFont(L"微软雅黑", 8);
                SolidBrush btnTextBrush(Color(200, 200, 200));
                RectF btnTextRect((float)btnX, (float)btnY, (float)(buttonWidth - 2), (float)btnH);
                StringFormat btnFormat;
                btnFormat.SetAlignment(StringAlignmentCenter);
                btnFormat.SetLineAlignment(StringAlignmentCenter);
                graphics.DrawString(L"...", -1, &btnFont, btnTextRect, &btnFormat, &btnTextBrush);
            }
            
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
    // 检查是否点击在属性区域
    if (y < 35) return;  // 标题栏区域
    
    int row = (y - 35 + m_scrollPos) / m_rowHeight;
    if (row >= 0 && row < (int)m_properties.size()) {
        m_selectedRow = row;
        InvalidateRect(m_hWnd, NULL, FALSE);
        
        PropertyDef& prop = m_properties[row];
        
        // 检查是否是资源类属性需要选择按钮
        bool needsPickButton = (prop.editorType == PropertyEditorType::Image ||
                               prop.editorType == PropertyEditorType::File ||
                               prop.editorType == PropertyEditorType::Font ||
                               prop.fneType == PropertyType::Picture ||
                               prop.fneType == PropertyType::Icon ||
                               prop.fneType == PropertyType::ImageList ||
                               prop.fneType == PropertyType::Music);
        
        RECT clientRect;
        GetClientRect(m_hWnd, &clientRect);
        int buttonWidth = 20;
        int btnX = clientRect.right - buttonWidth - 2;
        
        // 如果点击了选择按钮
        if (needsPickButton && !prop.readOnly && x >= btnX) {
            ShowResourcePicker(prop);
            return;
        }
        
        // 如果点击值列，开始编辑
        if (x > NAME_COLUMN_WIDTH) {
            BeginEdit(row);
        }
    }
}

void PropertyGrid::OnLButtonUp(int x, int y)
{
    // 
}

void PropertyGrid::OnMouseMove(int x, int y)
{
    // 
}

void PropertyGrid::OnLButtonDblClick(int x, int y)
{
    // 双击属性行开始编辑
    if (y < 35) return;
    
    int row = (y - 35 + m_scrollPos) / m_rowHeight;
    if (row >= 0 && row < (int)m_properties.size()) {
        BeginEdit(row);
    }
}

void PropertyGrid::OnVScroll(WPARAM wParam)
{
    int action = LOWORD(wParam);
    int oldPos = m_scrollPos;
    
    switch (action) {
        case SB_LINEUP:
            m_scrollPos = (std::max)(0, m_scrollPos - m_rowHeight);
            break;
        case SB_LINEDOWN:
            m_scrollPos += m_rowHeight;
            break;
        case SB_PAGEUP:
            m_scrollPos = (std::max)(0, m_scrollPos - 100);
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
    int maxScroll = (int)m_properties.size() * m_rowHeight - 100;
    if (maxScroll < 0) maxScroll = 0;
    m_scrollPos = (std::min)(m_scrollPos, maxScroll);
    
    if (m_scrollPos != oldPos) {
        SetScrollPos(m_hWnd, SB_VERT, m_scrollPos, TRUE);
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

// 编辑功能
void PropertyGrid::BeginEdit(int row)
{
    if (row < 0 || row >= (int)m_properties.size()) return;
    
    PropertyDef& prop = m_properties[row];
    if (prop.readOnly) return;
    
    // 如果点击的是同一行且下拉列表已展开，则关闭它
    if (m_editingRow == row && m_hDropdownPopup && IsWindow(m_hDropdownPopup)) {
        CloseCustomDropdown(false);
        return;
    }
    
    // 如果刚刚关闭了同一行的下拉列表（200毫秒内），不要重新打开
    DWORD currentTime = GetTickCount();
    if (m_dropdownCloseRow == row && (currentTime - m_dropdownCloseTime) < 200) {
        m_dropdownCloseRow = -1;  // 重置，下次可以正常打开
        return;
    }
    
    // 结束之前的编辑
    if (m_editingRow >= 0) {
        FinishEdit(true);
    }
    
    m_editingRow = row;
    m_selectedRow = row;
    
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    
    // 检查是否需要选择按钮（资源类属性）
    bool needsPickButton = (prop.editorType == PropertyEditorType::Image ||
                           prop.editorType == PropertyEditorType::File ||
                           prop.editorType == PropertyEditorType::Font ||
                           prop.fneType == PropertyType::Picture ||
                           prop.fneType == PropertyType::Icon ||
                           prop.fneType == PropertyType::ImageList ||
                           prop.fneType == PropertyType::Music);
    
    int buttonWidth = needsPickButton ? 22 : 0;
    
    int editY = 35 + row * m_rowHeight - m_scrollPos + 2;
    int editX = NAME_COLUMN_WIDTH + 2;
    int editW = rect.right - NAME_COLUMN_WIDTH - 4 - buttonWidth;
    int editH = m_rowHeight - 4;
    
    // 根据属性类型创建不同的编辑控件
    switch (prop.editorType) {
        case PropertyEditorType::Boolean:
            // 布尔型使用下拉框选择真/假
            CreateBooleanComboEditor(prop, Rect(editX, editY, editW, editH));
            break;
            
        case PropertyEditorType::Enum:
            // 枚举型使用下拉框
            CreateComboEditor(prop, Rect(editX, editY, editW, editH));
            break;
            
        case PropertyEditorType::Color:
            // 颜色选择器
            ShowColorPicker(prop);
            m_editingRow = -1;
            break;
            
        default:
            // 默认使用文本编辑框
            CreateTextEditor(prop, Rect(editX, editY, editW, editH));
            break;
    }
}

void PropertyGrid::FinishEdit(bool apply)
{
    if (m_editingRow < 0) return;
    
    if (m_hEditControl && IsWindow(m_hEditControl)) {
        if (apply && m_editingRow < (int)m_properties.size()) {
            // 获取编辑框内容
            int len = GetWindowTextLength(m_hEditControl);
            std::wstring newValue(len + 1, L'\0');
            GetWindowText(m_hEditControl, &newValue[0], len + 1);
            newValue.resize(len);
            
            PropertyDef& prop = m_properties[m_editingRow];
            if (prop.value != newValue) {
                prop.value = newValue;
                
                // 触发属性变更回调
                if (m_propertyChangedCallback) {
                    m_propertyChangedCallback(prop.name, newValue);
                }
            }
        }
        
        DestroyWindow(m_hEditControl);
        m_hEditControl = nullptr;
    }
    
    if (m_hComboBox && IsWindow(m_hComboBox)) {
        if (apply && m_editingRow < (int)m_properties.size()) {
            int sel = (int)SendMessage(m_hComboBox, CB_GETCURSEL, 0, 0);
            if (sel >= 0) {
                int len = (int)SendMessage(m_hComboBox, CB_GETLBTEXTLEN, sel, 0);
                std::wstring newValue(len + 1, L'\0');
                SendMessage(m_hComboBox, CB_GETLBTEXT, sel, (LPARAM)&newValue[0]);
                newValue.resize(len);
                
                PropertyDef& prop = m_properties[m_editingRow];
                if (prop.value != newValue) {
                    prop.value = newValue;
                    
                    if (m_propertyChangedCallback) {
                        m_propertyChangedCallback(prop.name, newValue);
                    }
                }
            }
        }
        
        DestroyWindow(m_hComboBox);
        m_hComboBox = nullptr;
    }
    
    m_editingRow = -1;
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void PropertyGrid::ApplyEditWithoutFinish()
{
    if (m_editingRow < 0 || m_editingRow >= (int)m_properties.size()) return;
    
    if (m_hEditControl && IsWindow(m_hEditControl)) {
        // 获取编辑框内容
        int len = GetWindowTextLength(m_hEditControl);
        std::wstring newValue(len + 1, L'\0');
        GetWindowText(m_hEditControl, &newValue[0], len + 1);
        newValue.resize(len);
        
        PropertyDef& prop = m_properties[m_editingRow];
        if (prop.value != newValue) {
            prop.value = newValue;
            
            // 触发属性变更回调
            if (m_propertyChangedCallback) {
                m_propertyChangedCallback(prop.name, newValue);
            }
            
            // 刷新显示（不销毁编辑框）
            InvalidateRect(m_hWnd, NULL, FALSE);
        }
    }
}

void PropertyGrid::CreateTextEditor(const PropertyDef& prop, const Rect& rect)
{
    m_hEditControl = CreateWindowW(L"EDIT", prop.value.c_str(),
        WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
        rect.X, rect.Y, rect.Width, rect.Height,
        m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
    
    if (m_hEditControl) {
        SendMessage(m_hEditControl, EM_SETSEL, 0, -1);
        SetFocus(m_hEditControl);
    }
}

void PropertyGrid::CreateComboEditor(const PropertyDef& prop, const Rect& rect)
{
    // 使用自绘下拉列表
    ShowCustomDropdown(prop, rect);
}

void PropertyGrid::CreateBooleanComboEditor(const PropertyDef& prop, const Rect& rect)
{
    // 创建临时属性用于布尔值
    PropertyDef boolProp = prop;
    boolProp.enumValues.clear();
    boolProp.enumValues.push_back(L"真");
    boolProp.enumValues.push_back(L"假");
    
    // 使用自绘下拉列表
    ShowCustomDropdown(boolProp, rect);
}

void PropertyGrid::ToggleBooleanValue(PropertyDef& prop)
{
    if (prop.value == L"真" || prop.value == L"true" || prop.value == L"1") {
        prop.value = L"假";
    } else {
        prop.value = L"真";
    }
    
    if (m_propertyChangedCallback) {
        m_propertyChangedCallback(prop.name, prop.value);
    }
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

// 自绘下拉列表窗口过程
LRESULT CALLBACK PropertyGrid::DropdownWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    PropertyGrid* pGrid = (PropertyGrid*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
    
    switch (msg) {
        case WM_PAINT: {
            if (!pGrid) break;
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT rect;
            GetClientRect(hWnd, &rect);
            
            // 创建双缓冲
            HDC memDC = CreateCompatibleDC(hdc);
            HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
            
            Graphics graphics(memDC);
            graphics.SetSmoothingMode(SmoothingModeAntiAlias);
            graphics.SetTextRenderingHint(TextRenderingHintClearTypeGridFit);
            
            // 根据主题背景色判断是深色还是浅色主题
            bool isDarkTheme = (GetRValue(g_CurrentTheme.bg) < 128);
            
            // 根据主题设置颜色
            Color bgColor, borderColor, textColor, hoverColor, selectedColor;
            if (isDarkTheme) {
                bgColor = Color(45, 45, 48);
                borderColor = Color(70, 70, 70);
                textColor = Color(220, 220, 220);
                hoverColor = Color(62, 62, 64);
                selectedColor = Color(0, 122, 204);
            } else {
                bgColor = Color(255, 255, 255);
                borderColor = Color(200, 200, 200);
                textColor = Color(30, 30, 30);
                hoverColor = Color(229, 243, 255);
                selectedColor = Color(0, 120, 215);
            }
            
            // 背景
            SolidBrush bgBrush(bgColor);
            graphics.FillRectangle(&bgBrush, 0, 0, rect.right, rect.bottom);
            
            // 边框
            Pen borderPen(borderColor);
            graphics.DrawRectangle(&borderPen, 0, 0, rect.right - 1, rect.bottom - 1);
            
            // 绘制列表项
            Font itemFont(L"微软雅黑", 10);
            SolidBrush textBrush(textColor);
            SolidBrush textBrushSelected(Color(255, 255, 255));  // 选中项始终用白色文字
            SolidBrush hoverBrush(hoverColor);
            SolidBrush selectedBrush(selectedColor);
            
            int y = 2;
            for (int i = 0; i < (int)pGrid->m_dropdownItems.size(); i++) {
                RectF itemRect(2.0f, (float)y, (float)(rect.right - 4), (float)pGrid->m_dropdownItemHeight);
                
                // 绘制悬停或选中背景
                if (i == pGrid->m_dropdownSelectedIndex) {
                    graphics.FillRectangle(&selectedBrush, itemRect);
                    // 选中项使用白色文本
                    RectF textRect(6.0f, (float)y + 2, (float)(rect.right - 12), (float)pGrid->m_dropdownItemHeight - 4);
                    graphics.DrawString(pGrid->m_dropdownItems[i].c_str(), -1, &itemFont, textRect, nullptr, &textBrushSelected);
                } else {
                    if (i == pGrid->m_dropdownHoverIndex) {
                        graphics.FillRectangle(&hoverBrush, itemRect);
                    }
                    // 正常项使用主题文本颜色
                    RectF textRect(6.0f, (float)y + 2, (float)(rect.right - 12), (float)pGrid->m_dropdownItemHeight - 4);
                    graphics.DrawString(pGrid->m_dropdownItems[i].c_str(), -1, &itemFont, textRect, nullptr, &textBrush);
                }
                
                y += pGrid->m_dropdownItemHeight;
            }
            
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
            
            SelectObject(memDC, oldBitmap);
            DeleteObject(memBitmap);
            DeleteDC(memDC);
            
            EndPaint(hWnd, &ps);
            return 0;
        }
        
        case WM_MOUSEMOVE: {
            if (!pGrid) break;
            
            int y = HIWORD(lParam);
            int newHover = (y - 2) / pGrid->m_dropdownItemHeight;
            if (newHover >= 0 && newHover < (int)pGrid->m_dropdownItems.size()) {
                if (newHover != pGrid->m_dropdownHoverIndex) {
                    pGrid->m_dropdownHoverIndex = newHover;
                    InvalidateRect(hWnd, NULL, FALSE);
                }
            }
            
            // 启用鼠标追踪以便检测鼠标离开
            TRACKMOUSEEVENT tme = {0};
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hWnd;
            TrackMouseEvent(&tme);
            return 0;
        }
        
        case WM_MOUSELEAVE: {
            if (!pGrid) break;
            pGrid->m_dropdownHoverIndex = -1;
            InvalidateRect(hWnd, NULL, FALSE);
            return 0;
        }
        
        case WM_LBUTTONDOWN: {
            if (!pGrid) break;
            
            int y = HIWORD(lParam);
            int clickedIndex = (y - 2) / pGrid->m_dropdownItemHeight;
            if (clickedIndex >= 0 && clickedIndex < (int)pGrid->m_dropdownItems.size()) {
                pGrid->m_dropdownSelectedIndex = clickedIndex;
                pGrid->CloseCustomDropdown(true);
            }
            return 0;
        }
        
        case WM_KILLFOCUS: {
            if (pGrid) {
                pGrid->CloseCustomDropdown(false);
            }
            return 0;
        }
        
        case WM_KEYDOWN: {
            if (!pGrid) break;
            
            switch (wParam) {
                case VK_UP:
                    if (pGrid->m_dropdownSelectedIndex > 0) {
                        pGrid->m_dropdownSelectedIndex--;
                        InvalidateRect(hWnd, NULL, FALSE);
                    }
                    return 0;
                case VK_DOWN:
                    if (pGrid->m_dropdownSelectedIndex < (int)pGrid->m_dropdownItems.size() - 1) {
                        pGrid->m_dropdownSelectedIndex++;
                        InvalidateRect(hWnd, NULL, FALSE);
                    }
                    return 0;
                case VK_RETURN:
                    pGrid->CloseCustomDropdown(true);
                    return 0;
                case VK_ESCAPE:
                    pGrid->CloseCustomDropdown(false);
                    return 0;
            }
            break;
        }
    }
    
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void PropertyGrid::ShowCustomDropdown(const PropertyDef& prop, const Rect& rect)
{
    // 关闭已有的下拉列表
    if (m_hDropdownPopup && IsWindow(m_hDropdownPopup)) {
        DestroyWindow(m_hDropdownPopup);
        m_hDropdownPopup = nullptr;
    }
    
    // 保存列表项
    m_dropdownItems = prop.enumValues;
    m_dropdownItemHeight = 22;
    m_dropdownHoverIndex = -1;
    
    // 找到当前选中项
    m_dropdownSelectedIndex = 0;
    for (int i = 0; i < (int)m_dropdownItems.size(); i++) {
        if (m_dropdownItems[i] == prop.value) {
            m_dropdownSelectedIndex = i;
            break;
        }
    }
    
    // 对于布尔值特殊处理
    if (prop.value == L"真" || prop.value == L"true" || prop.value == L"1") {
        for (int i = 0; i < (int)m_dropdownItems.size(); i++) {
            if (m_dropdownItems[i] == L"真") {
                m_dropdownSelectedIndex = i;
                break;
            }
        }
    } else if (prop.value == L"假" || prop.value == L"false" || prop.value == L"0") {
        for (int i = 0; i < (int)m_dropdownItems.size(); i++) {
            if (m_dropdownItems[i] == L"假") {
                m_dropdownSelectedIndex = i;
                break;
            }
        }
    }
    
    // 注册窗口类
    static bool classRegistered = false;
    if (!classRegistered) {
        WNDCLASSEX wc = {0};
        wc.cbSize = sizeof(wc);
        wc.lpfnWndProc = DropdownWndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"PropertyGridDropdown";
        RegisterClassEx(&wc);
        classRegistered = true;
    }
    
    // 计算弹出窗口位置和大小
    POINT pt = {rect.X, rect.Y + rect.Height};
    ClientToScreen(m_hWnd, &pt);
    
    int popupHeight = (int)m_dropdownItems.size() * m_dropdownItemHeight + 4;
    int maxHeight = 200;
    if (popupHeight > maxHeight) popupHeight = maxHeight;
    
    // 创建弹出窗口
    m_hDropdownPopup = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        L"PropertyGridDropdown",
        nullptr,
        WS_POPUP | WS_VISIBLE,
        pt.x, pt.y,
        rect.Width, popupHeight,
        m_hWnd,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );
    
    if (m_hDropdownPopup) {
        SetWindowLongPtr(m_hDropdownPopup, GWLP_USERDATA, (LONG_PTR)this);
        SetFocus(m_hDropdownPopup);
    }
}

void PropertyGrid::CloseCustomDropdown(bool apply)
{
    if (!m_hDropdownPopup || !IsWindow(m_hDropdownPopup)) return;
    
    // 记录关闭时间和行号，用于防止立即重新打开
    m_dropdownCloseTime = GetTickCount();
    m_dropdownCloseRow = m_editingRow;
    
    if (apply && m_editingRow >= 0 && m_editingRow < (int)m_properties.size()) {
        if (m_dropdownSelectedIndex >= 0 && m_dropdownSelectedIndex < (int)m_dropdownItems.size()) {
            PropertyDef& prop = m_properties[m_editingRow];
            std::wstring newValue = m_dropdownItems[m_dropdownSelectedIndex];
            
            if (prop.value != newValue) {
                prop.value = newValue;
                
                if (m_propertyChangedCallback) {
                    m_propertyChangedCallback(prop.name, newValue);
                }
            }
        }
    }
    
    DestroyWindow(m_hDropdownPopup);
    m_hDropdownPopup = nullptr;
    m_editingRow = -1;
    m_dropdownItems.clear();
    
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void PropertyGrid::ShowColorPicker(PropertyDef& prop)
{
    CHOOSECOLOR cc = {0};
    static COLORREF customColors[16] = {0};
    
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = m_hWnd;
    cc.lpCustColors = customColors;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    
    // 解析当前颜色值
    try {
        int colorValue = std::stoi(prop.value);
        cc.rgbResult = colorValue;
    } catch (...) {
        cc.rgbResult = RGB(255, 255, 255);
    }
    
    if (ChooseColor(&cc)) {
        prop.value = std::to_wstring(cc.rgbResult);
        
        if (m_propertyChangedCallback) {
            m_propertyChangedCallback(prop.name, prop.value);
        }
        
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void PropertyGrid::ShowFontPicker(PropertyDef& prop)
{
    // TODO: 实现字体选择器
}

void PropertyGrid::ShowFilePicker(PropertyDef& prop)
{
    // TODO: 实现文件选择器
}

void PropertyGrid::ShowResourcePicker(PropertyDef& prop)
{
    if (prop.readOnly) return;
    
    // 根据属性类型设置不同的文件过滤器
    std::wstring filter;
    std::wstring title;
    
    if (prop.fneType == PropertyType::Picture || 
        prop.fneType == PropertyType::Icon ||
        prop.fneType == PropertyType::ImageList ||
        prop.editorType == PropertyEditorType::Image) {
        filter = L"图片文件 (*.bmp;*.jpg;*.jpeg;*.png;*.gif;*.ico)\0*.bmp;*.jpg;*.jpeg;*.png;*.gif;*.ico\0所有文件 (*.*)\0*.*\0\0";
        title = L"选择图片";
    } else if (prop.fneType == PropertyType::Music) {
        filter = L"音频文件 (*.wav;*.mp3;*.mid;*.midi)\0*.wav;*.mp3;*.mid;*.midi\0所有文件 (*.*)\0*.*\0\0";
        title = L"选择音频";
    } else if (prop.fneType == PropertyType::Font || prop.editorType == PropertyEditorType::Font) {
        // 字体选择使用系统字体对话框
        ShowFontPicker(prop);
        return;
    } else {
        filter = L"所有文件 (*.*)\0*.*\0\0";
        title = L"选择文件";
    }
    
    wchar_t fileName[MAX_PATH] = {0};
    
    OPENFILENAME ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = filter.c_str();
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = title.c_str();
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    
    if (GetOpenFileName(&ofn)) {
        // 获取文件名（不含路径）
        std::wstring fullPath = fileName;
        std::wstring fileNameOnly = fullPath;
        size_t lastSlash = fullPath.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos) {
            fileNameOnly = fullPath.substr(lastSlash + 1);
        }
        
        prop.value = fileNameOnly;
        
        // 触发属性变更回调
        if (m_propertyChangedCallback) {
            // 传递完整路径，由回调处理资源附加
            m_propertyChangedCallback(prop.name, fullPath);
        }
        
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
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

// 将 FNE 属性类型转换为编辑器类型
PropertyEditorType PropertyGrid::ConvertPropertyType(PropertyType fneType)
{
    switch (fneType) {
        case PropertyType::Int:
        case PropertyType::Double:
            return PropertyEditorType::Number;
        case PropertyType::Bool:
            return PropertyEditorType::Boolean;
        case PropertyType::Text:
        case PropertyType::EditPickText:
            return PropertyEditorType::Text;
        case PropertyType::PickInt:
        case PropertyType::PickText:
        case PropertyType::PickSpecInt:
            return PropertyEditorType::Enum;
        case PropertyType::Color:
        case PropertyType::ColorTrans:
        case PropertyType::ColorBack:
            return PropertyEditorType::Color;
        case PropertyType::Font:
            return PropertyEditorType::Font;
        case PropertyType::Picture:
        case PropertyType::Icon:
        case PropertyType::ImageList:
            return PropertyEditorType::Image;
        case PropertyType::Cursor:
            return PropertyEditorType::Enum;  // 鼠标指针使用下拉框选择
        case PropertyType::FileName:
            return PropertyEditorType::File;
        case PropertyType::DateTime:
            return PropertyEditorType::Text;  // 日期时间暂时用文本
        case PropertyType::Music:
        case PropertyType::Customize:
        default:
            return PropertyEditorType::Text;
    }
}

// 将 FNE 属性转换为 PropertyDef
PropertyDef PropertyGrid::ConvertFneProperty(const FnePropertyInfo& fneProp, 
                                              const std::wstring& currentValue)
{
    PropertyDef prop;
    prop.name = fneProp.name;
    prop.displayName = fneProp.name;
    
    // 属性名称映射（显示名称与内部名称不同）
    if (fneProp.name == L"帮助按钮") {
        prop.displayName = L"F1键打开帮助";
    } else if (fneProp.name == L"播放音乐") {
        prop.displayName = L"背景音乐";
    }
    
    prop.description = fneProp.description;
    prop.readOnly = fneProp.isReadOnly;
    prop.fneType = fneProp.type;
    prop.editorType = ConvertPropertyType(fneProp.type);
    
    // 设置枚举值
    prop.enumValues = fneProp.pickOptions;
    
    // 鼠标指针类型需要预定义选项
    if (fneProp.type == PropertyType::Cursor) {
        prop.enumValues = {
            L"默认", L"箭头", L"十字", L"文本选择", L"不可用",
            L"垂直调整", L"水平调整", L"左斜调整", L"右斜调整",
            L"移动", L"忙", L"后台运行", L"帮助选择", L"全部选择",
            L"手型"
        };
    }
    
    // 边框属性需要预定义选项
    if (fneProp.name == L"边框") {
        prop.enumValues = {
            L"无边框", L"普通可调边框", L"普通固定边框"
        };
    }
    
    // 位置属性需要预定义选项
    if (fneProp.name == L"位置") {
        prop.enumValues = {
            L"通常", L"居中"
        };
    }
    
    // 设置默认值
    switch (fneProp.type) {
        case PropertyType::Bool:
            // 某些布尔属性默认为"真"
            if (fneProp.name == L"可视" || fneProp.name == L"最大化按钮" || 
                fneProp.name == L"最小化按钮" || fneProp.name == L"控制按钮" ||
                fneProp.name == L"随意移动") {
                prop.defaultValue = L"真";
            } else {
                prop.defaultValue = L"假";
            }
            break;
        case PropertyType::Int:
            prop.defaultValue = L"0";
            break;
        case PropertyType::PickInt:
        case PropertyType::PickSpecInt:
            // 特定属性设置默认值
            if (fneProp.name == L"边框") {
                prop.defaultValue = L"普通可调边框";
            } else if (fneProp.name == L"位置") {
                prop.defaultValue = L"通常";
            } else if (!prop.enumValues.empty()) {
                prop.defaultValue = prop.enumValues[0];
            } else {
                prop.defaultValue = L"0";
            }
            break;
        case PropertyType::Double:
            prop.defaultValue = L"0.0";
            break;
        case PropertyType::Color:
        case PropertyType::ColorTrans:
        case PropertyType::ColorBack:
            prop.defaultValue = L"16777215";  // 白色
            break;
        case PropertyType::PickText:
        case PropertyType::EditPickText:
            // 枚举类型使用第一个选项作为默认值
            if (!fneProp.pickOptions.empty()) {
                prop.defaultValue = fneProp.pickOptions[0];
            } else {
                prop.defaultValue = L"";
            }
            break;
        case PropertyType::Cursor:
            prop.defaultValue = L"默认";
            break;
        default:
            prop.defaultValue = L"";
            break;
    }
    
    // 如果当前值为空，使用默认值
    if (currentValue.empty()) {
        prop.value = prop.defaultValue;
    } else {
        prop.value = currentValue;
    }
    
    // 设置分类（基于属性名或固定规则）
    if (prop.name == L"左边" || prop.name == L"顶边" || 
        prop.name == L"宽度" || prop.name == L"高度") {
        prop.category = L"位置";
    } else if (prop.name == L"可视" || prop.name == L"禁止") {
        prop.category = L"行为";
    } else if (prop.name.find(L"颜色") != std::wstring::npos || 
               prop.name.find(L"字体") != std::wstring::npos) {
        prop.category = L"外观";
    } else {
        prop.category = L"杂项";
    }
    
    return prop;
}

// 从支持库加载属性
void PropertyGrid::LoadPropertiesFromLibrary(const std::wstring& controlType,
                                              const std::map<std::wstring, std::wstring>& currentValues)
{
    Clear();
    
    // 从支持库查找组件信息
    const WindowUnitInfo* unitInfo = LibraryParser::GetInstance().FindWindowUnit(controlType);
    
    if (unitInfo) {
        OutputDebugStringW((L"[PropertyGrid] 从支持库加载控件属性: " + controlType + 
            L", 属性数: " + std::to_wstring(unitInfo->properties.size()) + L"\n").c_str());
        
        std::vector<PropertyDef> props;
        
        // 检查 FNE 是否已包含"名称"属性
        bool hasNamePropInFne = false;
        for (const auto& fneProp : unitInfo->properties) {
            if (fneProp.name == L"名称") {
                hasNamePropInFne = true;
                break;
            }
        }
        
        // 如果 FNE 没有"名称"属性，手动在最前面添加一个可编辑的名称属性
        if (!hasNamePropInFne) {
            auto itName = currentValues.find(L"名称");
            if (itName != currentValues.end()) {
                PropertyDef nameProp;
                nameProp.name = L"名称";
                nameProp.displayName = L"名称";
                nameProp.description = L"组件的名称标识";
                nameProp.category = L"基本";
                nameProp.editorType = PropertyEditorType::Text;
                nameProp.readOnly = false;
                nameProp.value = itName->second;
                props.push_back(nameProp);
            }
        }
        
        // 检查控制按钮的值，用于决定子选项是否禁用
        bool controlBoxEnabled = true;
        auto itControlBox = currentValues.find(L"控制按钮");
        if (itControlBox != currentValues.end()) {
            controlBoxEnabled = (itControlBox->second == L"真" || itControlBox->second == L"true" || itControlBox->second == L"1");
        }
        
        for (const auto& fneProp : unitInfo->properties) {
            // 跳过"底图方式"，稍后添加到"背景图片"后面
            if (fneProp.name == L"底图方式") {
                continue;
            }
            
            // 跳过"最大化按钮"和"最小化按钮"，稍后添加到"控制按钮"后面
            if (fneProp.name == L"最大化按钮" || fneProp.name == L"最小化按钮") {
                continue;
            }
            
            // 获取当前值
            std::wstring currentValue;
            auto it = currentValues.find(fneProp.name);
            if (it != currentValues.end()) {
                currentValue = it->second;
            }
            
            PropertyDef prop = ConvertFneProperty(fneProp, currentValue);
            // "名称"属性始终允许编辑（FNE可能标记为只读，但用户需要自定义名称）
            if (fneProp.name == L"名称") {
                prop.readOnly = false;
                prop.editorType = PropertyEditorType::Text;
            }
            props.push_back(prop);
            
            // 在"控制按钮"属性后添加"最大化按钮"和"最小化按钮"属性
            if (fneProp.name == L"控制按钮") {
                // 添加"最大化按钮"
                for (const auto& searchProp : unitInfo->properties) {
                    if (searchProp.name == L"最大化按钮") {
                        std::wstring maxBtnValue;
                        auto itMaxBtn = currentValues.find(L"最大化按钮");
                        if (itMaxBtn != currentValues.end()) {
                            maxBtnValue = itMaxBtn->second;
                        }
                        
                        PropertyDef maxBtnProp = ConvertFneProperty(searchProp, maxBtnValue);
                        maxBtnProp.displayName = L"    最大化按钮";  // 缩进表示子选项
                        maxBtnProp.readOnly = !controlBoxEnabled;  // 控制按钮为假时禁用
                        props.push_back(maxBtnProp);
                        break;
                    }
                }
                
                // 添加"最小化按钮"
                for (const auto& searchProp : unitInfo->properties) {
                    if (searchProp.name == L"最小化按钮") {
                        std::wstring minBtnValue;
                        auto itMinBtn = currentValues.find(L"最小化按钮");
                        if (itMinBtn != currentValues.end()) {
                            minBtnValue = itMinBtn->second;
                        }
                        
                        PropertyDef minBtnProp = ConvertFneProperty(searchProp, minBtnValue);
                        minBtnProp.displayName = L"    最小化按钮";  // 缩进表示子选项
                        minBtnProp.readOnly = !controlBoxEnabled;  // 控制按钮为假时禁用
                        props.push_back(minBtnProp);
                        break;
                    }
                }
            }
            
            // 在"背景图片"属性后添加"底图方式"属性
            if (fneProp.name == L"背景图片") {
                // 查找"底图方式"属性
                for (const auto& searchProp : unitInfo->properties) {
                    if (searchProp.name == L"底图方式") {
                        std::wstring modeValue;
                        auto itMode = currentValues.find(L"底图方式");
                        if (itMode != currentValues.end()) {
                            modeValue = itMode->second;
                        }
                        
                        PropertyDef modeProp = ConvertFneProperty(searchProp, modeValue);
                        modeProp.displayName = L"    底图方式";  // 缩进表示子选项
                        props.push_back(modeProp);
                        break;
                    }
                }
            }
            
            // 在"播放音乐"属性后添加"播放次数"属性
            if (fneProp.name == L"播放音乐") {
                PropertyDef playCountProp;
                playCountProp.name = L"播放次数";
                playCountProp.displayName = L"    播放次数";  // 缩进表示子选项
                playCountProp.description = L"背景音乐播放次数";
                playCountProp.editorType = PropertyEditorType::Enum;
                playCountProp.enumValues = { L"循环播放", L"仅播放一次", L"不播放" };
                playCountProp.defaultValue = L"不播放";
                playCountProp.category = L"杂项";
                playCountProp.readOnly = false;
                
                // 获取当前值
                auto itPlayCount = currentValues.find(L"播放次数");
                if (itPlayCount != currentValues.end()) {
                    playCountProp.value = itPlayCount->second;
                } else {
                    playCountProp.value = playCountProp.defaultValue;
                }
                
                props.push_back(playCountProp);
            }
        }
        
        SetProperties(props);
    }
    else {
        OutputDebugStringW((L"[PropertyGrid] 未找到控件类型: " + controlType + L"\n").c_str());
        
        // 后备：使用当前值创建基本属性
        std::vector<PropertyDef> props;
        
        for (const auto& pair : currentValues) {
            PropertyDef prop;
            prop.name = pair.first;
            prop.displayName = pair.first;
            prop.value = pair.second;
            prop.category = L"属性";
            prop.editorType = PropertyEditorType::Text;
            props.push_back(prop);
        }
        
        SetProperties(props);
    }
    
    SetObjectInfo(controlType, controlType);
}
