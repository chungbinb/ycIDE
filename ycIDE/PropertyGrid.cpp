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
    // 检查是否点击在属性区域
    if (y < 35) return;  // 标题栏区域
    
    int row = (y - 35 + m_scrollPos) / m_rowHeight;
    if (row >= 0 && row < (int)m_properties.size()) {
        m_selectedRow = row;
        InvalidateRect(m_hWnd, NULL, FALSE);
        
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
            m_scrollPos = max(0, m_scrollPos - m_rowHeight);
            break;
        case SB_LINEDOWN:
            m_scrollPos += m_rowHeight;
            break;
        case SB_PAGEUP:
            m_scrollPos = max(0, m_scrollPos - 100);
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
    m_scrollPos = min(m_scrollPos, maxScroll);
    
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
    
    // 结束之前的编辑
    if (m_editingRow >= 0) {
        FinishEdit(true);
    }
    
    m_editingRow = row;
    m_selectedRow = row;
    
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    
    int editY = 35 + row * m_rowHeight - m_scrollPos;
    int editX = NAME_COLUMN_WIDTH + 2;
    int editW = rect.right - NAME_COLUMN_WIDTH - 4;
    int editH = m_rowHeight - 2;
    
    // 根据属性类型创建不同的编辑控件
    switch (prop.editorType) {
        case PropertyEditorType::Boolean:
            // 布尔型直接切换值
            ToggleBooleanValue(prop);
            m_editingRow = -1;
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

void PropertyGrid::CreateTextEditor(const PropertyDef& prop, const Rect& rect)
{
    m_hEditControl = CreateWindowW(L"EDIT", prop.value.c_str(),
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        rect.X, rect.Y, rect.Width, rect.Height,
        m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
    
    if (m_hEditControl) {
        SendMessage(m_hEditControl, EM_SETSEL, 0, -1);
        SetFocus(m_hEditControl);
    }
}

void PropertyGrid::CreateComboEditor(const PropertyDef& prop, const Rect& rect)
{
    m_hComboBox = CreateWindowW(L"COMBOBOX", nullptr,
        WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
        rect.X, rect.Y, rect.Width, rect.Height * 6,
        m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
    
    if (m_hComboBox) {
        // 添加枚举值
        int selectedIndex = 0;
        for (int i = 0; i < (int)prop.enumValues.size(); i++) {
            SendMessage(m_hComboBox, CB_ADDSTRING, 0, (LPARAM)prop.enumValues[i].c_str());
            if (prop.enumValues[i] == prop.value) {
                selectedIndex = i;
            }
        }
        SendMessage(m_hComboBox, CB_SETCURSEL, selectedIndex, 0);
        SetFocus(m_hComboBox);
    }
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
            return PropertyEditorType::Cursor;
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
    prop.description = fneProp.description;
    prop.readOnly = fneProp.isReadOnly;
    prop.fneType = fneProp.type;
    prop.editorType = ConvertPropertyType(fneProp.type);
    prop.value = currentValue;
    
    // 设置枚举值
    prop.enumValues = fneProp.pickOptions;
    
    // 设置默认值
    switch (fneProp.type) {
        case PropertyType::Bool:
            prop.defaultValue = L"假";
            break;
        case PropertyType::Int:
        case PropertyType::PickInt:
        case PropertyType::PickSpecInt:
            prop.defaultValue = L"0";
            break;
        case PropertyType::Color:
        case PropertyType::ColorTrans:
        case PropertyType::ColorBack:
            prop.defaultValue = L"16777215";  // 白色
            break;
        default:
            prop.defaultValue = L"";
            break;
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
        
        for (const auto& fneProp : unitInfo->properties) {
            // 获取当前值
            std::wstring currentValue;
            auto it = currentValues.find(fneProp.name);
            if (it != currentValues.end()) {
                currentValue = it->second;
            }
            
            PropertyDef prop = ConvertFneProperty(fneProp, currentValue);
            props.push_back(prop);
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
