#include "OutputPanel.h"
#include "Theme.h"

extern AppTheme g_CurrentTheme;

// 将 Windows COLORREF (BGR) 转换为 GDI+ Color (ARGB)
inline Gdiplus::Color ColorRefToGdiplus(COLORREF cr) {
    return Gdiplus::Color(255, GetRValue(cr), GetGValue(cr), GetBValue(cr));
}

OutputPanel::OutputPanel(HWND hWnd)
    : m_hWnd(hWnd)
    , m_hOutputEdit(nullptr)
    , m_hHintEdit(nullptr)
    , m_activeTab(OutputTabType::Hint)  // 默认显示提示标签
    , m_tabHeight(28)
    , m_hoveredTab(-1)
    , m_gdiplusToken(0)
{
    // 初始化GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    
    RECT rect;
    GetClientRect(m_hWnd, &rect);
    
    // 创建输出编辑框（初始隐藏，编译/运行时自动切换显示）
    m_hOutputEdit = CreateWindowW(L"EDIT", L"输出信息:\r\n就绪.",
        WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL,
        0, m_tabHeight, rect.right, rect.bottom - m_tabHeight,
        m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
    
    // 创建提示编辑框（默认显示）
    m_hHintEdit = CreateWindowW(L"EDIT", L"选择代码中的命令以查看帮助信息",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY | WS_VSCROLL,
        0, m_tabHeight, rect.right, rect.bottom - m_tabHeight,
        m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
    
    // 设置字体
    HFONT hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"微软雅黑");
    SendMessage(m_hOutputEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
    SendMessage(m_hHintEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
}

OutputPanel::~OutputPanel()
{
    if (m_hOutputEdit) DestroyWindow(m_hOutputEdit);
    if (m_hHintEdit) DestroyWindow(m_hHintEdit);
    
    if (m_gdiplusToken) {
        GdiplusShutdown(m_gdiplusToken);
    }
}

void OutputPanel::OnPaint(HDC hdc)
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
    
    // 绘制标签栏
    DrawTabs(graphics, rect);
    
    // 复制到屏幕（只复制标签栏区域，编辑框由系统绘制）
    BitBlt(hdc, 0, 0, rect.right, m_tabHeight, memDC, 0, 0, SRCCOPY);
    
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
}

void OutputPanel::DrawTabs(Graphics& g, const RECT& rect)
{
    // 标签栏背景
    SolidBrush tabBarBrush(ColorRefToGdiplus(g_CurrentTheme.tableHeaderBg));
    g.FillRectangle(&tabBarBrush, 0, 0, rect.right, m_tabHeight);
    
    Font tabFont(L"微软雅黑", 10);
    SolidBrush textBrush(ColorRefToGdiplus(g_CurrentTheme.text));
    SolidBrush activeTextBrush(Color(255, 255, 255));
    SolidBrush activeBgBrush(Color(0, 122, 204));
    SolidBrush hoverBrush(Color(60, 60, 60));
    
    // 标签定义
    const wchar_t* tabNames[] = { L"输出", L"提示" };
    int tabWidth = 80;
    
    for (int i = 0; i < 2; i++) {
        int x = i * tabWidth;
        RectF tabRect((float)x, 0, (float)tabWidth, (float)m_tabHeight);
        
        bool isActive = (i == 0 && m_activeTab == OutputTabType::Output) ||
                       (i == 1 && m_activeTab == OutputTabType::Hint);
        bool isHovered = (i == m_hoveredTab);
        
        // 绘制标签背景
        if (isActive) {
            g.FillRectangle(&activeBgBrush, tabRect);
        } else if (isHovered) {
            g.FillRectangle(&hoverBrush, tabRect);
        }
        
        // 绘制标签文字
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentCenter);
        g.DrawString(tabNames[i], -1, &tabFont, tabRect, &format, 
                    isActive ? &activeTextBrush : &textBrush);
    }
    
    // 绘制底部分隔线
    Pen linePen(Color(60, 60, 60));
    g.DrawLine(&linePen, 0, m_tabHeight - 1, rect.right, m_tabHeight - 1);
}

void OutputPanel::OnSize(int width, int height)
{
    // 调整编辑框大小
    if (m_hOutputEdit) {
        MoveWindow(m_hOutputEdit, 0, m_tabHeight, width, height - m_tabHeight, TRUE);
    }
    if (m_hHintEdit) {
        MoveWindow(m_hHintEdit, 0, m_tabHeight, width, height - m_tabHeight, TRUE);
    }
    InvalidateRect(m_hWnd, NULL, FALSE);
}

void OutputPanel::OnLButtonDown(int x, int y)
{
    int tabIndex = HitTestTab(x, y);
    if (tabIndex >= 0) {
        SetActiveTab(tabIndex == 0 ? OutputTabType::Output : OutputTabType::Hint);
    }
}

int OutputPanel::HitTestTab(int x, int y)
{
    if (y >= m_tabHeight) return -1;
    
    int tabWidth = 80;
    int tabIndex = x / tabWidth;
    if (tabIndex < 2) {
        return tabIndex;
    }
    return -1;
}

void OutputPanel::SetActiveTab(OutputTabType tab)
{
    if (m_activeTab != tab) {
        m_activeTab = tab;
        UpdateEditVisibility();
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
}

void OutputPanel::UpdateEditVisibility()
{
    if (m_activeTab == OutputTabType::Output) {
        ShowWindow(m_hOutputEdit, SW_SHOW);
        ShowWindow(m_hHintEdit, SW_HIDE);
    } else {
        ShowWindow(m_hOutputEdit, SW_HIDE);
        ShowWindow(m_hHintEdit, SW_SHOW);
    }
}

void OutputPanel::AppendOutput(const std::wstring& text, bool autoSwitch)
{
    m_outputText += text;
    if (m_hOutputEdit) {
        // 追加文本到末尾
        int len = GetWindowTextLength(m_hOutputEdit);
        SendMessage(m_hOutputEdit, EM_SETSEL, len, len);
        SendMessage(m_hOutputEdit, EM_REPLACESEL, FALSE, (LPARAM)text.c_str());
    }
    
    // 如果需要自动切换且当前不是输出标签，则切换到输出标签
    if (autoSwitch && m_activeTab != OutputTabType::Output) {
        SetActiveTab(OutputTabType::Output);
    }
}

void OutputPanel::ClearOutput()
{
    m_outputText.clear();
    if (m_hOutputEdit) {
        SetWindowText(m_hOutputEdit, L"");
    }
}

void OutputPanel::SetHint(const CommandHint& hint)
{
    m_currentHint = hint;
    UpdateHintDisplay();
}

void OutputPanel::ClearHint()
{
    m_currentHint = CommandHint();
    if (m_hHintEdit) {
        SetWindowText(m_hHintEdit, L"选择代码中的命令以查看帮助信息");
    }
}

void OutputPanel::SetCurrentCommand(const std::wstring& commandName, int lineNumber)
{
    // 这里可以根据命令名称查找帮助信息
    // 暂时显示基本信息
    CommandHint hint;
    hint.commandName = commandName;
    hint.description = L"正在查找命令帮助信息...";
    SetHint(hint);
}

void OutputPanel::UpdateHintDisplay()
{
    if (!m_hHintEdit) return;
    
    std::wstring hintText;
    
    if (m_currentHint.commandName.empty()) {
        hintText = L"选择代码中的命令以查看帮助信息";
    } else {
        // 调用格式（包含英文名称）
        if (!m_currentHint.syntax.empty()) {
            hintText = L"    " + m_currentHint.syntax + L"\r\n";
        }
        
        // 命令说明
        if (!m_currentHint.description.empty()) {
            hintText += L"    " + m_currentHint.description + L"\r\n";
        }
        
        // 参数详细信息
        if (!m_currentHint.parameters.empty()) {
            for (const auto& param : m_currentHint.parameters) {
                hintText += L"    " + param + L"\r\n";
            }
        }
        
        // 空行
        hintText += L"\r\n";
        
        // 操作系统需求
        if (!m_currentHint.returnValue.empty()) {
            hintText += L"    操作系统需求： " + m_currentHint.returnValue + L"\r\n";
        }
    }
    
    SetWindowText(m_hHintEdit, hintText.c_str());
}
